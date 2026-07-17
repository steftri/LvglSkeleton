#include <Arduino.h>

#include "worker_task.h"


static const size_t LIGHTSTRIPE_NUM_PIXELS = 8; // Number of pixels in the light stripe
static const uint8_t LIGHTSTRIPE_PIN = 38; // GPIO pin connected to the light stripe data line


// Porsche Taycan (Gen 1, 2019) AC charging via Porsche Mobile Charger Connect
// 3-phase 400 V / 32 A, max 22 kW, 93.4 kWh gross / 83.7 kWh usable battery
static const float PMC_AC_PHASE_VOLTAGE_V = 400.0f;     ///< Three-phase line voltage in V
static const float PMC_AC_SQRT3           = 1.7320508f;  ///< sqrt(3) for three-phase power
static const float TAYCAN_BATTERY_KWH     = 83.7f;      ///< Usable battery capacity in kWh

struct TaycanProfilePoint { float soc; float powerKW; };

/// Piecewise-linear AC charging power curve via Porsche Mobile Charger Connect (SoC % → kW)
static const TaycanProfilePoint TAYCAN_PROFILE[] =
{
  {  0.0f, 22.0f },
  { 80.0f, 22.0f },
  { 85.0f, 18.0f },
  { 90.0f, 13.0f },
  { 95.0f,  8.0f },
  {100.0f,  3.0f },
};
static const size_t TAYCAN_PROFILE_LEN = sizeof(TAYCAN_PROFILE) / sizeof(TAYCAN_PROFILE[0]);

/// Linearly interpolates charging power [kW] for a given SoC [%].
static float taycanPowerKW(float soc)
{
  if (soc <= TAYCAN_PROFILE[0].soc)
    return TAYCAN_PROFILE[0].powerKW;
  if (soc >= TAYCAN_PROFILE[TAYCAN_PROFILE_LEN - 1].soc)
    return TAYCAN_PROFILE[TAYCAN_PROFILE_LEN - 1].powerKW;

  for (size_t i = 0; i < TAYCAN_PROFILE_LEN - 1; ++i)
  {
    if (soc >= TAYCAN_PROFILE[i].soc && soc < TAYCAN_PROFILE[i + 1].soc)
    {
      float t = (soc - TAYCAN_PROFILE[i].soc)
                / (TAYCAN_PROFILE[i + 1].soc - TAYCAN_PROFILE[i].soc);
      return TAYCAN_PROFILE[i].powerKW
             + t * (TAYCAN_PROFILE[i + 1].powerKW - TAYCAN_PROFILE[i].powerKW);
    }
  }
  return TAYCAN_PROFILE[TAYCAN_PROFILE_LEN - 1].powerKW;
}

enum class ENotificationBits : uint32_t
{
  ChangedLightstripeEnableState = (1UL << 0),
  ChangedLightstripeColorSettings = (1UL << 1),
  ChangedChargingData = (1UL << 2)
};



WorkerTask *WorkerTask::mp_thisInstance = nullptr; // Initialize static instance pointer


WorkerTask::WorkerTask(LightstripeSettings &settings, LightstripeData &data, SystemData &systemData)
  : mp_TaskHandle(nullptr)
  , m_Settings(settings)
  , m_Data(data)
  , m_SystemData(systemData)
  , m_Lightstripe(LIGHTSTRIPE_PIN, LIGHTSTRIPE_NUM_PIXELS)
  , mf32_SimSoc(5.0f)
  , mf32_SimCurrentA(0.0f)
  , mu16_SimDurationMin(0)
  , mf32_SimPowerKWh(0.0f)
  , mf32_SimChargingSpeedKW(0.0f)
{
    mp_thisInstance = this;
}


void WorkerTask::begin()
{
  Serial.println("Creating WorkerTask");
  
  xTaskCreatePinnedToCore(
     task,                     // Task function
     "WorkerTask",             // Task name
     WORKER_TASK_STACK_SIZE,   // Stack size
     nullptr,                  // Parameters
     1,                        // Priority
     &mp_TaskHandle,           // Task handle
     0                         // Core 0 (PRO_CPU, more WiFi interference but better for background tasks)
  );
}


void WorkerTask::task(void *pvParameters) 
{
  if(mp_thisInstance == nullptr)
  {
    Serial.println("Error: WorkerTask instance not set");
    vTaskDelete(nullptr); // Delete the task if instance is not set
    return;
  }

  Serial.println("Worker Task started");    
  mp_thisInstance->setup(); // Call setup once at the beginning

  while (true)
  {
    mp_thisInstance->loop(); // Call loop for periodic updates
  }
}


void WorkerTask::setup()
{
  Serial.println("WorkerTask running.");

  m_Settings.registerObserver(this); 
  m_SystemData.registerObserver(this);

  m_Lightstripe.setup();
  m_Lightstripe.enable(); // Enable the light stripe hardware

  m_Lightstripe.setValueTarget(Lightstripe::EValueTarget::Speed);
  m_Lightstripe.setWaveForm(Lightstripe::EWaveForm::Sine);
  m_Lightstripe.setWaveInterval(static_cast<float>(LIGHTSTRIPE_NUM_PIXELS));
  m_Lightstripe.setWaveMaxSpeed(2.0f); // 1 cycle per second at setValue(1.0)
  m_Lightstripe.setMinRgbColor(0x000000); // Minimum color (black/off)
  m_Lightstripe.setMaxRgbColor(0xFF0000); // Maximum color (red/full brightness)
  m_Lightstripe.setValue(1.0f);  // 5 seconds for a full wave cycle (1.0 / 0.2 = 5 seconds)
}


void WorkerTask::loop()
{
  static uint32_t lastUpdateTime = 0;
  uint32_t currentTime = millis();
  uint32_t u32_NotifiedValue = 0;

  xTaskNotifyWait(0, 0xffff, &u32_NotifiedValue, pdMS_TO_TICKS(20UL)); // Wait for notifications with a timeout
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::ChangedLightstripeEnableState))
  {
    if (m_Settings.getEnable(0) && m_SystemData.getChargingCurrentA() > 0)
    {
      m_Lightstripe.enable();
    }
    else
    {
      m_Lightstripe.disable();
    }
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::ChangedLightstripeColorSettings))
  {
    m_Lightstripe.setValueTarget(static_cast<Lightstripe::EValueTarget>(m_Settings.getValueTarget(0)));
    m_Lightstripe.setWaveForm(static_cast<Lightstripe::EWaveForm>(m_Settings.getWaveForm(0)));
    m_Lightstripe.setWaveInterval(m_Settings.getWaveInterval(0));
    m_Lightstripe.setWaveMaxSpeed(m_Settings.getWaveMaxSpeed(0));
    m_Lightstripe.setMinRgbColor(m_Settings.getMinRgbColor(0));
    m_Lightstripe.setMaxRgbColor(m_Settings.getMaxRgbColor(0));
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::ChangedChargingData))
  {
    if (m_SystemData.getChargingCurrentA() > 0)
    {
      m_Lightstripe.enable();
    }
    else
    {
      m_Lightstripe.disable();
    }
  }

//  m_Lightstripe.loop(currentTime); // Update the light stripe based on the current time

  updateSimulation(currentTime);

  if (currentTime - lastUpdateTime >= 60*1000UL) // Update every 60 seconds
  {
    lastUpdateTime = currentTime;
    Serial.printf("  Free WorkerTask stack: %u/%u (Usage: %u%%)\n",
                  uxTaskGetStackHighWaterMark(nullptr), WORKER_TASK_STACK_SIZE,
                  ((WORKER_TASK_STACK_SIZE - uxTaskGetStackHighWaterMark(nullptr)) * 100) / WORKER_TASK_STACK_SIZE); // nullptr = aktueller Task
  }

  vTaskDelay(pdMS_TO_TICKS(20UL)); // Alle 20 Millisekunden aktualisieren
}


void WorkerTask::updateSimulation(uint32_t u32_CurrentTimeMs)
{
  // Porsche Taycan Gen 1 DC fast-charging simulation
  // 1 real second ≙ SIM_STEP_MIN simulated minutes (demo speed)
  static uint32_t u32_LastSimUpdateMs = 0;
  static float    f32_DurationAccMin  = 0.0f; // fractional minute accumulator
  static bool     b_IsPausing         = false;
  static uint32_t u32_PauseStartMs    = 0;
  static const uint32_t SIM_INTERVAL_MS   = 250UL;
  static const uint32_t PAUSE_DURATION_MS = 10000UL; // 10 s pause between cycles

  if (u32_CurrentTimeMs - u32_LastSimUpdateMs < SIM_INTERVAL_MS)
    return;

  u32_LastSimUpdateMs = u32_CurrentTimeMs;

  // --- Inter-cycle pause ---
  if (b_IsPausing)
  {
    if (u32_CurrentTimeMs - u32_PauseStartMs >= PAUSE_DURATION_MS)
    {
      // Pause over: reset state and begin new cycle
      b_IsPausing         = false;
      mf32_SimSoc         = 5.0f;
      mf32_SimPowerKWh    = 0.0f;
      f32_DurationAccMin  = 0.0f;
    }
    else
    {
      return; // hold display values until pause expires
    }
  }

  // Determine charging power from SoC-dependent profile
  mf32_SimChargingSpeedKW = taycanPowerKW(mf32_SimSoc);

  // Each real second represents SIM_STEP_MIN simulated minutes
  static const float SIM_STEP_MIN = 1.0f;
  static const float SIM_STEP_H   = SIM_STEP_MIN / 60.0f;

  // Advance SoC: deltaSOC [%] = P [kW] / C [kWh] * dt [h] * 100
  float f32_DeltaSoc = (mf32_SimChargingSpeedKW / TAYCAN_BATTERY_KWH) * SIM_STEP_H * 100.0f;
  mf32_SimSoc += f32_DeltaSoc;

  if (mf32_SimSoc >= 100.0f)
  {
    // Charge complete – hold 100 % display, start inter-cycle pause
    mf32_SimSoc             = 100.0f;
    mf32_SimChargingSpeedKW = 0.0f;
    mf32_SimCurrentA        = 0.0f;
    b_IsPausing             = true;
    u32_PauseStartMs        = u32_CurrentTimeMs;
    m_SystemData.setSocPercent(100.0f);
    m_SystemData.setChargingCurrentA(0.0f);
    m_SystemData.setDurationMin(mu16_SimDurationMin);
    m_SystemData.setPowerConsumptionKWh(mf32_SimPowerKWh);
    m_SystemData.setChargingSpeedKW(0.0f);
    return;
  }

  // AC current per phase: I = P / (sqrt(3) * U_L)
  mf32_SimCurrentA = (mf32_SimChargingSpeedKW * 1000.0f) / (PMC_AC_SQRT3 * PMC_AC_PHASE_VOLTAGE_V);

  // Integrate transferred energy
  mf32_SimPowerKWh += mf32_SimChargingSpeedKW * SIM_STEP_H;

  // Physical charging time for this SoC step: t = (ΔSoC/100 · C_kWh / P_kW) · 60 min
  f32_DurationAccMin += (f32_DeltaSoc / 100.0f * TAYCAN_BATTERY_KWH / mf32_SimChargingSpeedKW) * 60.0f;
  mu16_SimDurationMin  = static_cast<uint16_t>(f32_DurationAccMin);

  m_SystemData.setSocPercent(mf32_SimSoc);
  m_SystemData.setChargingCurrentA(mf32_SimCurrentA);
  m_SystemData.setDurationMin(mu16_SimDurationMin);
  m_SystemData.setPowerConsumptionKWh(mf32_SimPowerKWh);
  m_SystemData.setChargingSpeedKW(mf32_SimChargingSpeedKW);
}



void WorkerTask::onDataChanged(Data &r_Data, EDataField e_Field)
{
  if(&r_Data == &m_Settings)
  {
    onSettingsChanged(e_Field);
  }
  else if(&r_Data == &m_SystemData)
  {
    onChargingDataChanged(e_Field);
  }
  else
  {
    Serial.println("WorkerTask: Unknown data source changed");
  }
}


void WorkerTask::onSettingsChanged(EDataField e_Field)
{
  switch (static_cast<LightstripeSettings::EField>(e_Field))
  {
    case LightstripeSettings::EField::EnableState:
      Serial.println("WorkerTask: Enable state updated");
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::ChangedLightstripeEnableState), eSetBits);
      break;
    case LightstripeSettings::EField::ColorSettings:
      Serial.println("WorkerTask: Color settings updated");
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::ChangedLightstripeColorSettings), eSetBits);
      break;
    default:
      Serial.println("WorkerTask: Unknown Lightstripe settings field changed");
      break;
  }
}


void WorkerTask::onChargingDataChanged(EDataField e_Field)
{
  xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::ChangedChargingData), eSetBits);
}


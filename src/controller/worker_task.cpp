#include <Arduino.h>

#include "worker_task.h"


static const size_t LIGHTSTRIPE_NUM_PIXELS = 8; // Number of pixels in the light stripe
static const uint8_t LIGHTSTRIPE_PIN = 38; // GPIO pin connected to the light stripe data line

enum class ENotificationBits : uint32_t
{
  ChangedLightstripeEnableState = (1UL << 0),
  ChangedLightstripeColorSettings = (1UL << 1)
};



WorkerTask *WorkerTask::mp_thisInstance = nullptr; // Initialize static instance pointer


WorkerTask::WorkerTask(LightstripeSettings &settings, LightstripeData &data, SystemData &systemData)
  : mp_TaskHandle(nullptr)
  , m_Settings(settings)
  , m_Data(data)
  , m_SystemData(systemData)
  , m_Lightstripe(LIGHTSTRIPE_PIN, LIGHTSTRIPE_NUM_PIXELS) 
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
    if (m_Settings.getEnable(0))
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

  m_Lightstripe.setValue((currentTime % 10000) / 10000.0f); // Update the value based on time for demonstration
  m_Lightstripe.loop(currentTime); // Update the light stripe based on the current time

  if (currentTime - lastUpdateTime >= 60*1000UL) // Update every 60 seconds
  {
    lastUpdateTime = currentTime;
    Serial.printf("  Free WorkerTask stack: %u/%u (Usage: %u%%)\n",
                  uxTaskGetStackHighWaterMark(nullptr), WORKER_TASK_STACK_SIZE,
                  ((WORKER_TASK_STACK_SIZE - uxTaskGetStackHighWaterMark(nullptr)) * 100) / WORKER_TASK_STACK_SIZE); // nullptr = aktueller Task
  }

  vTaskDelay(pdMS_TO_TICKS(20UL)); // Alle 20 Millisekunden aktualisieren
}



void WorkerTask::onDataChanged(Data &r_Data, EDataField e_Field)
{
  if(&r_Data == &m_Settings)
  {
    onSettingsChanged(e_Field);
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


void WorkerTask::onDataChanged(EDataField e_Field)
{
  // Handle data changes if needed
}


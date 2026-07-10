#include <Arduino.h>

#include "worker_task.h"


static const size_t LIGHTSTRIPE_NUM_PIXELS = 8; // Number of pixels in the light stripe
static const uint8_t LIGHTSTRIPE_PIN = 38; // GPIO pin connected to the light stripe data line



WorkerTask *WorkerTask::mp_thisInstance = nullptr; // Initialize static instance pointer


WorkerTask::WorkerTask(LightstripeSettings &settings, LightstripeData &data)
  : mp_TaskHandle(nullptr)
  , m_Settings(settings)
  , m_Data(data)
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

  m_Lightstripe.setup();
  m_Lightstripe.enable(); // Enable the light stripe hardware

  m_Lightstripe.setValueTarget(Lightstripe::EValueTarget::Hue);
  m_Lightstripe.setWaveForm(Lightstripe::EWaveForm::Sine);
  m_Lightstripe.setWaveInterval(static_cast<float>(LIGHTSTRIPE_NUM_PIXELS)/2.0f);
  m_Lightstripe.setWaveMaxSpeed(1000.0f); // 1 second for a full wave cycle
  m_Lightstripe.setMinRgbColor(0x000000); // Minimum color (black/off)
  m_Lightstripe.setMaxRgbColor(0xFF0000); // Maximum color (red/full brightness)
  m_Lightstripe.setValue(1.0f); 


}


void WorkerTask::loop()
{
  static uint32_t lastUpdateTime = 0;
  uint32_t currentTime = millis();

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

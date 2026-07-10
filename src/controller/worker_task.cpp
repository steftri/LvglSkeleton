#include <Arduino.h>

#include "worker_task.h"


static const size_t LIGHTSTRIPE_NUM_PIXELS = 30; // Number of pixels in the light stripe
static const uint8_t LIGHTSTRIPE_PIN = 38; // GPIO pin connected to the light stripe data line



WorkerTask *WorkerTask::mp_thisInstance = nullptr; // Initialize static instance pointer


WorkerTask::WorkerTask(WorkerSettings &settings, WorkerData &data)
  : mp_TaskHandle(nullptr)
  , m_Settings(settings)
  , m_Data(data)
  , m_LightstripeHal(LIGHTSTRIPE_NUM_PIXELS, LIGHTSTRIPE_PIN) 
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
     10,                       // Priority
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

  m_LightstripeHal.setup();
  m_LightstripeHal.enable(); // Enable the light stripe hardware
}


void WorkerTask::loop()
{
  static uint32_t lastUpdateTime = 0;
  uint32_t currentTime = millis();


  m_LightstripeHal.setPixelColor(0, 0xff0000); // Set first pixel to red
  m_LightstripeHal.setPixelColor(1, 0x00ff00); // Set second pixel to green
  m_LightstripeHal.setPixelColor(2, 0x0000ff); // Set third pixel to blue

  m_LightstripeHal.show(); // Update the light stripe to display the changes


  if (currentTime - lastUpdateTime >= 60*1000UL) // Update every 60 seconds
  {
    lastUpdateTime = currentTime;
    Serial.printf("  Free WorkerTask stack: %u/%u (Usage: %u%%)\n",
                  uxTaskGetStackHighWaterMark(nullptr), WORKER_TASK_STACK_SIZE,
                  ((WORKER_TASK_STACK_SIZE - uxTaskGetStackHighWaterMark(nullptr)) * 100) / WORKER_TASK_STACK_SIZE); // nullptr = aktueller Task
  }

  vTaskDelay(pdMS_TO_TICKS(100UL)); // Alle 100 Millisekunden aktualisieren
}

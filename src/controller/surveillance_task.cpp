#include <Arduino.h>

#include "surveillance_task.h"

#include "controller.h"

extern Controller g_controller; // Declare the external Controller instance


SurveillanceTask *SurveillanceTask::mp_thisInstance = nullptr; // Initialize static instance pointer


SurveillanceTask::SurveillanceTask(SurveillanceData &data)
  : mp_TaskHandle(nullptr)
  , m_Data(data)
{
    mp_thisInstance = this;
}


void SurveillanceTask::begin()
{
  Serial.println("Creating SurveillanceTask");
  
  xTaskCreatePinnedToCore(
     task,                     // Task function
     "Surveillance",           // Task name
     SURVEILLANCE_TASK_STACK_SIZE, // Stack size
     nullptr,                  // Parameters
     1,                        // Priority
     &mp_TaskHandle,           // Task handle
     0                         // Core 0 (PRO_CPU, more WiFi interference but better for background tasks)
  );
}



void SurveillanceTask::task(void *pvParameters)
{
  if(mp_thisInstance == nullptr)
  {
    Serial.println("Error: SurveillanceTask instance not set");
    vTaskDelete(nullptr); // Delete the task if instance is not set
    return;
  }

  Serial.println("Surveillance Task started");    
  mp_thisInstance->setup(); // Call setup once at the beginning

  while (true)
  {
    mp_thisInstance->loop(); // Call loop for periodic updates
  }

}


void SurveillanceTask::setup()
{
  Serial.println("SurveillanceTask running.");
}


void SurveillanceTask::loop()
{
  static uint32_t lastUpdateTime = 0;
  static uint32_t lastPublishTime = 0;
  uint32_t currentTime = millis();



  uint32_t u32_Uptime = xTaskGetTickCount() / configTICK_RATE_HZ;
  uint32_t u32_NumberOfTasks = uxTaskGetNumberOfTasks();
  uint32_t u32_FreeHeapTotal = xPortGetFreeHeapSize();
  uint32_t u32_MinimumEverFreeHeap = xPortGetMinimumEverFreeHeapSize();
#ifdef ESP32
  uint32_t u32_FreeHeapInternal = esp_get_free_internal_heap_size();
  uint32_t u32_FreePSRAM = ESP.getFreePsram();
#else
  uint32_t u32_FreeHeapInternal = 0;
  uint32_t u32_FreePSRAM = 0;
#endif

  m_Data.setUptime(u32_Uptime);
  m_Data.setTasks(u32_NumberOfTasks);
  m_Data.setFreeHeapSize(u32_FreeHeapTotal, u32_MinimumEverFreeHeap, u32_FreeHeapInternal, u32_FreePSRAM);

  if(currentTime - lastPublishTime >= 5*1000UL) // Publish every 5 seconds
  { 
    lastPublishTime = currentTime;
    // TODO: publish something usefull
    // uint8_t au8_Data[]={0x01, 0x02, 0x03, 0x04}; // Example data
    // g_controller.getMqtt().publishNodeData(au8_Data, sizeof(au8_Data)); 
  }   

  if (currentTime - lastUpdateTime >= 60*1000UL) // Update every 60 seconds
  {
    lastUpdateTime = currentTime;
    Serial.printf("  Free SurveillanceTask stack: %u/%u (Usage: %u%%)\n",
                  uxTaskGetStackHighWaterMark(nullptr), SURVEILLANCE_TASK_STACK_SIZE,
                  ((SURVEILLANCE_TASK_STACK_SIZE - uxTaskGetStackHighWaterMark(nullptr)) * 100) / SURVEILLANCE_TASK_STACK_SIZE); // nullptr = aktueller Task
  }

  vTaskDelay(pdMS_TO_TICKS(1000UL)); // Alle 1 Sekunde aktualisieren
}

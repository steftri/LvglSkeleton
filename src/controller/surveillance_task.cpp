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
  mp_TaskHandle = xTaskCreateStaticPinnedToCore(
     task,                     // Task function
     "Surveillance",           // Task name
     SURVEILLANCE_TASK_STACK_SIZE, // Stack size
     nullptr,                  // Parameters
     1,                        // Priority
     m_TaskStack,              // Task handle
     &m_TaskBuffer,            // Static task buffer
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
  uint32_t currentTime = millis();

  if (currentTime - lastUpdateTime >= 60*1000UL) // Update every 60 seconds
  {
    lastUpdateTime = currentTime;
    Serial.printf("  Free SurveillanceTask stack: %u/%u (Usage: %u%%)\n",
                  uxTaskGetStackHighWaterMark(nullptr), SURVEILLANCE_TASK_STACK_SIZE,
                  ((SURVEILLANCE_TASK_STACK_SIZE - uxTaskGetStackHighWaterMark(nullptr)) * 100) / SURVEILLANCE_TASK_STACK_SIZE); // nullptr = aktueller Task
  } 

  m_Data.setUptime(xTaskGetTickCount() / configTICK_RATE_HZ); // Uptime in Sekunden
  m_Data.setTasks(uxTaskGetNumberOfTasks()); // Anzahl der Tasks aktualisieren
  m_Data.setFreeHeapSize(xPortGetFreeHeapSize(), xPortGetMinimumEverFreeHeapSize()); // Free heap size aktualisieren

  g_controller.getMqtt().publishNodeData(reinterpret_cast<uint8_t*>(&m_Data), sizeof(SurveillanceData)); 

  vTaskDelay(pdMS_TO_TICKS(1000UL)); // Alle 1 Sekunde aktualisieren
}

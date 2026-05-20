#include <Arduino.h>

#include "wifi_task.h"

WifiTask *WifiTask::mp_thisInstance = nullptr; // Initialize static instance pointer


WifiTask::WifiTask()
  : mp_TaskHandle(nullptr)
{
  mp_thisInstance = this;
}



void WifiTask::begin(void)
{
  Serial.println("Creating WifiTask");

  mp_TaskHandle = xTaskCreateStatic(
     task,                     // Task function
     "Wifi",                   // Task name
     WIFI_TASK_STACK_SIZE,     // Stack size
     nullptr,                  // Parameters
     1,                        // Priority
     m_TaskStack,              // Task handle
     &m_TaskBuffer             // Static task buffer
  );
}



void WifiTask::task(void *pvParameters)
{
  if(mp_thisInstance == nullptr)
  {
    Serial.println("Error: WifiTask instance not set");
    vTaskDelete(nullptr); // Delete the task if instance is not set
    return;
  }

  Serial.println("Wifi Task started");
  mp_thisInstance->setup(); // Call setup once at the beginning

  while (true)
  {
    mp_thisInstance->loop(); // Call loop for periodic updates
  }

}



void WifiTask::setup(void)
{
  Serial.println("WifiTask running.");
}



void WifiTask::loop(void)
{
  Serial.printf("Free Wifi stack: %u/%u (Usage: %u%%)\n",
                uxTaskGetStackHighWaterMark(NULL), WIFI_TASK_STACK_SIZE,
                ((WIFI_TASK_STACK_SIZE - uxTaskGetStackHighWaterMark(NULL)) * 100) / WIFI_TASK_STACK_SIZE); // NULL = aktueller Task

  vTaskDelay(pdMS_TO_TICKS(10000)); // Alle 10 Sekunden aktualisieren  
}

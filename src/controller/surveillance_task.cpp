#include <Arduino.h>

#include "surveillance_task.h"


SurveillanceTask *SurveillanceTask::mp_thisInstance = nullptr; // Initialize static instance pointer


SurveillanceTask::SurveillanceTask()
  : mp_TaskHandle(nullptr)
{
    mp_thisInstance = this;
}


void SurveillanceTask::begin()
{
  Serial.println("Creating SurveillanceTask");
  mp_TaskHandle = xTaskCreateStatic(
     task,                     // Task function
     "Surveillance",           // Task name
     SURVEILLANCE_TASK_STACK_SIZE, // Stack size
     nullptr,                  // Parameters
     1,                        // Priority
     m_TaskStack,              // Task handle
     &m_TaskBuffer             // Static task buffer
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
  // Anzahl der Tasks abrufen
  UBaseType_t taskCount = uxTaskGetNumberOfTasks();
  Serial.printf("Number of tasks: %u\n", taskCount);
  Serial.printf("Free heap size: %u bytes\n", xPortGetFreeHeapSize());
  Serial.printf("Minimum ever free heap size: %u bytes\n", xPortGetMinimumEverFreeHeapSize());  

  Serial.printf("Free Surveillance stack: %u/%u (Usage: %u%%)\n",
                uxTaskGetStackHighWaterMark(NULL), SURVEILLANCE_TASK_STACK_SIZE,
                ((SURVEILLANCE_TASK_STACK_SIZE - uxTaskGetStackHighWaterMark(NULL)) * 100) / SURVEILLANCE_TASK_STACK_SIZE); // NULL = aktueller Task

   vTaskDelay(pdMS_TO_TICKS(10000)); // Alle 10 Sekunden aktualisieren
}

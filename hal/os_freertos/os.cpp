#include <Arduino.h>

#ifdef USE_FREERTOS
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#endif

#include "os.h"


Os::Os()
#ifdef USE_FREERTOS
  : mp_surveillanceTaskHandle(nullptr)
#endif
{
}


#ifdef USE_FREERTOS
void Os::surveillanceTask(void *pvParameters)
{
  const TickType_t monitoringInterval = pdMS_TO_TICKS(5000);

  Serial.println("Surveillance Task started");    
  while (true)
  {
    // Anzahl der Tasks abrufen
    UBaseType_t taskCount = uxTaskGetNumberOfTasks();
    Serial.printf("Number of tasks: %u\n", taskCount);

    vTaskDelay(monitoringInterval);
  }
}
#endif


void Os::setup()
{
#ifdef USE_FREERTOS    
  mp_surveillanceTaskHandle = xTaskCreateStatic(
     surveillanceTask,         // Task function
     "Surveillance",           // Task name
     SURVEILANCE_TASK_STACK_SIZE, // Stack size
     nullptr,                  // Parameters
     1,                        // Priority
     m_surveillanceTaskStack,  // Task handle
     &m_surveillanceTaskBuffer // Static task buffer
  );
#endif  
}


void Os::loop()
{
    // Currently empty, can be extended for periodic updates
}


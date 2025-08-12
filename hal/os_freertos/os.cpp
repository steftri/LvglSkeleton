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
  ledc_timer_config_t ledc_timer = {
  .speed_mode       = LEDC_LOW_SPEED_MODE,
  .timer_num        = LEDC_TIMER_0,
  .duty_resolution  = LEDC_TIMER_13_BIT,
  .freq_hz          = 1000,
  .clk_cfg          = LEDC_AUTO_CLK
  };
  ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

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


void Os::log(const char *pc_Message)
{
    // Log the message to the console or a file
    Serial.println(pc_Message);
}

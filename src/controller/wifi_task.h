#ifndef WIFI_TASK_H
#define WIFI_TASK_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static const size_t WIFI_TASK_STACK_SIZE = 4096; // Stack size for the task


class WifiTask
{
private:
  TaskHandle_t mp_TaskHandle;
  StaticTask_t m_TaskBuffer;
  StackType_t m_TaskStack[ WIFI_TASK_STACK_SIZE ];

  static void task(void *pvParameters);  
  static WifiTask *mp_thisInstance; // Static instance pointer for task access

public:
  WifiTask();

  void begin(void);

private:  
  void setup(void);
  void loop(void);
};



#endif // WIFI_TASK_H
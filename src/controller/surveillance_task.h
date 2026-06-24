#ifndef SURVEILLANCE_TASK_H
#define SURVEILLANCE_TASK_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "surveillance_data.h"


static const size_t SURVEILLANCE_TASK_STACK_SIZE = 4096; // Stack size for the task


class SurveillanceTask
{
private:
  TaskHandle_t mp_TaskHandle;
  StaticTask_t m_TaskBuffer;
  StackType_t m_TaskStack[ SURVEILLANCE_TASK_STACK_SIZE ];

  static void task(void *pvParameters);  
  static SurveillanceTask *mp_thisInstance; // Static instance pointer for task access

  SurveillanceData &m_Data;

public:
  SurveillanceTask(SurveillanceData &data);
  void begin(void);

private:  
  void setup();
  void loop();
};


#endif // SURVEILLANCE_TASK_H
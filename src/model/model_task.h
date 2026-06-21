#ifndef MODEL_TASK_H
#define MODEL_TASK_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>



static const size_t MODEL_TASK_STACK_SIZE = 4096; // Stack size for the task


class ModelTask
{
private:
  TaskHandle_t mp_TaskHandle;
  StaticTask_t m_TaskBuffer;
  StackType_t m_TaskStack[ MODEL_TASK_STACK_SIZE ];

  static void task(void *pvParameters);  
  static ModelTask *mp_thisInstance; // Static instance pointer for task access

public:
  ModelTask();

  void begin(void);

  void save();
  void clear();

private:  
  void setup(void);
  void loop(void);

  // Thread-safe data storage actions
  void actionSave();
  void actionClear();
};


#endif // MODEL_TASK_H  

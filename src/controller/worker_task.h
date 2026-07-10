#ifndef WORKER_TASK_H
#define WORKER_TASK_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "worker_settings.h"
#include "worker_data.h"

#include "lightstripe_hal.h"


static const size_t WORKER_TASK_STACK_SIZE = 4096; // Stack size for the task


class WorkerTask
{
private:
  TaskHandle_t mp_TaskHandle;

  static void task(void *pvParameters);  
  static WorkerTask *mp_thisInstance; // Static instance pointer for task access

  WorkerSettings &m_Settings;
  WorkerData &m_Data;

  LightstripeHal m_LightstripeHal; // Instance of the Lightstripe HAL

public:
  WorkerTask(WorkerSettings &settings, WorkerData &data);
  void begin(void);

private:  
  void setup();
  void loop();
};


#endif // WORKER_TASK_H
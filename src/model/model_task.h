#ifndef MODEL_TASK_H
#define MODEL_TASK_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "data_container.h"
#include "settings_container.h"

#include "data_observer.h"


static const size_t MODEL_TASK_STACK_SIZE = 5*1024; // Stack size for the task


class ModelTask : public DataObserverInterface
{
private:
  TaskHandle_t mp_TaskHandle;
  StaticTask_t m_TaskBuffer;
  StackType_t m_TaskStack[ MODEL_TASK_STACK_SIZE ];

  static void task(void *pvParameters);  
  static ModelTask *mp_thisInstance; // Static instance pointer for task access

  SettingsContainer &m_Settings; // Holds the persistant settings
  DataContainer &m_Data; // Holds the runtime data, e.g., Wi-Fi data

public:
  ModelTask(SettingsContainer &settings, DataContainer &data);

  void begin(void);

  void save();
  void clear();

private:  
  void setup(void);
  void loop(void);

  // Thread-safe data storage actions
  void actionSave();
  void actionClear();

  // DataObserver implementation
  void onDataChanged(Data &r_Data, EDataField e_Field) override;
};


#endif // MODEL_TASK_H  

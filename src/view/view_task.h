#ifndef VIEW_TASK_H
#define VIEW_TASK_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#ifdef ARDUINO
#include "ui_c64.h"
#else
#include "ui.h"
#endif

#include "lv_main.h"

#include "model/data_container.h"
#include "data_observer.h"


static const size_t VIEW_TASK_STACK_SIZE = 8192; // Stack size for the task

static constexpr uint32_t UITASK_NOTIFY_NETWORKS_UPDATED = (1UL << 0); // Notification bit for available networks update



class ViewTask : public DataObserverInterface
{
private:
  TaskHandle_t mp_TaskHandle;
  StaticTask_t m_TaskBuffer;
  StackType_t m_TaskStack[ VIEW_TASK_STACK_SIZE ];

  static void task(void *pvParameters);  
  static ViewTask *mp_thisInstance; // Static instance pointer for task access

#ifdef ARDUINO
  UiC64 m_ui;
#else
  Ui m_ui;
#endif

  LvMain m_LvMain;

public:
  ViewTask();

  void begin();

  LvMain *getLvMain(void);

private:
  void setup();
  void loop();

  // DataObserver implementation
  void onDataChanged(Data &r_Data, EDataField e_Field) override;

  void updateNetworkList();
};



#endif // VIEW_TASK_H
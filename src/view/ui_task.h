#ifndef UI_TASK_H
#define UI_TASK_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#ifdef ARDUINO
#include "ui_c64.h"
#else
#include "ui.h"
#endif

#include "lv_main.h"


static const size_t UI_TASK_STACK_SIZE = 8192; // Stack size for the task



class UiTask
{
private:
  TaskHandle_t mp_TaskHandle;
  StaticTask_t m_TaskBuffer;
  StackType_t m_TaskStack[ UI_TASK_STACK_SIZE ];

  static void task(void *pvParameters);  
  static UiTask *mp_thisInstance; // Static instance pointer for task access

#ifdef ARDUINO
  UiC64 m_ui;
#else
  Ui m_ui;
#endif

  LvMain m_LvMain;

public:
  UiTask();

  void begin();

  LvMain *getLvMain(void);

private:
  void setup();
  void loop();
};



#endif // UI_TASK_H
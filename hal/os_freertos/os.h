#ifndef OS_H
#define OS_H

#ifdef USE_FREERTOS
#include <freertos/FreeRTOS.h>
#endif

#include "interfaces/os_interface.h"


#ifdef USE_FREERTOS
static const size_t SURVEILANCE_TASK_STACK_SIZE = 2048; // Stack size for the task
#endif


class Os : public OsInterface
{
#ifdef USE_FREERTOS
private:
  TaskHandle_t mp_surveillanceTaskHandle;
  StaticTask_t m_surveillanceTaskBuffer;
  StackType_t m_surveillanceTaskStack[ SURVEILANCE_TASK_STACK_SIZE ];
  static void surveillanceTask(void *pvParameters);
#endif    

public:
  Os();

  // Setup function to initialize tasks
  void setup();

  // Loop function for periodic updates
  void loop();

  static void log(const char *pc_Message);  
};

#endif // OS_H

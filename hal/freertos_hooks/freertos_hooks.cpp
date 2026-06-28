/* FreeRTOS application hooks for desktop simulation (Windows + macOS/Linux) */

#include <cstdio>
#include <cstdlib>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>


/* ------------------------------------------------------------------ */
/* Assert handler                                                      */
/* ------------------------------------------------------------------ */
extern "C" void vAssertCalled(unsigned long ulLine, const char * const pcFileName)
{
    fprintf(stderr, "FreeRTOS assert failed: %s:%lu\n", pcFileName, ulLine);
    fflush(stderr);
    abort();
}


/* ------------------------------------------------------------------ */
/* Static allocation hooks (required when configSUPPORT_STATIC_ALLOCATION=1) */
/* ------------------------------------------------------------------ */

/* Idle task */
static StaticTask_t s_IdleTaskTCB;
static StackType_t  s_IdleTaskStack[configMINIMAL_STACK_SIZE];

extern "C" void vApplicationGetIdleTaskMemory(
    StaticTask_t **ppxIdleTaskTCBBuffer,
    StackType_t  **ppxIdleTaskStackBuffer,
    configSTACK_DEPTH_TYPE *puxIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer   = &s_IdleTaskTCB;
    *ppxIdleTaskStackBuffer = s_IdleTaskStack;
    *puxIdleTaskStackSize   = configMINIMAL_STACK_SIZE;
}

/* Timer daemon task */
static StaticTask_t s_TimerTaskTCB;
static StackType_t  s_TimerTaskStack[configTIMER_TASK_STACK_DEPTH];

extern "C" void vApplicationGetTimerTaskMemory(
    StaticTask_t **ppxTimerTaskTCBBuffer,
    StackType_t  **ppxTimerTaskStackBuffer,
    configSTACK_DEPTH_TYPE *puxTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer   = &s_TimerTaskTCB;
    *ppxTimerTaskStackBuffer = s_TimerTaskStack;
    *puxTimerTaskStackSize   = configTIMER_TASK_STACK_DEPTH;
}

#include <Arduino.h>

#include "ui_task.h"

UiTask *UiTask::mp_thisInstance = nullptr; // Initialize static instance pointer


UiTask::UiTask()
  : mp_TaskHandle(nullptr)
{
  mp_thisInstance = this;
}


void UiTask::begin(void)
{
  Serial.println("Creating UiTask");

  mp_TaskHandle = xTaskCreateStaticPinnedToCore(
     task,                     // Task function
     "UI",                     // Task name
     UI_TASK_STACK_SIZE,       // Stack size
     nullptr,                  // Parameters
     1,                        // Priority
     m_TaskStack,              // Task stack
     &m_TaskBuffer,            // Static task buffer
     1                         // Core 1 (APP_CPU, less WiFi interference)
  );
}




void UiTask::task(void *pvParameters)
{
  if(mp_thisInstance == nullptr)
  {
    Serial.println("Error: UiTask instance not set");
    vTaskDelete(nullptr); // Delete the task if instance is not set
    return;
  }

  Serial.println("Ui Task started");
  mp_thisInstance->setup(); // Initialize the UI components before looping

  while (true)
  {
    mp_thisInstance->loop(); // Call loop for periodic updates
  }
}



void UiTask::setup()
{
  Serial.println("UiTask running.");
  lv_init();
  m_ui.setup(); // Initialize the UI components (display hardware + LVGL display)
  m_LvMain.setup(); // Build LVGL widget tree (display must exist first)
}


LvMain *UiTask::getLvMain(void)
{
  return &m_LvMain;
}


void UiTask::loop()
{
  static uint32_t lastUpdateTime = 0;
  uint32_t currentTime = millis();

  m_ui.loop(); // Update the UI components

  if (currentTime - lastUpdateTime >= 10000) // Update every 10 seconds
  {
    lastUpdateTime = currentTime;
    Serial.printf("Free UI stack: %u/%u (Usage: %u%%)\n",
                  uxTaskGetStackHighWaterMark(NULL), UI_TASK_STACK_SIZE,
                  ((UI_TASK_STACK_SIZE - uxTaskGetStackHighWaterMark(NULL)) * 100) / UI_TASK_STACK_SIZE); // NULL = aktueller Task
  }

  vTaskDelay(pdMS_TO_TICKS(5)); // Sleep for 5 ms to prevent busy looping
}

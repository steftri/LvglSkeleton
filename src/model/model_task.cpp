#include <Arduino.h>

#include "controller.h"

#include "model_task.h"


extern Controller g_controller; // Declare the global controller instance defined in main.cpp


ModelTask *ModelTask::mp_thisInstance = nullptr; // Initialize static instance pointer


enum class ENotificationBits : uint32_t
{
  Save = (1UL << 0),
  Clear = (1UL << 1)
};


ModelTask::ModelTask(SettingsContainer &settings, DataContainer &data)
  : mp_TaskHandle(nullptr)
  , m_Settings(settings)
  , m_Data(data)
{
  mp_thisInstance = this;
}




void ModelTask::begin(void)
{
  Serial.println("Creating ModelTask");

  mp_TaskHandle = xTaskCreateStaticPinnedToCore(
     task,                     // Task function
     "Model",                  // Task name
     MODEL_TASK_STACK_SIZE,    // Stack size
     nullptr,                  // Parameters
     1,                        // Priority
     m_TaskStack,              // Task handle
     &m_TaskBuffer,            // Static task buffer
     0                         // Core 0  
     );
}



void ModelTask::save()
{
  xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::Save), eSetBits);
}



void ModelTask::clear()
{
  xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::Clear), eSetBits);
}



void ModelTask::task(void *pvParameters)
{
  if(mp_thisInstance == nullptr)
  {
    Serial.println("Error: ModelTask instance not set");
    vTaskDelete(nullptr); // Delete the task if instance is not set
    return;
  }

  Serial.println("ModelTask started");
  mp_thisInstance->setup(); // Call setup once at the beginning

  while (true)
  {
    mp_thisInstance->loop(); // Call loop for periodic updates
  }

}



void ModelTask::setup(void)
{
  Serial.println("ModelTask running.");

  // The settings are already loaded in the Model's setup, so we can just register as an observer here

   m_Settings.getWifiSettings().registerObserver(this); 
   m_Settings.getMqttSettings().registerObserver(this); 
}



void ModelTask::loop(void)
{
  static uint32_t lastUpdateTime = 0;
  uint32_t currentTime = millis();
  uint32_t u32_NotifiedValue = 0;

  xTaskNotifyWait(0, 0xffff, &u32_NotifiedValue, pdMS_TO_TICKS(1000)); // Wait for notifications with a timeout
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::Save))
  {
    actionSave();
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::Clear))
  {
    actionClear();
  }

  if (currentTime - lastUpdateTime >= 60*1000UL) // Update every 60 seconds
  {
    lastUpdateTime = currentTime;
    Serial.printf("  Free ModelTask stack: %u/%u (Usage: %u%%)\n",
                  uxTaskGetStackHighWaterMark(nullptr), MODEL_TASK_STACK_SIZE,
                  ((MODEL_TASK_STACK_SIZE - uxTaskGetStackHighWaterMark(nullptr)) * 100) / MODEL_TASK_STACK_SIZE); // nullptr = aktueller Task
  }
}


void ModelTask::actionSave()
{
  Serial.println("Saving settings to storage...");
  m_Settings.save(); // Call the save method of the settings to persist them
}

void ModelTask::actionClear()
{
  Serial.println("Clearing settings from storage...");
  m_Settings.clear(); // Call the clear method of the settings to remove them from storage
}


void ModelTask::onDataChanged(Data &r_Data, EDataField e_Field)
{
  Serial.println("ModelTask: Settings changed.");
  xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::Save), eSetBits);
}

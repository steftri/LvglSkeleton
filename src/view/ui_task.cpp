#include <Arduino.h>

#include "controller.h"

#include "ui_task.h"


extern Controller g_controller; // Declare the global controller instance defined in main.cpp


UiTask *UiTask::mp_thisInstance = nullptr; // Initialize static instance pointer


enum class ENotificationBits : uint32_t
{
  AvailableNetworks = (1UL << 0),
  ConnectionState = (1UL << 1),
  IPAddress = (1UL << 2)
};





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

  g_controller.getModel().getData().getWifiData().registerObserver(this); // Register as observer for data changes
  Serial.println("Observer registered for Wi-Fi data changes");


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

  uint32_t u32_NotifiedValue = 0;
  xTaskNotifyWait(0, 0xffff, &u32_NotifiedValue, pdMS_TO_TICKS(5));
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::AvailableNetworks))
  {
    updateNetworkList();
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::ConnectionState))
  {
    m_LvMain.updateWlanSymbol();
    m_LvMain.getTabSettings()->updateWlanStatePanel();
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::IPAddress))
  {
    m_LvMain.getTabSettings()->updateWlanStatePanel();
  }
}



void UiTask::onDataChanged(Data &r_Data, EDataField e_Field)
{
  // Handle data changes and update the UI accordingly
  switch (static_cast<WifiData::EField>(e_Field))
  {
    case WifiData::EField::AvailableNetworks:
      Serial.println("UI task: Available Wi-Fi networks updated");
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::AvailableNetworks), eSetBits);
      break;
    case WifiData::EField::ConnectionState:
      Serial.println("UI task: Wi-Fi connection state changed");
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::ConnectionState), eSetBits);
      break;
    case WifiData::EField::IPAddress:
      Serial.println("UI task: IP address updated");
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::IPAddress), eSetBits);
      break;
    default:
      Serial.println("UI task: Unknown data field changed");
      break;
  }
}


void UiTask::updateNetworkList()
{
  m_LvMain.getTabSettings()->updateWlanSelectList(); // Update the Wi-Fi network list in the UI
}
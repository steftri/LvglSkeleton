#include <Arduino.h>

#include "controller.h"

#include "view_task.h"


extern Controller g_controller; // Declare the global controller instance defined in main.cpp


ViewTask *ViewTask::mp_thisInstance = nullptr; // Initialize static instance pointer


enum class ENotificationBits : uint32_t
{
  AvailableNetworks = (1UL << 0),
  ConnectionState = (1UL << 1),
  IPAddress = (1UL << 2)
};





ViewTask::ViewTask()
  : mp_TaskHandle(nullptr)
{
  mp_thisInstance = this;
}


void ViewTask::begin(void)
{
  Serial.println("Creating ViewTask");

  mp_TaskHandle = xTaskCreateStaticPinnedToCore(
     task,                     // Task function
     "View",                   // Task name
     VIEW_TASK_STACK_SIZE,       // Stack size
     nullptr,                  // Parameters
     1,                        // Priority
     m_TaskStack,              // Task stack
     &m_TaskBuffer,            // Static task buffer
     1                         // Core 1 (APP_CPU, less WiFi interference)
  );
}




void ViewTask::task(void *pvParameters)
{
  if(mp_thisInstance == nullptr)
  {
    Serial.println("Error: ViewTask instance not set");
    vTaskDelete(nullptr); // Delete the task if instance is not set
    return;
  }

  Serial.println("ViewTask started");
  mp_thisInstance->setup(); // Initialize the UI components before looping

  while (true)
  {
    mp_thisInstance->loop(); // Call loop for periodic updates
  }
}



void ViewTask::setup()
{
  Serial.println("ViewTask running.");

  g_controller.getModel().getData().getWifiData().registerObserver(this); // Register as observer for data changes
  Serial.println("Observer registered for Wi-Fi data changes");


  lv_init();
  m_ui.setup(); // Initialize the UI components (display hardware + LVGL display)
  m_LvMain.setup(); // Build LVGL widget tree (display must exist first)
}


LvMain *ViewTask::getLvMain(void)
{
  return &m_LvMain;
}


void ViewTask::loop()
{
  static uint32_t lastUpdateTime = 0;
  uint32_t currentTime = millis();

  m_ui.loop(); // Update the UI components

  if (currentTime - lastUpdateTime >= 10000) // Update every 10 seconds
  {
    lastUpdateTime = currentTime;
    Serial.printf("Free ViewTask stack: %u/%u (Usage: %u%%)\n",
                  uxTaskGetStackHighWaterMark(NULL), VIEW_TASK_STACK_SIZE,
                  ((VIEW_TASK_STACK_SIZE - uxTaskGetStackHighWaterMark(NULL)) * 100) / VIEW_TASK_STACK_SIZE); // NULL = aktueller Task
  }

  uint32_t u32_NotifiedValue = 0;
  xTaskNotifyWait(0, 0xffff, &u32_NotifiedValue, pdMS_TO_TICKS(5));
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::AvailableNetworks))
  {
    updateNetworkList();
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::ConnectionState))
  {
    Serial.println("ViewTask: Connection state changed, updating UI");
    m_LvMain.updateWlanSymbol();
    m_LvMain.getTabSettings()->updateWlanStatePanel();
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::IPAddress))
  {
    m_LvMain.getTabSettings()->updateWlanStatePanel();
  }
}



void ViewTask::onDataChanged(Data &r_Data, EDataField e_Field)
{
  // Handle data changes and update the UI accordingly
  switch (static_cast<WifiData::EField>(e_Field))
  {
    case WifiData::EField::AvailableNetworks:
      Serial.println("ViewTask: Available Wi-Fi networks updated");
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::AvailableNetworks), eSetBits);
      break;
    case WifiData::EField::ConnectionState:
      Serial.println("ViewTask: Wi-Fi connection state changed");
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::ConnectionState), eSetBits);
      break;
    case WifiData::EField::IPAddress:
      Serial.println("ViewTask: IP address updated");
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::IPAddress), eSetBits);
      break;
    default:
      Serial.println("ViewTask: Unknown data field changed");
      break;
  }
}


void ViewTask::updateNetworkList()
{
  m_LvMain.getTabSettings()->updateWlanSelectList(); // Update the Wi-Fi network list in the UI
}

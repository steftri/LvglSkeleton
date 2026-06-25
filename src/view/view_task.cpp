#include <Arduino.h>

#include "controller.h"
#include "lv_main.h"

#include "view_task.h"

extern Controller g_controller;

LvMain g_ViewLvMain; // Global instance of the LVGL main view class to be used in the ViewTask

ViewTask *ViewTask::mp_thisInstance = nullptr; // Initialize static instance pointer


enum class ENotificationBits : uint32_t
{
  AvailableNetworks = (1UL << 0),
  EnableState = (1UL << 1),
  WifiConnectionState = (1UL << 2),
  IPAddress = (1UL << 3),
  SurveillanceStats = (1UL << 4),
  LVGLStats = (1UL << 5),
  MQTTConnectionState = (1UL << 6),
  MQTTStats = (1UL << 7)
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
  DataContainer &r_DataContainer = g_controller.getModel().getData();

  Serial.println("ViewTask running.");

  lv_init();  
  m_ui.setup(); // Initialize the UI components (display hardware + LVGL display)
  g_ViewLvMain.setup(); // Build LVGL widget tree (display must exist first)
  Serial.println("LVGL UI setup complete");

  r_DataContainer.getWifiData().registerObserver(this); 
  r_DataContainer.getMqttData().registerObserver(this); 
  r_DataContainer.getSurveillanceData().registerObserver(this); 
  Serial.println("Observer registered for data changes");
}




void ViewTask::loop()
{
  static uint32_t lastUpdateTime = 0;
  uint32_t currentTime = millis();

  m_ui.loop(); // Update the UI components

  if (currentTime - lastUpdateTime >= 60*1000UL) // Update every 60 seconds
  {
    lastUpdateTime = currentTime;
    Serial.printf("  Free ViewTask stack: %u/%u (Usage: %u%%)\n",
                  uxTaskGetStackHighWaterMark(nullptr), VIEW_TASK_STACK_SIZE,
                  ((VIEW_TASK_STACK_SIZE - uxTaskGetStackHighWaterMark(nullptr)) * 100) / VIEW_TASK_STACK_SIZE); // nullptr = aktueller Task
  }

  uint32_t u32_NotifiedValue = 0;
  xTaskNotifyWait(0, 0xffff, &u32_NotifiedValue, pdMS_TO_TICKS(5));
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::AvailableNetworks))
  {
    onUpdateSettingsNetworkList();
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::EnableState))
  {
    onUpdateEnableState();
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::WifiConnectionState))
  {
    onUpdateWIFIConnectionState();
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::IPAddress))
  {
    onUpdateSettingsIPAddress();
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::SurveillanceStats))
  {
    onUpdateInfoSurveillanceStats();
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::LVGLStats))
  {
    onUpdateInfoLVGLStats();
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::MQTTConnectionState))
  {
    onUpdateInfoMQTTConnectionState();
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::MQTTStats))
  {
    onUpdateInfoMQTTStats();
  }
}



void ViewTask::onDataChanged(Data &r_Data, EDataField e_Field)
{
  DataContainer &r_DataContainer = g_controller.getModel().getData();

  if(&r_Data == &r_DataContainer.getWifiData())
  {
    onWIFIDataChanged(e_Field);
  }
  else if(&r_Data == &r_DataContainer.getMqttData())
  {
    onMQTTDataChanged(e_Field);
  }
  else if(&r_Data == &r_DataContainer.getSurveillanceData())
  {
    onSurveillanceDataChanged(e_Field);
  }
  else
  {
    Serial.println("ViewTask: Unknown data source changed");
  }
}


void ViewTask::onWIFIDataChanged(EDataField e_Field)
{
  switch (static_cast<WifiData::EField>(e_Field))
  {
    case WifiData::EField::AvailableNetworks:
      Serial.println("ViewTask: Available Wi-Fi networks updated");
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::AvailableNetworks), eSetBits);
      break;
    case WifiData::EField::EnableState:
      Serial.println("ViewTask: Wi-Fi enable state changed");
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::EnableState), eSetBits);
      break;
    case WifiData::EField::ConnectionState:
      Serial.println("ViewTask: Wi-Fi connection state changed");
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::WifiConnectionState), eSetBits);
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


void ViewTask::onMQTTDataChanged(EDataField e_Field)
{
  switch (static_cast<MqttData::EField>(e_Field))
  {
    case MqttData::EField::ConnectionState:
      Serial.println("ViewTask: MQTT connection state changed");
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::MQTTConnectionState), eSetBits);
      break;
    case MqttData::EField::MessageCount:
      Serial.println("ViewTask: MQTT message count updated");
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::MQTTStats), eSetBits);
      break;  
    default:
      Serial.println("ViewTask: Unknown MQTT data field changed");
      break;
  }
}


void ViewTask::onSurveillanceDataChanged(EDataField e_Field)
{
  switch (static_cast<SurveillanceData::EField>(e_Field))
  {
    case SurveillanceData::EField::Uptime:
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::SurveillanceStats), eSetBits);
      break;
    case SurveillanceData::EField::Tasks:
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::SurveillanceStats), eSetBits);
      break;
    case SurveillanceData::EField::FreeHeapSize:
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::SurveillanceStats), eSetBits);
      break;
    default:
      Serial.println("ViewTask: Unknown surveillance data field changed");
      break;
  }
}




void ViewTask::onUpdateSettingsNetworkList()
{
  g_ViewLvMain.getTabSettings()->updateWlanSelectList(); // Update the Wi-Fi network list in the UI
}



void ViewTask::onUpdateEnableState()
{
  g_ViewLvMain.getTabSettings()->updateWlanStatePanel(); 
}


void ViewTask::onUpdateWIFIConnectionState()
{
  auto &WifiData = g_controller.getModel().getData().getWifiData();
  bool b_IsConnected = (WifiData.getState() == WifiData::EState::Connected);
  g_ViewLvMain.setWlanSymbol(b_IsConnected); // Update the Wi-Fi symbol in the UI
  g_ViewLvMain.getTabSettings()->updateWlanStatePanel(); // Update the Wi-Fi state panel in the settings tab
}


void ViewTask::onUpdateSettingsIPAddress()
{
  g_ViewLvMain.getTabSettings()->updateWlanStatePanel(); // Update the IP address display in the Wi-Fi state panel
}


void ViewTask::onUpdateInfoSurveillanceStats()
{
  g_ViewLvMain.getTabInfo()->updateFreeRTOSInfo(); // Update the Surveillance stats in the info tab
}


void ViewTask::onUpdateInfoLVGLStats()
{
  g_ViewLvMain.getTabInfo()->updateLVGLInfo(); // Update the LVGL stats in the info tab
}


void ViewTask::onUpdateInfoMQTTConnectionState()
{
  // TODO
}


void ViewTask::onUpdateInfoMQTTStats()
{
  g_ViewLvMain.getTabInfo()->updateMQTTInfo(); // Update the MQTT stats in the info tab
}


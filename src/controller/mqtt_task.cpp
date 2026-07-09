#include <Arduino.h>

#include "mqtt_task.h"

#include "controller.h"

extern Controller g_controller; // Declare the external Controller instance


MqttTask *MqttTask::mp_thisInstance = nullptr; // Initialize static instance pointer


enum class ENotificationBits : uint32_t
{
  ChangedBrokerSettings = (1UL << 0),
  ChangedSparkplugBSettings = (1UL << 1),
  ChangedWifiConnectionState = (1UL << 2)
};



MqttTask::MqttTask(SystemSettings &systemSettings, MqttSettings &mqttSettings, MqttData &mqttData, WifiData &wifiData)
  : mp_TaskHandle(nullptr)
  , m_MqttHal(*this) // Pass the MqttActionInterface reference to the MqttHal
  , m_SystemSettings(systemSettings) // Initialize the reference to the system settings
  , m_MqttSettings(mqttSettings) // Initialize the reference to the MQTT settings
  , m_MqttData(mqttData) // Initialize the reference to the MQTT data
  , m_WifiData(wifiData) // Initialize the reference to the Wi-Fi data
{
  mp_thisInstance = this;
}



void MqttTask::begin()
{
  Serial.println("Creating MqttTask");

  mp_TaskHandle = xTaskCreateStaticPinnedToCore(
     task,                     // Task function
     "Mqtt",                   // Task name
     MQTT_TASK_STACK_SIZE,     // Stack size
     nullptr,                  // Parameters
     1,                        // Priority
     m_TaskStack,              // Task handle
     &m_TaskBuffer,            // Static task buffer
     0                         // Core 0 
  );
}





void MqttTask::task(void *pvParameters)
{
  if(mp_thisInstance == nullptr)
  {
    Serial.println("Error: MqttTask instance not set");
    vTaskDelete(nullptr); // Delete the task if instance is not set
    return;
  }

  Serial.println("Mqtt Task started");
  mp_thisInstance->setup(); // Call setup once at the beginning

  while (true)
  {
    mp_thisInstance->loop(); // Call loop for periodic updates
  }

}



void MqttTask::setup(void)
{
  Serial.println("MqttTask running.");

  m_MqttHal.setup();

  // Register this MqttTask as an observer for settings changes
  m_SystemSettings.registerObserver(this); 
  m_MqttSettings.registerObserver(this); 
  m_WifiData.registerObserver(this); 
  Serial.println("Observer registered for data changes");  
}



void MqttTask::loop(void)
{
  static uint32_t lastUpdateTime = 0;
  uint32_t currentTime = millis();
  uint32_t u32_NotifiedValue = 0;

  xTaskNotifyWait(0, 0xffff, &u32_NotifiedValue, pdMS_TO_TICKS(100)); // Wait for notifications with a timeout
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::ChangedSparkplugBSettings))
  {
    actionChangeSparkplugBSettings();
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::ChangedBrokerSettings) )
  {
    actionChangeBrokerSettings();
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::ChangedWifiConnectionState))
  {
    actionChangedWifiConnectionState();
  }

  m_MqttHal.poll();  // handling of keepalive messages

  if (currentTime - lastUpdateTime >= 60*1000UL) // Update every 60 seconds
  {
    lastUpdateTime = currentTime;
    Serial.printf("  Free MqttTask stack: %u/%u (Usage: %u%%)\n",
                  uxTaskGetStackHighWaterMark(nullptr), MQTT_TASK_STACK_SIZE,
                  ((MQTT_TASK_STACK_SIZE - uxTaskGetStackHighWaterMark(nullptr)) * 100) / MQTT_TASK_STACK_SIZE); 
  }
}




void MqttTask::onDataChanged(Data &r_Data, EDataField e_Field)
{
  if(&r_Data == &m_SystemSettings)
  {
    onSystemSettingsChanged(e_Field);
  }
  else if(&r_Data == &m_MqttSettings)
  {
    onMqttSettingsChanged(e_Field);
  }
  else if(&r_Data == &m_WifiData)
  {
    onWifiDataChanged(e_Field);
  }
  else
  {
    Serial.println("ViewTask: Unknown data source changed");
  }
}


void MqttTask::onSystemSettingsChanged(EDataField e_Field)
{
  switch (static_cast<SystemSettings::EField>(e_Field))
  {
    case SystemSettings::EField::HostName:
      Serial.println("MqttTask: Host name (Node ID) updated");
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::ChangedSparkplugBSettings), eSetBits);
      break;
    default:
      Serial.println("MqttTask: Unknown data field changed");
      break;
  }
}


void MqttTask::onMqttSettingsChanged(EDataField e_Field)
{
  switch (static_cast<MqttSettings::EField>(e_Field))
  {
    case MqttSettings::EField::BrokerAddress:
      Serial.println("MqttTask: MQTT broker settings updated");
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::ChangedBrokerSettings), eSetBits);
      break;
    case MqttSettings::EField::GroupId:
      Serial.println("MqttTask: MQTT Group ID updated");
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::ChangedSparkplugBSettings), eSetBits);
      break;  
    default:
      Serial.println("MqttTask: Unknown data field changed");
      break;
  }
}



void MqttTask::onWifiDataChanged(EDataField e_Field)
{
  switch (static_cast<WifiData::EField>(e_Field))
  {
    case WifiData::EField::ConnectionState:
      Serial.println("MqttTask: Wi-Fi connection state changed");
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::ChangedWifiConnectionState), eSetBits);
      break;
    default:
      Serial.println("MqttTask: Unknown data field changed");
      break;
  }
}





void MqttTask::connect()
{
  const char *pc_BrokerAddr = m_MqttSettings.getBrokerAddr();
  uint16_t u16_BrokerPort = m_MqttSettings.getBrokerPort();

  char ac_DbgConnectMsg[80];
  snprintf(ac_DbgConnectMsg, 80, "Trying to connect to %s:%i", pc_BrokerAddr, u16_BrokerPort);
  Serial.println(ac_DbgConnectMsg);

  m_MqttData.setState(MqttData::EState::Connecting); // Update the MQTT connection state in the data

  // specify last will message for the broker
  char ac_lastWillTopic[80];
  uint8_t au8_lastWillMessage[80];

  snprintf(ac_lastWillTopic, sizeof(ac_lastWillTopic), "spBv1.0/%s/NDEATH/%s", m_MqttSettings.getGroupId(), m_SystemSettings.getHostName());
  snprintf((char*)au8_lastWillMessage, sizeof(au8_lastWillMessage), "I am dead, Jim.");
  m_MqttHal.setLastWill(ac_lastWillTopic, au8_lastWillMessage, strlen((char*)au8_lastWillMessage), 0, false); 

  if(m_MqttHal.connect(pc_BrokerAddr, u16_BrokerPort) == MqttHal::ERc::Ok)
  {
    m_MqttData.setState(MqttData::EState::Connected);
  }
  else
  {
    m_MqttData.setState(MqttData::EState::Error);
    Serial.println("MQTT connection failed");
    g_controller.getView().showMessageBox("MQTT Connection Failed", "Failed to connect to the MQTT broker. Please check the broker address and port.");
  }
}



void MqttTask::disconnect()
{
  Serial.println("Disconnecting from MQTT broker");
  m_MqttHal.disconnect(); 
}



void MqttTask::actionChangeBrokerSettings()
{
  Serial.println("MqttTask: Broker settings changed, reconnecting...");
  disconnect();
  connect();
}



void MqttTask::actionChangeSparkplugBSettings()
{
  Serial.printf("Sparkplug-B settings changed. Group ID: %s, Node ID: %s\n", m_MqttSettings.getGroupId(), m_SystemSettings.getHostName());
  disconnect();
  connect();
}  



void MqttTask::actionChangedWifiConnectionState()
{
  if(m_WifiData.getState() == WifiData::EState::Connected)
  {
    Serial.println("Wi-Fi connected, attempting to connect to MQTT broker...");
    connect();
  }
  else
  {
    Serial.println("Wi-Fi disconnected, disconnecting from MQTT broker...");
    disconnect();
  }
}





void MqttTask::onConnected()
{
  char ac_SubscribeTopic[80];

  m_MqttData.setState(MqttData::EState::Connected); // Update the MQTT connection state in the data
  Serial.println("Connected to MQTT broker");

  // subscribe to the common Sparkplug-B toppics
  snprintf(ac_SubscribeTopic, sizeof(ac_SubscribeTopic), "spBv1.0/%s/NBIRTH/+", m_MqttSettings.getGroupId());
  m_MqttHal.subscribe(ac_SubscribeTopic); // Subscribe to all Sparkplug-B node topics for the group "ERNI"

  snprintf(ac_SubscribeTopic, sizeof(ac_SubscribeTopic), "spBv1.0/%s/NDEATH/+", m_MqttSettings.getGroupId());
  m_MqttHal.subscribe(ac_SubscribeTopic); // Subscribe to all Sparkplug-B node death topics for the group "ERNI"  

  snprintf(ac_SubscribeTopic, sizeof(ac_SubscribeTopic), "spBv1.0/%s/NCMD/%s", m_MqttSettings.getGroupId(), m_SystemSettings.getHostName());
  m_MqttHal.subscribe(ac_SubscribeTopic); // Subscribe to all Sparkplug-B command topics for the group "ERNI"

  // send Spartkplug-B birth message
  char ac_birthTopic[80];
  uint8_t au8_birthMessage[80];

  snprintf(ac_birthTopic, sizeof(ac_birthTopic), "spBv1.0/%s/NBIRTH/%s", m_MqttSettings.getGroupId(), m_SystemSettings.getHostName());
  snprintf((char*)au8_birthMessage, sizeof(au8_birthMessage), "Hello from ESP32!");

  m_MqttHal.publish(ac_birthTopic, au8_birthMessage, strlen((char*)au8_birthMessage), 0, false); // Publish a test message to verify connection

}



void MqttTask::onDisconnected()
{
  m_MqttData.setState(MqttData::EState::Disconnected); // Update the MQTT connection state in the data
  Serial.println("Disconnected from MQTT broker");
}



void MqttTask::onConnectionFailed(EConnectionError error)
{
  Serial.printf("MQTT connection failed with error: %d\n", static_cast<int>(error));
  m_MqttData.setState(MqttData::EState::Error); // Update the MQTT connection state in the data

  g_controller.getView().showMessageBox("MQTT Connection Failed", "Failed to connect to the MQTT broker. Please check the broker address and port.");
}


void MqttTask::onMessageReceived(const char *topic, const char *message)
{
  Serial.printf("MQTT message received on topic \"%s\": %s\n", topic, message);
  m_MqttData.incrementReceivedMessageCount(); // Increment the received message count in the data
}

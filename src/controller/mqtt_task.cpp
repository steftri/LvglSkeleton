#include <Arduino.h>

#include "mqtt_task.h"


static const char *MQTT_GROUP_ID = "ERNI"; // Group ID for Sparkplug-B messages
static const char *MQTT_NODE_ID = "LvglSkeleton"; // Node ID for Sparkplug-B messages - shall be the same as the hostname for WIFI


MqttTask *MqttTask::mp_thisInstance = nullptr; // Initialize static instance pointer


enum class ENotificationBits : uint32_t
{
  Connect = (1UL << 0),
  Disconnect = (1UL << 1),
  ChangeNodeId = (1UL << 2)
};



MqttTask::MqttTask(MqttSettings &mqttSettings, MqttData &mqttData)
  : mp_TaskHandle(nullptr)
  , m_MqttHal(*this) // Pass the MqttActionInterface reference to the MqttHal
  , m_MqttSettings(mqttSettings) // Initialize the reference to the MQTT settings
  , m_MqttData(mqttData) // Initialize the reference to the MQTT data
{
  mp_thisInstance = this;
}



void MqttTask::begin(const char *pc_NodeId)
{
  strncpy(mac_NodeId, pc_NodeId, MAX_NODEID_LENGTH);
  mac_NodeId[MAX_NODEID_LENGTH] = '\0'; // Ensure null-termination

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




void MqttTask::connect()
{
  xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::Connect), eSetBits);
}


void MqttTask::disconnect()
{
  xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::Disconnect), eSetBits);
}


void MqttTask::setGroupId(const char *pc_GroupId)
{
  // TODO: store the group ID in the settings 
  xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::ChangeNodeId), eSetBits);
}


void MqttTask::setNodeId(const char *pc_NodeId)
{
  strncpy(mac_NodeId, pc_NodeId, MAX_NODEID_LENGTH);
  mac_NodeId[MAX_NODEID_LENGTH] = '\0'; // Ensure null-termination
  xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::ChangeNodeId), eSetBits);
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
}



void MqttTask::loop(void)
{
  static uint32_t lastUpdateTime = 0;
  uint32_t currentTime = millis();
  uint32_t u32_NotifiedValue = 0;

  xTaskNotifyWait(0, 0xffff, &u32_NotifiedValue, pdMS_TO_TICKS(100)); // Wait for notifications with a timeout
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::Connect))
  {
    actionConnect();
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::Disconnect))
  {
    actionDisconnect();
  }

  m_MqttHal.poll();  // handling of keepalive messages

  if (currentTime - lastUpdateTime >= 60*1000UL) // Update every 60 seconds
  {
    lastUpdateTime = currentTime;
    Serial.printf("  Free MqttTask stack: %u/%u (Usage: %u%%)\n",
                  uxTaskGetStackHighWaterMark(nullptr), MQTT_TASK_STACK_SIZE,
                  ((MQTT_TASK_STACK_SIZE - uxTaskGetStackHighWaterMark(nullptr)) * 100) / MQTT_TASK_STACK_SIZE); // nullptr = aktueller Task
  }
}



void MqttTask::actionConnect()
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

  snprintf(ac_lastWillTopic, sizeof(ac_lastWillTopic), "spBv1.0/%s/NDEATH/%s", MQTT_GROUP_ID, MQTT_NODE_ID);
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
  }
}

void MqttTask::actionDisconnect()
{
  Serial.println("Disconnecting from MQTT broker");
  m_MqttHal.disconnect(); // Disconnect from the MQTT broker using the HAL
}




void MqttTask::actionChangeNodeId()
{
  Serial.printf("Sparkplug-B Node ID changed to: %s\n", mac_NodeId);

  // TODO: Handle the change in Node ID, e.g., by reconnecting to the MQTT broker with the new ID
}  



void MqttTask::onConnected()
{
  char ac_SubscribeTopic[80];

  m_MqttData.setState(MqttData::EState::Connected); // Update the MQTT connection state in the data
  Serial.println("Connected to MQTT broker");

  // subscribe to the common Sparkplug-B toppics
  snprintf(ac_SubscribeTopic, sizeof(ac_SubscribeTopic), "spBv1.0/%s/NBIRTH/+", MQTT_GROUP_ID);
  m_MqttHal.subscribe(ac_SubscribeTopic); // Subscribe to all Sparkplug-B node topics for the group "ERNI"

  snprintf(ac_SubscribeTopic, sizeof(ac_SubscribeTopic), "spBv1.0/%s/NDEATH/+", MQTT_GROUP_ID);
  m_MqttHal.subscribe(ac_SubscribeTopic); // Subscribe to all Sparkplug-B node death topics for the group "ERNI"  

  snprintf(ac_SubscribeTopic, sizeof(ac_SubscribeTopic), "spBv1.0/%s/NCMD/%s", MQTT_GROUP_ID, MQTT_NODE_ID);
  m_MqttHal.subscribe(ac_SubscribeTopic); // Subscribe to all Sparkplug-B command topics for the group "ERNI"

  // send Spartkplug-B birth message
  char ac_birthTopic[80];
  uint8_t au8_birthMessage[80];

  snprintf(ac_birthTopic, sizeof(ac_birthTopic), "spBv1.0/%s/NBIRTH/%s", MQTT_GROUP_ID, MQTT_NODE_ID);
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

  // TODO: Notify the view to update the MQTT connection status and show an error message if needed
}


void MqttTask::onMessageReceived(const char *topic, const char *message)
{
  Serial.printf("MQTT message received on topic \"%s\": %s\n", topic, message);
  m_MqttData.incrementReceivedMessageCount(); // Increment the received message count in the data
}

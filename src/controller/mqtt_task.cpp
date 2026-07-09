#include <Arduino.h>

#include "mqtt_task.h"

#include "controller.h"

extern Controller g_controller; // Declare the external Controller instance


static const size_t MAX_NODE_ID_LENGTH = SystemSettings::MAX_HOSTNAME_LENGTH;
static const size_t MAX_DEVICE_ID_LENGTH = 64;

static const size_t MAX_MQTT_BIRTH_TOPIC_LENGTH = 8 + MqttSettings::MAX_GROUP_ID_LENGTH + 8 + MAX_NODE_ID_LENGTH;
static const size_t MAX_MQTT_BIRTH_MESSAGE_LENGTH = 80;
static const size_t MAX_MQTT_LAST_WILL_TOPIC_LENGTH = 8 + MqttSettings::MAX_GROUP_ID_LENGTH + 8 + MAX_NODE_ID_LENGTH;
static const size_t MAX_MQTT_LAST_WILL_MESSAGE_LENGTH = 80;

static const size_t MAX_MQTT_TOPIC_LENGTH = 8 + MqttSettings::MAX_GROUP_ID_LENGTH + 1 + 7 + 1 + MAX_NODE_ID_LENGTH + 1 + MAX_DEVICE_ID_LENGTH;
static const size_t MAX_MQTT_SUBSCRIBE_TOPIC_LENGTH = 8 + MqttSettings::MAX_GROUP_ID_LENGTH + 8 + 1 + 1;



MqttTask *MqttTask::mp_thisInstance = nullptr; // Initialize static instance pointer
StaticQueue_t MqttTask::m_MqttStaticQueue;

enum class ENotificationBits : uint32_t
{
  ChangedBrokerSettings = (1UL << 0),
  ChangedSparkplugBSettings = (1UL << 1),
  ChangedWifiConnectionState = (1UL << 2),
  ChangedWifiIPAddress = (1UL << 3)
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

  m_MqttQueueHandle = xQueueCreateStatic(
    MQTT_TASK_MESSAGE_QUEUE_SIZE,
    sizeof(SMqttMessage),
    reinterpret_cast<uint8_t*>(ma_MqttMessageQueueStorage),
    &m_MqttStaticQueue
  );

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
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::ChangedWifiIPAddress))
  {
    actionChangedWifiIPAddress();
  }

  
  // Check if there are any messages in the MQTT message queue
  if(uxQueueMessagesWaiting(m_MqttQueueHandle) > 0)
  {
    SMqttMessage MqttMessage;
    if (xQueueReceive(m_MqttQueueHandle, &MqttMessage, 0) == pdPASS)
    {
      actionPublishNodeData(MqttMessage.au8_MessageBuffer, MqttMessage.MessageSize, MqttMessage.u8_QoS, MqttMessage.b_Retain);
    }
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





void MqttTask::publishNodeData(uint8_t *pu8_MessageBuffer, const size_t MessageSize, uint8_t u8_QoS, bool b_Retain)
{
  if(MessageSize > MAX_MQTT_MESSAGE_SIZE)
  {
    Serial.printf("Error: Message size %zu exceeds maximum allowed size of %zu bytes\n", MessageSize, MAX_MQTT_MESSAGE_SIZE);
    return;
  }

  SMqttMessage MqttMessage;
  memcpy(MqttMessage.au8_MessageBuffer, pu8_MessageBuffer, MessageSize);
  MqttMessage.MessageSize = MessageSize;
  MqttMessage.u8_QoS = u8_QoS;
  MqttMessage.b_Retain = b_Retain;

  if (xQueueSend(m_MqttQueueHandle, &MqttMessage, pdMS_TO_TICKS(100)) != pdPASS)
  {
    Serial.println("MqttTask: Failed to enqueue message for publishing");
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
    Serial.println("MqttTask: Unknown data source changed");
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
      Serial.println("MqttTask: Unknown System settings field changed");
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
      Serial.println("MqttTask: Unknown MQTT settings field changed");
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
    case WifiData::EField::IPAddress:
      Serial.println("MqttTask: Wi-Fi IP address changed");
      xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::ChangedWifiIPAddress), eSetBits);
      break;
    default:
      Serial.println("MqttTask: Unknown Wi-Fi data field changed");
      break;
  }
}





void MqttTask::connect()
{
  if(m_MqttData.getState() == MqttData::EState::Connected)
  {
    return; // Already connected, no need to connect again
  }

  m_MqttData.setState(MqttData::EState::Connecting); 

  // specify last will message for the broker
  char ac_lastWillTopic[MAX_MQTT_LAST_WILL_TOPIC_LENGTH + 1];
  uint8_t au8_lastWillMessage[MAX_MQTT_LAST_WILL_MESSAGE_LENGTH + 1];

  snprintf(ac_lastWillTopic, sizeof(ac_lastWillTopic), "spBv1.0/%s/NDEATH/%s", m_MqttSettings.getGroupId(), m_SystemSettings.getHostName());
  snprintf((char*)au8_lastWillMessage, sizeof(au8_lastWillMessage), "I am dead, Jim.");
  m_MqttHal.setLastWill(ac_lastWillTopic, au8_lastWillMessage, strlen((char*)au8_lastWillMessage), 0, false); 

  m_MqttHal.connect(m_MqttSettings.getBrokerAddr(), m_MqttSettings.getBrokerPort());
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
    // wi-Fi is connected, but we need the IP adress to connect to the broker, 
    // so we wait for the IP address notification
  }
  else
  {
    Serial.println("Wi-Fi disconnected, disconnecting from MQTT broker...");
    disconnect();
  }
}



void MqttTask::actionChangedWifiIPAddress()
{
  Serial.println("Wi-Fi IP address changed, connecting to MQTT broker...");
  connect();  
}




void MqttTask::actionPublishNodeData(uint8_t *pu8_MessageBuffer, const size_t MessageSize, uint8_t u8_QoS, bool b_Retain)
{
  char ac_Topic[MAX_MQTT_TOPIC_LENGTH + 1];

  snprintf(ac_Topic, sizeof(ac_Topic), "spBv1.0/%s/NDATA/%s", m_MqttSettings.getGroupId(), m_SystemSettings.getHostName());
  m_MqttHal.publish(ac_Topic, pu8_MessageBuffer, MessageSize, u8_QoS, b_Retain);

  m_MqttData.incrementSentMessageCount();
}



void MqttTask::onConnected()
{
  char ac_SubscribeTopic[MAX_MQTT_SUBSCRIBE_TOPIC_LENGTH + 1];
  char ac_birthTopic[MAX_MQTT_BIRTH_TOPIC_LENGTH + 1];
  uint8_t au8_birthMessage[MAX_MQTT_BIRTH_MESSAGE_LENGTH + 1];

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
  snprintf(ac_birthTopic, sizeof(ac_birthTopic), "spBv1.0/%s/NBIRTH/%s", m_MqttSettings.getGroupId(), m_SystemSettings.getHostName());
  snprintf((char*)au8_birthMessage, sizeof(au8_birthMessage), "Hello from ESP32!");

  m_MqttHal.publish(ac_birthTopic, au8_birthMessage, strlen((char*)au8_birthMessage), 0, false); // Publish a test message to verify connection
}



void MqttTask::onDisconnected()
{
  Serial.println("Disconnected from MQTT broker");
  m_MqttData.setState(MqttData::EState::Disconnected); // Update the MQTT connection state in the data
}



void MqttTask::onConnectionFailed(int32_t s32_Error)
{
  const char *pc_ErrorMessage = MqttHal::RcToString(static_cast<MqttHal::ERc>(s32_Error));
  Serial.printf("MQTT connection failed with error code: %d, message: %s\n", s32_Error, pc_ErrorMessage);
  m_MqttData.setState(MqttData::EState::Error, s32_Error, pc_ErrorMessage);
}



void MqttTask::onMessageReceived(const char *pc_Topic, const uint8_t *pu8_MessageBuffer, const size_t MessageSize)
{
  char ac_TopicCopy[MAX_MQTT_TOPIC_LENGTH + 1] = {0};

  //char *pc_GroupId = nullptr;
  char *pc_MessageType = nullptr;
  char *pc_NodeID = nullptr;
  char *pc_DeviceID = nullptr;
  //char *pc_Extra = nullptr;

  Mqtt::EMessageType e_MessageType = Mqtt::EMessageType::NBIRTH; // safe default; overwritten below

  if(pc_Topic == nullptr || pu8_MessageBuffer == nullptr)
  {
    return;
  }

  m_MqttData.incrementReceivedMessageCount();

  Serial.printf("Topic: \"%s\" Content: \"%.*s\"\n", pc_Topic, static_cast<int>(MessageSize), pu8_MessageBuffer);

  // we need some buffer to work with strtok, so we copy the topic into a local buffer
  strncpy(ac_TopicCopy, pc_Topic, sizeof(ac_TopicCopy) - 1);
  ac_TopicCopy[sizeof(ac_TopicCopy) - 1] = '\0';

  const char *pc_Prefix = "spBv1.0/";
  const size_t PrefixLen = strlen(pc_Prefix);  
  if(strncmp(ac_TopicCopy, pc_Prefix, PrefixLen) == 0)
  {
    char *pc_Rest = ac_TopicCopy + PrefixLen;
    /*pc_GroupId = */strtok(pc_Rest, "/");
    pc_MessageType = strtok(nullptr, "/");
    pc_NodeID = strtok(nullptr, "/");
    pc_DeviceID = strtok(nullptr, "/");
    //pc_Extra = strtok(nullptr, "/");

    if(strcmp(pc_MessageType, "NBIRTH") == 0) 
      e_MessageType = Mqtt::EMessageType::NBIRTH;
    else if(strcmp(pc_MessageType, "NDEATH") == 0)
      e_MessageType = Mqtt::EMessageType::NDEATH;
    else if(strcmp(pc_MessageType, "NDATA") == 0)
      e_MessageType = Mqtt::EMessageType::NDATA;
    else if(strcmp(pc_MessageType, "NCMD") == 0)
      e_MessageType = Mqtt::EMessageType::NCMD;
    else if(strcmp(pc_MessageType, "DBIRTH") == 0)
      e_MessageType = Mqtt::EMessageType::DBIRTH;
    else if(strcmp(pc_MessageType, "DDEATH") == 0)
      e_MessageType = Mqtt::EMessageType::DDEATH;
    else if(strcmp(pc_MessageType, "DDATA") == 0)
      e_MessageType = Mqtt::EMessageType::DDATA;
    else if(strcmp(pc_MessageType, "DCMD") == 0)
      e_MessageType = Mqtt::EMessageType::DCMD;
    else
    {
      Serial.printf("Invalid/unsupported Sparkplug-B topic: %s\n", pc_Topic);
      return; 
    }
  }
  else
  {
    Serial.printf("MqttTask: Non-Sparkplug topic received, ignoring: %s\n", pc_Topic);
    return;
  }

  switch(e_MessageType)
  {
    case Mqtt::EMessageType::NBIRTH:
      Serial.printf("NBIRTH message from node \"%s\" received with size %d\n", pc_NodeID, static_cast<int>(MessageSize));
      break;
    case Mqtt::EMessageType::NDEATH:
      Serial.printf("NDEATH message from node \"%s\" received with size %d\n", pc_NodeID, static_cast<int>(MessageSize));
      break;
    case Mqtt::EMessageType::NDATA:
      Serial.printf("NDATA message from node \"%s\" received with size %d\n", pc_NodeID, static_cast<int>(MessageSize));
      break;
    case Mqtt::EMessageType::NCMD:
      Serial.printf("NCMD message for node \"%s\" received with size %d\n", pc_NodeID, static_cast<int>(MessageSize));
      break;
    case Mqtt::EMessageType::DBIRTH:
      Serial.printf("DBIRTH message from device \"%s/%s\" received with size %d\n", pc_NodeID, pc_DeviceID, static_cast<int>(MessageSize));
      break;  
    case Mqtt::EMessageType::DDEATH:
      Serial.printf("DDEATH message from device \"%s/%s\" received with size %d\n", pc_NodeID, pc_DeviceID, static_cast<int>(MessageSize));
      break;  
    case Mqtt::EMessageType::DDATA:
      Serial.printf("DDATA message from device \"%s/%s\" received with size %d\n", pc_NodeID, pc_DeviceID, static_cast<int>(MessageSize));
      break;
    case Mqtt::EMessageType::DCMD:
      Serial.printf("DCMD message for device \"%s/%s\" received with size %d\n", pc_NodeID, pc_DeviceID, static_cast<int>(MessageSize));
      break;
    default:
      Serial.printf("Unknown MQTT message type received; NodeID: %s, DeviceID: %s, Size: %d\n", pc_NodeID, pc_DeviceID, static_cast<int>(MessageSize));
      break;
  }
}

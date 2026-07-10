#include <Arduino.h>

#include "mqtt_hal.h"


WiFiClient MqttHal::m_MqttTcpClient;
MqttClient MqttHal::m_MqttClient(&MqttHal::m_MqttTcpClient); 


MqttHal *MqttHal::mp_thisInstance = nullptr; // Initialize static instance pointer


MqttHal::MqttHal(MqttActionInterface &actionListener)
  : m_actionListener(actionListener) 
{
  mp_thisInstance = this; // Set the static instance pointer to this instance
  mac_ClientId[0] = '\0';
}


void MqttHal::setup()
{
  Serial.println("MqttHal setup called");

  strncpy(mac_ClientId, WiFi.macAddress().c_str(), sizeof(mac_ClientId) - 1);
  mac_ClientId[sizeof(mac_ClientId) - 1] = '\0'; // Ensure null termination

  m_MqttClient.setId(mac_ClientId); // Set the MQTT client ID
  m_MqttClient.setConnectionTimeout(3 * 1000);
  m_MqttClient.onMessage(MqttHal::onMessage); // Set the callback for incoming messages
}


MqttHal::ERc MqttHal::connect(const char *pc_BrokerAddress, uint16_t u16_BrokerPort)
{
  ERc rc = ERc::Ok;

  Serial.printf("MqttHal: Connecting to broker %s:%u\n", pc_BrokerAddress, u16_BrokerPort);
  if (m_MqttClient.connect(pc_BrokerAddress, u16_BrokerPort))
  {
    m_actionListener.onConnected();
  }
  else
  {
    rc = mqttErrorToERc(m_MqttClient.connectError());
    m_actionListener.onConnectionFailed(static_cast<int32_t>(rc)); 
  }
  return rc;
} 


void MqttHal::disconnect()
{
  Serial.println("MqttHal: Disconnecting from MQTT broker");

  m_MqttClient.stop();
  m_actionListener.onDisconnected();
}



void MqttHal::setLastWill(const char *pc_Topic, const uint8_t *pu8_MessageBuffer, const size_t MessageSize, uint8_t u8_QoS, bool b_Retain)
{
  Serial.printf("MqttHal: Setting last will for topic %s\n", pc_Topic);
  m_MqttClient.beginWill(pc_Topic, u8_QoS, b_Retain);
  m_MqttClient.write(pu8_MessageBuffer, MessageSize);
  m_MqttClient.endWill();
}



void MqttHal::publish(const char *pc_Topic, const uint8_t *pu8_MessageBuffer, const size_t MessageSize, uint8_t u8_QoS, bool b_Retain)
{
  Serial.printf("MqttHal: Publishing message to topic %s\n", pc_Topic);
  m_MqttClient.beginMessage(pc_Topic, MessageSize, u8_QoS, b_Retain);
  m_MqttClient.write(pu8_MessageBuffer, MessageSize);
  m_MqttClient.endMessage();
}



void MqttHal::subscribe(const char *pc_Topic)
{
  Serial.printf("MqttHal: Subscribing to topic %s\n", pc_Topic);
  m_MqttClient.subscribe(pc_Topic);
}


void MqttHal::poll()
{
  m_MqttClient.poll(); 
}



MqttHal::ERc MqttHal::mqttErrorToERc(int errorCode)
{
  switch(errorCode)
  {
    case MQTT_SUCCESS:
      return ERc::Ok;
    case MQTT_CONNECTION_TIMEOUT:
      return ERc::ConnectionTimeout;
    case MQTT_CONNECTION_REFUSED:
      return ERc::ConnectionRefused;
    case MQTT_UNACCEPTABLE_PROTOCOL_VERSION:
      return ERc::UnacceptableProtocolVersion;
    case MQTT_IDENTIFIER_REJECTED:
      return ERc::IdentifierRejected;
    case MQTT_SERVER_UNAVAILABLE:
      return ERc::ServerUnavailable;
    case MQTT_BAD_USER_NAME_OR_PASSWORD:
      return ERc::BadUserNameOrPassword;
    case MQTT_NOT_AUTHORIZED:
      return ERc::NotAuthorized;
    default:
      return ERc::UnknownError;
  }
}





void MqttHal::onMessage(int messageSize)
{
  char ac_MessageBuffer[MAX_MESSAGE_SIZE + 1]; // Buffer to hold the incoming message
  size_t bytesRead = 0;

  if (mp_thisInstance == nullptr) 
  {
    Serial.println("Error: MqttHal instance not set");
    return;
  }

  String topic = m_MqttClient.messageTopic();

  while (m_MqttClient.available()) 
  {
    char c = m_MqttClient.read();
    if(bytesRead < MAX_MESSAGE_SIZE) 
    {
      ac_MessageBuffer[bytesRead++] = c;
    }
  }
  ac_MessageBuffer[bytesRead] = '\0'; // Null-terminate the string
  
  Serial.printf("MqttHal: Message received on topic %s\n", topic.c_str());
  mp_thisInstance->m_actionListener.onMessageReceived(topic.c_str(), reinterpret_cast<const uint8_t *>(ac_MessageBuffer), bytesRead);
}





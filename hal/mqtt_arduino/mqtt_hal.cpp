#include <Arduino.h>

#include "mqtt_hal.h"


WiFiClient MqttHal::m_MqttTcpClient;
MqttClient MqttHal::m_MqttClient(&MqttHal::m_MqttTcpClient); 


MqttHal *MqttHal::mp_thisInstance = nullptr; // Initialize static instance pointer


MqttHal::MqttHal(MqttActionInterface &actionListener)
  : m_actionListener(actionListener) 
{
  mp_thisInstance = this; // Set the static instance pointer to this instance

  strncpy(mac_ClientId, WiFi.macAddress().c_str(), sizeof(mac_ClientId) - 1);
  mac_ClientId[sizeof(mac_ClientId) - 1] = '\0'; // Ensure null termination
}


void MqttHal::setup()
{
  Serial.println("MqttHal setup called");
  m_MqttClient.setId(mac_ClientId); // Set the MQTT client ID
  m_MqttClient.onMessage(MqttHal::onMessage); // Set the callback for incoming messages
}


MqttHal::ERc MqttHal::connect(const char *pc_BrokerAddress, uint16_t u16_BrokerPort)
{
  int error = 0;

  if(m_MqttClient.connected())
  {
    return ERc::Ok; // Already connected
  }

  Serial.printf("MqttHal: Connecting to broker %s:%u\n", pc_BrokerAddress, u16_BrokerPort);
  if (m_MqttClient.connect(pc_BrokerAddress, u16_BrokerPort))
  {
    m_actionListener.onConnected();
  }
  else
  {
    error = m_MqttClient.connectError();
    // TODO: Handle different error codes and notify the action listener accordingly
    m_actionListener.onConnectionFailed(MqttActionInterface::EConnectionError::UnknownError); 
  }
  return mqttErrorToERc(error);
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
  if (mp_thisInstance == nullptr) 
  {
    Serial.println("Error: MqttHal instance not set");
    return;
  }

  String topic = m_MqttClient.messageTopic();
  String message = m_MqttClient.readString();

  Serial.printf("MqttHal: Message received on topic %s: %s\n", topic.c_str(), message.c_str());
  mp_thisInstance->m_actionListener.onMessageReceived(topic.c_str(), message.c_str());
}





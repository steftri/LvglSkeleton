#ifndef MQTT_HAL_H
#define MQTT_HAL_H

#include <WiFi.h>
#include <ArduinoMqttClient.h>

#include "interfaces/mqtt_hal_interface.h"
#include "interfaces/mqtt_action_interface.h"

class MqttHal : public MqttHalInterface
{
  static const uint8_t MAX_IDENTIFIER_LENGTH = 64; // Maximum length for the client identifier

  static WiFiClient m_MqttTcpClient;
  static MqttClient m_MqttClient;

public:
  MqttHal(MqttActionInterface &actionListener);
  ~MqttHal() = default;

  void setup() override;

  ERc connect(const char *pc_BrokerAddress, uint16_t u16_BrokerPort) override;
  void disconnect() override;

  void setLastWill(const char *pc_Topic, const uint8_t *pu8_MessageBuffer, const size_t MessageSize, uint8_t u8_QoS, bool b_Retain) override;
  void publish(const char *pc_Topic, const uint8_t *pu8_MessageBuffer, const size_t MessageSize, uint8_t u8_QoS, bool b_Retain) override;
  void subscribe(const char *pc_Topic) override;

  void poll() override;

private:
  static MqttHal *mp_thisInstance; // Static instance pointer for task access
  MqttActionInterface &m_actionListener; // Reference to the action listener
  char mac_ClientId[MAX_IDENTIFIER_LENGTH+1]; // Client identifier

  ERc mqttErrorToERc(int errorCode); // Convert MQTT error code to ERc

  static void onMessage(int messageSize); // Callback for incoming messages
};

#endif  // MQTT_HAL_H
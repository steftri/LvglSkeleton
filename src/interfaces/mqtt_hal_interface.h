#ifndef MQTT_HAL_INTERFACE_H
#define MQTT_HAL_INTERFACE_H

#include <inttypes.h>
#include <stddef.h>

class MqttHalInterface
{
public:
  virtual ~MqttHalInterface() = default;

  virtual void setup() = 0;

  enum class ERc : int32_t
  {
    Ok = 0,
    Error = -1,
    ConnectionRefused = -2,
    ConnectionTimeout = -3,
    UnacceptableProtocolVersion = -4,
    IdentifierRejected = -5,
    ServerUnavailable = -6,
    BadUserNameOrPassword = -7,
    NotAuthorized = -8,
    UnknownError = -9
  };

  virtual ERc connect(const char *pc_BrokerAddress, uint16_t u16_BrokerPort) = 0;
  virtual void disconnect() = 0;

  virtual void setLastWill(const char *pc_Topic, const uint8_t *pu8_MessageBuffer, const size_t MessageSize, uint8_t u8_QoS, bool b_Retain) = 0;
  virtual void publish(const char *pc_Topic, const uint8_t *pu8_MessageBuffer, const size_t MessageSize, uint8_t u8_QoS, bool b_Retain) = 0;
  virtual void subscribe(const char *pc_Topic) = 0;

  virtual void poll() = 0;
};


#endif  // MQTT_HAL_INTERFACE_H

#ifndef MQTT_DATA_H
#define MQTT_DATA_H

#include <inttypes.h>
#include <stddef.h>

#include "data.h"


class MqttData : public Data
{
public:
  enum class EField : uint8_t 
  { 
    ConnectionState = 0,
    MessageCount
  };

  enum class EState
  {
    Disconnected = 0,
    Connecting,
    Connected,
    Error
  };

private:


  EState me_State; ///< Current state of the MQTT connection
  uint32_t mu32_SentMessageCount; ///< Number of messages sent
  uint32_t mu32_ReceivedMessageCount; ///< Number of messages received

public:
  MqttData();
  ~MqttData() = default;

  void setState(EState e_State);
  EState getState(void) const;

  void incrementSentMessageCount();
  void incrementReceivedMessageCount();
  uint32_t getSentMessageCount() const;
  uint32_t getReceivedMessageCount() const;
};

#endif // MQTT_DATA_H
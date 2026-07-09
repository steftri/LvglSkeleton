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
    LastError,
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

  int32_t ms32_LastErrorCode; ///< Last error code encountered
  char mac_LastErrorMessage[64]; ///< Last error message encountered

  uint32_t mu32_SentMessageCount; ///< Number of messages sent
  uint32_t mu32_ReceivedMessageCount; ///< Number of messages received

public:
  MqttData();
  ~MqttData() = default;

  void setState(EState e_State, int32_t s32_ErrorCode = 0, const char *pc_ErrorMessage = nullptr);
  EState getState(void) const;
  int32_t getLastErrorCode(void) const;
  const char *getLastErrorMessage(void) const;

  void incrementSentMessageCount();
  void incrementReceivedMessageCount();
  uint32_t getSentMessageCount() const;
  uint32_t getReceivedMessageCount() const;
};

#endif // MQTT_DATA_H
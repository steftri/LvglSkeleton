/**
 * @file mqtt_settings.h
 * @brief This file contains the definition of the MqttSettings class and related constants.
 *
 * @author Stefan Trippler, ERNI (Deutschland) GmbH
 */

#ifndef MQTT_SETTINGS_H
#define MQTT_SETTINGS_H

#include <inttypes.h>

#include "data.h"

/**
 * @class MqttSettings
 * @brief A class to manage MQTT broker settings.
 */
class MqttSettings : public Data
{
  static const uint8_t MAX_BROKER_ADDR_LENGTH = 64; ///< Maximum length of the broker address.
  static const uint16_t DEFAULT_BROKER_PORT = 1883; ///< Default port number for the MQTT broker.

  static const uint8_t MAX_GROUP_ID_LENGTH = 32; ///< Maximum length of the Sparkplug-B Group ID

  char mac_BrokerAddr[MAX_BROKER_ADDR_LENGTH+1]; ///< Broker address.
  uint16_t mu16_BrokerPort; ///< Broker port.

  char mac_GroupId[MAX_GROUP_ID_LENGTH+1]; ///< Group ID.

public:
  enum class EField : uint8_t 
  { 
    ConnectState = 0,
    BrokerAddress = 1,
    GroupId = 2
  };

  static const uint16_t MQTT_SETTINGS_SIZE = MAX_BROKER_ADDR_LENGTH + sizeof(mu16_BrokerPort) + MAX_GROUP_ID_LENGTH; ///< Size of the MQTT settings in bytes.

  MqttSettings(void);

  void init(void);
  void setBrokerAddr(const char *pc_Addr);
  const char *getBrokerAddr(void);
  void setBrokerPort(uint16_t u16_Port);
  uint16_t getBrokerPort(void);
  void setGroupId(const char *pc_GroupId);
  const char *getGroupId(void);

  uint16_t serialize(uint8_t *pu8_Buffer, const uint16_t u16_BufferSize);
  void unserialize(const uint8_t *pu8_Buffer, const uint16_t u16_Size);
};

#endif

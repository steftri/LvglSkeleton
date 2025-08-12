/**
 * @file mqtt_settings.h
 * @brief This file contains the definition of the MqttSettings class and related constants.
 *
 * @author Stefan Trippler, ERNI (Deutschland) GmbH
 */

#ifndef MQTT_SETTINGS_H
#define MQTT_SETTINGS_H

#include <inttypes.h>

#define MAX_BROKER_ADDR_LENGTH   64  ///< Maximum length of the broker address.
#define DEFAULT_BROKER_PORT    1883  ///< Default port number for the MQTT broker.

#define MQTT_SETTINGS_SIZE        (MAX_BROKER_ADDR_LENGTH+2)  ///< Size of the MQTT settings.

/**
 * @class MqttSettings
 * @brief A class to manage MQTT broker settings.
 */
class MqttSettings
{
  char mac_BrokerAddr[MAX_BROKER_ADDR_LENGTH+1]; ///< Broker address.
  uint16_t mu16_BrokerPort; ///< Broker port.

public:
  MqttSettings(void);

  void init(void);
  void setBroker(const char *pc_Addr, const uint16_t u16_Port = DEFAULT_BROKER_PORT);
  const char *getBrokerAddr(void);
  uint16_t getBrokerPort(void);

  uint16_t serialize(uint8_t *pu8_Buffer, const uint16_t u16_BufferSize);
  void unserialize(const uint8_t *pu8_Buffer, const uint16_t u16_Size);
};

#endif

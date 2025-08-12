
/**
 * @file mqtt_settings.cpp
 * @brief Implementation of the MqttSettings class for managing MQTT settings.
 * 
 * This file contains the implementation of the MqttSettings class, which provides
 * methods to initialize, set, get, serialize, and unserialize MQTT broker settings.
 * 
 * @author Stefan Trippler, ERNI (Deutschland) GmbH
 */

#include <memory.h>
#include "mqtt_settings.h"

/**
 * @brief Constructor for MqttSettings class.
 * Initializes the MQTT settings.
 */
MqttSettings::MqttSettings(void)
{
  init();
}

/**
 * @brief Initializes the MQTT settings with default values.
 */
void MqttSettings::init(void)
{
  mu16_BrokerPort = DEFAULT_BROKER_PORT;
  mac_BrokerAddr[0] = '\0';
}

/**
 * @brief Sets the MQTT broker address and port.
 * 
 * @param pc_Addr The broker address as a C-string.
 * @param u16_Port The broker port as a 16-bit unsigned integer.
 */
void MqttSettings::setBroker(const char *pc_Addr, const uint16_t u16_Port)
{
  strncpy(mac_BrokerAddr, pc_Addr, MAX_BROKER_ADDR_LENGTH);
  mac_BrokerAddr[MAX_BROKER_ADDR_LENGTH] = '\0';
  mu16_BrokerPort = u16_Port;
}

/**
 * @brief Gets the MQTT broker address.
 * 
 * @return The broker address as a C-string.
 */
const char *MqttSettings::getBrokerAddr(void)
{
  return mac_BrokerAddr;
}

/**
 * @brief Gets the MQTT broker port.
 * 
 * @return The broker port as a 16-bit unsigned integer.
 */
uint16_t MqttSettings::getBrokerPort(void)
{
  return mu16_BrokerPort;
}

/**
 * @brief Serializes the MQTT settings into a buffer.
 * 
 * @param pu8_Buffer The buffer to serialize the settings into.
 * @param u16_BufferSize The size of the buffer.
 * @return The number of bytes written to the buffer.
 */
uint16_t MqttSettings::serialize(uint8_t *pu8_Buffer, const uint16_t u16_BufferSize)
{
  uint16_t u16_BufferPos = 0;

  if((!pu8_Buffer) || (u16_BufferSize<MQTT_SETTINGS_SIZE))
    return 0;

  for(auto i=0; i<MAX_BROKER_ADDR_LENGTH; i++)
      pu8_Buffer[u16_BufferPos++] = mac_BrokerAddr[i];
  pu8_Buffer[u16_BufferPos++] = static_cast<uint8_t>(mu16_BrokerPort>>8);
  pu8_Buffer[u16_BufferPos++] = static_cast<uint8_t>(mu16_BrokerPort); 

  return u16_BufferPos;
}

/**
 * @brief Unserializes the MQTT settings from a buffer.
 * 
 * @param pu8_Buffer The buffer containing the serialized settings.
 * @param u16_Size The size of the buffer.
 */
void MqttSettings::unserialize(const uint8_t *pu8_Buffer, const uint16_t u16_Size)
{
  uint16_t u16_BufferPos = 0;

  if((!pu8_Buffer) || (u16_Size<MQTT_SETTINGS_SIZE))
    return;

  for(auto i=0; i<MAX_BROKER_ADDR_LENGTH; i++)
    mac_BrokerAddr[i] = pu8_Buffer[u16_BufferPos++];
  mac_BrokerAddr[MAX_BROKER_ADDR_LENGTH]='\0';   

  mu16_BrokerPort  = static_cast<uint16_t>(pu8_Buffer[u16_BufferPos++])<<8;
  mu16_BrokerPort |= static_cast<uint16_t>(pu8_Buffer[u16_BufferPos++]);
}

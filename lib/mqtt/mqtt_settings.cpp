
/**
 * @file mqtt_settings.cpp
 * @brief Implementation of the MqttSettings class for managing MQTT settings.
 * 
 * This file contains the implementation of the MqttSettings class, which provides
 * methods to initialize, set, get, serialize, and unserialize MQTT broker settings.
 * 
 * @author Stefan Trippler, ERNI (Deutschland) GmbH
 */

#include <string.h>
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
 * @brief Sets the MQTT broker address.
 * 
 * @param pc_Addr The broker address as a C-string.
 */
void MqttSettings::setBrokerAddr(const char *pc_Addr)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  if(strncmp(mac_BrokerAddr, pc_Addr, MAX_BROKER_ADDR_LENGTH) != 0)
  {
    strncpy(mac_BrokerAddr, pc_Addr, MAX_BROKER_ADDR_LENGTH);
    mac_BrokerAddr[MAX_BROKER_ADDR_LENGTH] = '\0'; // Ensure null-termination

    notifyObservers(static_cast<EDataField>(EField::BrokerAddress));
  }
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
 * @brief Sets the MQTT broker port.
 * 
 * @param u16_Port The broker port as a 16-bit unsigned integer.
 */
void MqttSettings::setBrokerPort(uint16_t u16_Port)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  if(mu16_BrokerPort != u16_Port)
  {
    mu16_BrokerPort = u16_Port;
    notifyObservers(static_cast<EDataField>(EField::BrokerAddress));
  }
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
 * @brief Sets the MQTT Sparkplug-B Group ID.
 * 
 * @param pc_GroupId The Group ID as a C-string.
 */
void MqttSettings::setGroupId(const char *pc_GroupId)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  if(strncmp(mac_GroupId, pc_GroupId, MAX_GROUP_ID_LENGTH) != 0)
  {
    strncpy(mac_GroupId, pc_GroupId, MAX_GROUP_ID_LENGTH);
    mac_GroupId[MAX_GROUP_ID_LENGTH] = '\0'; // Ensure null-termination

    notifyObservers(static_cast<EDataField>(EField::GroupId));
  }
}

/**
 * @brief Gets the MQTT Sparkplug-B Group ID
 * 
 * @return The group ID address as a C-string.
 */
const char *MqttSettings::getGroupId(void)
{
  return mac_GroupId;
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

  std::lock_guard<std::mutex> lock(m_DataMutex);

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

  std::lock_guard<std::mutex> lock(m_DataMutex);    

  for(auto i=0; i<MAX_BROKER_ADDR_LENGTH; i++)
    mac_BrokerAddr[i] = pu8_Buffer[u16_BufferPos++];
  mac_BrokerAddr[MAX_BROKER_ADDR_LENGTH]='\0';   

  mu16_BrokerPort  = static_cast<uint16_t>(pu8_Buffer[u16_BufferPos++])<<8;
  mu16_BrokerPort |= static_cast<uint16_t>(pu8_Buffer[u16_BufferPos++]);
}

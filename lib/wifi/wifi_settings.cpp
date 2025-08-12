/**
 * @file wifi_settings.cpp
 * @brief Implementation of the WifiSettings class for managing WiFi network settings.
 * 
 * This file contains the implementation of the WifiSettings class, which provides
 * methods for initializing, setting, and retrieving WiFi network credentials, as well
 * as serializing and unserializing these settings.
 * 
 * The class maintains a list of WiFi networks, each identified by an SSID and a password.
 * It allows adding new networks, updating existing ones, and retrieving the stored
 * network credentials. The settings can be serialized into a buffer for storage and
 * unserialized back into the class from a buffer.
 *
 * @author Stefan Trippler, ERNI (Deutschland) GmbH
 */
#include <memory.h>

#include "wifi_settings.h"


/**
 * @brief Constructor for the WifiSettings class.
 * 
 * This constructor initializes the WifiSettings object by calling the init() method.
 */
WifiSettings::WifiSettings(void)
{
  init();
}


/**
 * @brief Initializes the WifiSettings object.
 * 
 * This function sets the initial state of the WifiSettings object by 
 * resetting the count of WiFi networks to zero and clearing the SSID 
 * and password for each network in the list.
 */
void WifiSettings::init(void)
{
  mu8_WifiNetworkCount = 0;
  for(uint8_t u8_Index = 0; u8_Index<MAX_WIFI_NETWORKS; u8_Index++)
  {
    ma_Networks[u8_Index].ac_SSID[0] = '\0';
    ma_Networks[u8_Index].ac_Password[0] = '\0';
  }
}



/**
 * @brief Sets the network credentials for a WiFi network.
 * 
 * This function sets the SSID and password for a WiFi network. If the network is already known,
 * it moves it to the front of the list (index 0) and updates its password with the new one.
 * If the network is not known, it adds it to the list with the highest priority.
 * 
 * @param pc_SSID The SSID of the WiFi network.
 * @param pc_Password The password of the WiFi network.
 */
void WifiSettings::setNetwork(const char *pc_SSID, const char *pc_Password)
{
  // if the network is already known, move it to the front of the list (index 0) and replace its passwort with the new one 
  for(uint8_t u8_Index = 0; u8_Index<mu8_WifiNetworkCount; u8_Index++)
  {
    if(0 == strncmp(pc_SSID,  ma_Networks[u8_Index].ac_SSID, MAX_SSID_LENGTH))
    { 
      for(uint8_t i = u8_Index; i>0; i--)
      {  
        // i is the new position of the entry
        strncpy(ma_Networks[i].ac_SSID, ma_Networks[i-1].ac_SSID, MAX_SSID_LENGTH);
        ma_Networks[i].ac_SSID[MAX_SSID_LENGTH] = '\0';
        strncpy(ma_Networks[i].ac_Password, ma_Networks[i-1].ac_Password, MAX_WPA2_PASSWORD_LENGTH);
        ma_Networks[i].ac_Password[MAX_WPA2_PASSWORD_LENGTH] = '\0';
      }
      strncpy(ma_Networks[0].ac_SSID, pc_SSID, MAX_SSID_LENGTH);
      ma_Networks[0].ac_SSID[MAX_SSID_LENGTH] = '\0';
      strncpy(ma_Networks[0].ac_Password, pc_Password, MAX_WPA2_PASSWORD_LENGTH);
      ma_Networks[0].ac_Password[MAX_WPA2_PASSWORD_LENGTH] = '\0';
      return;
    }
  }

  // if there already are other known networks, move them down. The new network gets the highest priority.
  if(mu8_WifiNetworkCount>0)
  {
    for(uint8_t i = (mu8_WifiNetworkCount<MAX_WIFI_NETWORKS)?mu8_WifiNetworkCount:(MAX_WIFI_NETWORKS-1); i>0; i--)
    {  
      // i is the new position of the entry
      strncpy(ma_Networks[i].ac_SSID, ma_Networks[i-1].ac_SSID, MAX_SSID_LENGTH);
      ma_Networks[i].ac_SSID[MAX_SSID_LENGTH] = '\0';
      strncpy(ma_Networks[i].ac_Password, ma_Networks[i-1].ac_Password, MAX_WPA2_PASSWORD_LENGTH);
      ma_Networks[i].ac_Password[MAX_WPA2_PASSWORD_LENGTH] = '\0';
    }  
  }
  strncpy(ma_Networks[0].ac_SSID, pc_SSID, MAX_SSID_LENGTH);
  ma_Networks[0].ac_SSID[MAX_SSID_LENGTH] = '\0';
  strncpy(ma_Networks[0].ac_Password, pc_Password, MAX_WPA2_PASSWORD_LENGTH);
  ma_Networks[0].ac_Password[MAX_WPA2_PASSWORD_LENGTH] = '\0';

  if(mu8_WifiNetworkCount<MAX_WIFI_NETWORKS)
    mu8_WifiNetworkCount++;
}


/**
 * @brief Get the number of Wi-Fi networks.
 * 
 * This function returns the count of Wi-Fi networks that have been detected
 * and stored in the WifiSettings object.
 * 
 * @return uint8_t The number of Wi-Fi networks.
 */
uint8_t WifiSettings::getNetworkCount(void)
{
  return mu8_WifiNetworkCount;
}


/**
 * @brief Retrieves the SSID of a WiFi network at a specified index.
 * 
 * @param u8_Index The index of the WiFi network.
 * @return const char* The SSID of the WiFi network. Returns an empty string if the index is out of bounds.
 */
const char *WifiSettings::getNetworkSSID(const uint8_t u8_Index)
{
  if((u8_Index>=MAX_WIFI_NETWORKS) || (u8_Index>=mu8_WifiNetworkCount))
    return "";
  return ma_Networks[u8_Index].ac_SSID;
}



/**
 * @brief Retrieves the password for a specified Wi-Fi network.
 * 
 * @param u8_Index The index of the Wi-Fi network whose password is to be retrieved.
 * @return const char* The password of the specified Wi-Fi network. 
 *                     Returns an empty string if the index is out of bounds.
 */
const char *WifiSettings::getNetworkPassword(const uint8_t u8_Index)
{
  if((u8_Index>=MAX_WIFI_NETWORKS) || (u8_Index>=mu8_WifiNetworkCount))
    return "";
  return ma_Networks[u8_Index].ac_Password;
}


/**
 * @brief Serializes the WiFi settings into a buffer.
 *
 * This function serializes the WiFi settings, including the network count,
 * SSIDs, and passwords, into the provided buffer.
 *
 * @param pu8_Buffer Pointer to the buffer where the serialized data will be stored.
 * @param u16_BufferSize Size of the provided buffer.
 * @return The number of bytes written to the buffer, or 0 if the buffer is null
 *         or the buffer size is less than the required size.
 */
uint16_t WifiSettings::serialize(uint8_t *pu8_Buffer, const uint16_t u16_BufferSize)
{
  uint16_t u16_BufferPos = 0;

  if((!pu8_Buffer) || (u16_BufferSize<WIFI_SETTINGS_SIZE))
    return 0;

  pu8_Buffer[u16_BufferPos++] = mu8_WifiNetworkCount; 

  for(uint8_t u8_Index = 0; u8_Index<MAX_WIFI_NETWORKS; u8_Index++)
  {
    for(auto i=0; i<MAX_SSID_LENGTH; i++)
      pu8_Buffer[u16_BufferPos++] = ma_Networks[u8_Index].ac_SSID[i];
    for(auto i=0; i<MAX_WPA2_PASSWORD_LENGTH; i++)
      pu8_Buffer[u16_BufferPos++] = ma_Networks[u8_Index].ac_Password[i]; 
  }

  return u16_BufferPos;
}


/**
 * @brief Unserializes the WiFi settings from a given buffer.
 *
 * This function reads the WiFi settings from a buffer and populates the
 * internal data structures with the deserialized values. It expects the
 * buffer to contain the WiFi network count followed by the SSID and password
 * for each network.
 *
 * @param pu8_Buffer Pointer to the buffer containing the serialized WiFi settings.
 * @param u16_Size Size of the buffer in bytes.
 *
 * @note The buffer must be at least WIFI_SETTINGS_SIZE bytes long.
 *       The function will return immediately if the buffer is null or
 *       smaller than WIFI_SETTINGS_SIZE.
 */
void WifiSettings::unserialize(const uint8_t *pu8_Buffer, const uint16_t u16_Size)
{
  uint16_t u16_BufferPos = 0;

  if((!pu8_Buffer) || (u16_Size<WIFI_SETTINGS_SIZE))
    return;

  mu8_WifiNetworkCount = pu8_Buffer[u16_BufferPos++];

  for(uint8_t u8_Index=0; u8_Index<MAX_WIFI_NETWORKS; u8_Index++)
  {
    for(auto i=0; i<MAX_SSID_LENGTH; i++)
      ma_Networks[u8_Index].ac_SSID[i] = pu8_Buffer[u16_BufferPos++];
    ma_Networks[u8_Index].ac_SSID[MAX_SSID_LENGTH]=0;   

    for(auto i=0; i<MAX_WPA2_PASSWORD_LENGTH; i++)
      ma_Networks[u8_Index].ac_Password[i] = pu8_Buffer[u16_BufferPos++];
    ma_Networks[u8_Index].ac_Password[MAX_WPA2_PASSWORD_LENGTH]=0;
  }
}

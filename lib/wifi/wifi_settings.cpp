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
#include <string.h>

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
  std::lock_guard<std::mutex> lock(m_DataMutex);

  mb_EnableState = true; // Wi-Fi station mode is initially enabled
  mu8_NetworkCount = 0;

  for(uint8_t u8_Index = 0; u8_Index<MAX_WIFI_NETWORKS; u8_Index++)
  {
    ma_Networks[u8_Index].ac_SSID[0] = '\0';
    ma_Networks[u8_Index].ac_Password[0] = '\0';
  }
}


/**
 * @brief Sets the Wi-Fi enable state.
 * 
 * This function sets the Wi-Fi enable state. If the state changes,
 * it notifies the observers about the change.
 * 
 * @param b_Enable The new enable state.
 */
void WifiSettings::setEnable(bool b_Enable)
{
  bool b_Changed = false;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    
    if(mb_EnableState != b_Enable)
    {
      mb_EnableState = b_Enable;
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::EnableState));
  }
}


/**
 * @brief Retrieves the Wi-Fi enable state.
 * 
 * This function retrieves the current enable state of the Wi-Fi station mode.
 * 
 * @return The current enable state.
 */
bool WifiSettings::getEnable(void) const
{
  return mb_EnableState;
}


/**
 * @brief Sets the Wi-Fi connect state.
 * 
 * This function sets the Wi-Fi connect state. If the state changes,
 * it notifies the observers about the change.
 * 
 * @param b_Enabled The new connect state.
 */
void WifiSettings::setConnect(bool b_Enabled)
{
  bool b_Changed = false;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);

    if(mb_ConnectState != b_Enabled)
    {
      mb_ConnectState = b_Enabled;
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::ConnectState));
  }
}




/**
 * @brief Retrieves the Wi-Fi connect state.
 * 
 * This function retrieves the current connect state of the Wi-Fi station mode.
 * 
 * @return The current connect state.
 */
bool WifiSettings::getConnect(void) const
{
  return mb_ConnectState;
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
  bool b_DataChanged = false;

  if((pc_SSID == nullptr) || (pc_Password == nullptr) || (strlen(pc_SSID) == 0))
    return;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);

    // if the network is already at index 0 with same password, nothing changed
    if((mu8_NetworkCount > 0)
      && (strncmp(pc_SSID, ma_Networks[0].ac_SSID, MAX_SSID_LENGTH) == 0)
      && (strncmp(pc_Password, ma_Networks[0].ac_Password, MAX_WPA2_PASSWORD_LENGTH) == 0))
    {
      return;
    }

    // if the network is already known, move it to the front of the list (index 0) and replace its passwort with the new one 
    for(uint8_t u8_Index = 0; u8_Index<mu8_NetworkCount; u8_Index++)
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

        b_DataChanged = true;
        break;
      }
    }

    // if there already are other known networks, move them down. The new network gets the highest priority.
    if(!b_DataChanged)
    {
      if(mu8_NetworkCount>0)
      {
        for(uint8_t i = (mu8_NetworkCount<MAX_WIFI_NETWORKS)?mu8_NetworkCount:(MAX_WIFI_NETWORKS-1); i>0; i--)
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

      if(mu8_NetworkCount<MAX_WIFI_NETWORKS)
        mu8_NetworkCount++;

      b_DataChanged = true;
    }
  }

  if(b_DataChanged)
  {
    notifyObservers(static_cast<EDataField>(EField::Networks)); 
  }
}

  
/**
 * @brief Get the number of Wi-Fi networks.
 * 
 * This function returns the count of Wi-Fi networks that have been detected
 * and stored in the WifiSettings object.
 * 
 * @return uint8_t The number of Wi-Fi networks.
 */
uint8_t WifiSettings::getNetworkCount(void) const
{
  return mu8_NetworkCount;
}


/** 
 * @brief Retrieves the SSID and password of a WiFi network at a specified index.
 * 
 * @param ppc_SSID Pointer to store the SSID of the WiFi network.
 * @param ppc_Password Pointer to store the password of the WiFi network.
 * @param u8_Index The index of the WiFi network.
 */
void WifiSettings::getNetwork(const char **ppc_SSID, const char **ppc_Password, const uint8_t u8_Index)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  if((u8_Index>=MAX_WIFI_NETWORKS) || (u8_Index>=mu8_NetworkCount))
  {
    if(ppc_SSID)
      *ppc_SSID = "";
    if(ppc_Password)      
      *ppc_Password = "";
    return;
  }

  if(ppc_SSID)
    *ppc_SSID = ma_Networks[u8_Index].ac_SSID;
  if(ppc_Password)
    *ppc_Password = ma_Networks[u8_Index].ac_Password;
}





/**
 * @brief Retrieves the password for a specified Wi-Fi network by its SSID.
 * 
 * @param pc_SSID The SSID of the Wi-Fi network whose password is to be retrieved.
 * @return const char* The password of the specified Wi-Fi network. 
 *                     Returns an empty string if the SSID is not found.
 */
const char *WifiSettings::getNetworkPassword(const char *pc_SSID)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  for(uint8_t u8_Index = 0; u8_Index < mu8_NetworkCount; u8_Index++)
  {
    if(strcmp(ma_Networks[u8_Index].ac_SSID, pc_SSID) == 0)
    {
      return ma_Networks[u8_Index].ac_Password;
    }
  }
  return "";
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

  std::lock_guard<std::mutex> lock(m_DataMutex);

  pu8_Buffer[u16_BufferPos++] = mb_EnableState ? 1 : 0;
  pu8_Buffer[u16_BufferPos++] = mb_ConnectState ? 1 : 0; 

  pu8_Buffer[u16_BufferPos++] = mu8_NetworkCount; 
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

  std::lock_guard<std::mutex> lock(m_DataMutex);

  if((!pu8_Buffer) || (u16_Size<WIFI_SETTINGS_SIZE))
    return;

  mb_EnableState = pu8_Buffer[u16_BufferPos++] ? true : false;
  mb_ConnectState = pu8_Buffer[u16_BufferPos++] ? true : false;

  mu8_NetworkCount = pu8_Buffer[u16_BufferPos++];
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

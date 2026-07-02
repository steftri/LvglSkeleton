/**
 * @file system_settings.cpp
 * @brief Implementation of the SystemSettings class for managing system settings.
 * 
 * This file contains the implementation of the SystemSettings class, which provides
 * methods for initializing, setting, and retrieving system settings, as well
 * as serializing and unserializing these settings.
 * 
 * The class manages system settings such as hostname and splash screen preferences.
 * It allows setting and retrieving these settings. The settings can be serialized
 * into a buffer for storage and unserialized back into the class from a buffer.
 *
 * @author Stefan Trippler, ERNI (Deutschland) GmbH
 */
#include <string.h>

#include "system_settings.h"


/**
 * @brief Constructor for the SystemSettings class.
 * 
 * This constructor initializes the SystemSettings object by calling the init() method.
 */
SystemSettings::SystemSettings(void)
{
  init();
}


/**
 * @brief Initializes the SystemSettings object.
 * 
 * This function sets the initial state of the SystemSettings object by 
 * resetting the count of WiFi networks to zero and clearing the SSID 
 * and password for each network in the list.
 */
void SystemSettings::init(void)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  me_SplashScreen = ESplashScreen::None;
  strncpy(mac_HostName, APPLICATION_NAME, MAX_HOSTNAME_LENGTH);
  mac_HostName[MAX_HOSTNAME_LENGTH] = '\0'; // Ensure null-termination
}


/**
 * @brief Sets the splash screen setting.
 * 
 * This function sets the splash screen setting. If the setting changes,
 * it notifies the observers about the change.
 * 
 * @param e_SplashScreen The new splash screen setting.
 */
void SystemSettings::setSplashScreen(ESplashScreen e_SplashScreen)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  if(me_SplashScreen != e_SplashScreen)
  {
    me_SplashScreen = e_SplashScreen;
    notifyObservers(static_cast<EDataField>(EField::SplashScreen)); // Notify observers that the splash screen setting has changed
  }
}


/**
 * @brief Retrieves the current splash screen setting.
 * 
 * This function retrieves the current splash screen setting.
 * 
 * @return The current splash screen setting.
 */
SystemSettings::ESplashScreen SystemSettings::getSplashScreen(void) const
{
  return me_SplashScreen;
}


/**
 * @brief Sets the hostname of the system.
 * 
 * This function sets the hostname of the system. If the hostname changes,
 * it notifies the observers about the change.
 * 
 * @param pc_HostName The new hostname.
 */
void SystemSettings::setHostName(const char *pc_HostName)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  if(strncmp(mac_HostName, pc_HostName, MAX_HOSTNAME_LENGTH) != 0)
  {
    strncpy(mac_HostName, pc_HostName, MAX_HOSTNAME_LENGTH);
    mac_HostName[MAX_HOSTNAME_LENGTH] = '\0'; // Ensure null-termination
    notifyObservers(static_cast<EDataField>(EField::HostName)); // Notify observers that the hostname has changed
  }
}


/**
 * @brief Retrieves the hostname of the system.
 * 
 * This function retrieves the current hostname of the system.
 * 
 * @return The current hostname.
 */
const char *SystemSettings::getHostName(void) const
{
  return mac_HostName;
}


/**
 * @brief Serializes the System settings into a buffer.
 *
 * This function serializes the system settings, including the hostname
 * and splash screen settings, into the provided buffer.
 *
 * @param pu8_Buffer Pointer to the buffer where the serialized data will be stored.
 * @param u16_BufferSize Size of the provided buffer.
 * @return The number of bytes written to the buffer, or 0 if the buffer is null
 *         or the buffer size is less than the required size.
 */
uint16_t SystemSettings::serialize(uint8_t *pu8_Buffer, const uint16_t u16_BufferSize)
{
  uint16_t u16_BufferPos = 0;

  if((!pu8_Buffer) || (u16_BufferSize<SYSTEM_SETTINGS_SIZE))
    return 0;

  std::lock_guard<std::mutex> lock(m_DataMutex);  

  pu8_Buffer[u16_BufferPos++] = static_cast<uint8_t>(me_SplashScreen);
  for(auto i=0; i<MAX_HOSTNAME_LENGTH; i++)
    pu8_Buffer[u16_BufferPos++] = mac_HostName[i];

  return u16_BufferPos;
}


/**
 * @brief Unserializes the System settings from a given buffer.
 *
 * This function reads the System settings from a buffer and populates the
 * internal data structures with the deserialized values. It expects the
 * buffer to contain the splash screen setting followed by the hostname.

 *
 * @param pu8_Buffer Pointer to the buffer containing the serialized System settings.
 * @param u16_Size Size of the buffer in bytes.
 *
 * @note The buffer must be at least SYSTEM_SETTINGS_SIZE bytes long.
 *       The function will return immediately if the buffer is null or
 *       smaller than SYSTEM_SETTINGS_SIZE.
 */
void SystemSettings::unserialize(const uint8_t *pu8_Buffer, const uint16_t u16_Size)
{
  uint16_t u16_BufferPos = 0;

  std::lock_guard<std::mutex> lock(m_DataMutex);

  if((!pu8_Buffer) || (u16_Size<SYSTEM_SETTINGS_SIZE))
    return;

  me_SplashScreen = static_cast<ESplashScreen>(pu8_Buffer[u16_BufferPos++]);
  for(auto i=0; i<MAX_HOSTNAME_LENGTH; i++)
    mac_HostName[i] = pu8_Buffer[u16_BufferPos++];
  mac_HostName[MAX_HOSTNAME_LENGTH] = '\0'; // Ensure null-termination
}

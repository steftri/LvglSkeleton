/**
 * @file wifi_settings.h
 * @brief This file contains the definition of the WifiSettings class, which manages Wi-Fi network settings.
 *
 * The WifiSettings class allows for the initialization, setting, and retrieval of Wi-Fi network credentials.
 * It supports serialization and deserialization of the network settings for storage and retrieval.
 *
 * @author Stefan Trippler, ERNI (Deutschland) GmbH
 */

#ifndef WIFI_SETTINGS_H
#define WIFI_SETTINGS_H

#include <inttypes.h>

#include "data.h"

/**
 * @class WifiSettings
 * @brief Manages Wi-Fi network settings including SSID and password.
 *
 * The WifiSettings class provides methods to initialize, set, and retrieve Wi-Fi network credentials.
 * It also supports serialization and deserialization of the network settings.
 */
class WifiSettings : public Data
{
  static const uint8_t MAX_WIFI_NETWORKS = 8; ///< Maximum number of Wi-Fi networks
  static const uint8_t MAX_SSID_LENGTH = 32; ///< Maximum length of SSID
  static const uint8_t MAX_WPA2_PASSWORD_LENGTH = 63; ///< Maximum length of WPA2 password

  bool mb_EnableState; ///< Indicates whether the Wi-Fi station mode is enabled
  
  struct 
  {
    char ac_SSID[MAX_SSID_LENGTH+1]; ///< SSID of the Wi-Fi network
    char ac_Password[MAX_WPA2_PASSWORD_LENGTH+1]; ///< Password of the Wi-Fi network
  } ma_Networks[MAX_WIFI_NETWORKS]; ///< Array of preferred Wi-Fi networks

  uint8_t mu8_NetworkCount; ///< Number of preferred Wi-Fi networks stored

public:
  enum class EField : uint8_t 
  { 
    EnableState = 0,
    Networks, 
  };

  static const uint16_t WIFI_SETTINGS_SIZE = 1UL + 1UL + MAX_WIFI_NETWORKS * (MAX_SSID_LENGTH + MAX_WPA2_PASSWORD_LENGTH); ///< Size of the Wi-Fi settings in bytes

  WifiSettings(void);

  void init(void);
  void setEnableState(bool b_Enable);
  bool getEnableState(void) const;
  void setNetwork(const char *pc_SSID, const char *pc_Password);
  uint8_t getNetworkCount(void);
  void getNetwork(const char **ppc_SSID, const char **ppc_Password, const uint8_t u8_Index);
  const char *getNetworkPassword(const char *pc_SSID);

  uint16_t serialize(uint8_t *pu8_Buffer, const uint16_t u16_BufferSize);
  void unserialize(const uint8_t *pu8_Buffer, const uint16_t u16_Size);
};

#endif

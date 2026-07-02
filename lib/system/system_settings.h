/**
 * @file system_settings.h
 * @brief This file contains the definition of the SystemSettings class, which manages system settings.
 *
 * The SystemSettings class allows for the initialization, setting, and retrieval of system settings.
 * It supports serialization and deserialization of the system settings for storage and retrieval.
 *
 * @author Stefan Trippler, ERNI (Deutschland) GmbH
 */

#ifndef SYSTEM_SETTINGS_H
#define SYSTEM_SETTINGS_H

#include <inttypes.h>

#include "data.h"

/**
 * @class SystemSettings
 * @brief Manages system settings including hostname and splash screen settings.
 *
 * The SystemSettings class provides methods to initialize, set, and retrieve system settings.
 * It also supports serialization and deserialization of the system settings.
 */
class SystemSettings : public Data
{
  static const uint8_t MAX_HOSTNAME_LENGTH = 64; ///< Maximum length of the hostname

public:
  enum class ESplashScreen : uint8_t
  {
    None = 0,
    C64,
    Amiga
  };

private:

  char mac_HostName[MAX_HOSTNAME_LENGTH+1]; ///< Hostname of the system
  ESplashScreen me_SplashScreen; ///< Splash screen setting

public:
  enum class EField : uint8_t 
  { 
    HostName = 0,
    SplashScreen
  };

  static const uint16_t SYSTEM_SETTINGS_SIZE = 1UL + MAX_HOSTNAME_LENGTH; ///< Size of the system settings in bytes

  SystemSettings(void);

  void init(void);
  void setSplashScreen(ESplashScreen e_SplashScreen);
  ESplashScreen getSplashScreen(void) const;
  void setHostName(const char *pc_HostName);
  const char *getHostName(void) const;

  uint16_t serialize(uint8_t *pu8_Buffer, const uint16_t u16_BufferSize);
  void unserialize(const uint8_t *pu8_Buffer, const uint16_t u16_Size);
};

#endif

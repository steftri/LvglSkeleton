#ifndef SETTINGS_CONTAINER_H
#define SETTINGS_CONTAINER_H


#include <inttypes.h>

#include "datastorage.h"

#include "system_settings.h"
#include "wifi_settings.h"
#include "mqtt_settings.h"
#include "lightstripe_settings.h"


class SettingsContainer
{
public: 
  enum class ERc 
  {
    Ok = 0,
    NotFoundError,
    VersionMismatchError,
    VerificationError
  };

private:
  DataStorage m_DataStorage;
  SystemSettings m_SystemSettings;
  WifiSettings m_WifiSettings;
  MqttSettings m_MqttSettings;
  LightstripeSettings m_LightstripeSettings;
  bool mb_Valid;

public:
  SettingsContainer(void);

  ERc save(void);
  ERc load(void);
  void clear(void);

  bool isValid(void);

  SystemSettings &getSystemSettings(void);
  WifiSettings &getWifiSettings(void);
  MqttSettings &getMqttSettings(void);
  LightstripeSettings &getLightstripeSettings(void);
};


#endif
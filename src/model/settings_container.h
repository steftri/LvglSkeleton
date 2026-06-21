#ifndef SETTINGS_CONTAINER_H
#define SETTINGS_CONTAINER_H


#include <inttypes.h>

#include "datastorage.h"

#include "wifi_settings.h"
#include "mqtt_settings.h"


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
  WifiSettings m_WifiSettings;
  MqttSettings m_MqttSettings;
  bool mb_Valid;

public:
  SettingsContainer(void);

  ERc save(void);
  ERc load(void);
  void clear(void);

  bool isValid(void);

  WifiSettings *getWifiSettings(void);
  MqttSettings *getMqttSettings(void);
};


#endif
#ifndef DATA_CONTAINER_H
#define DATA_CONTAINER_H

#include <inttypes.h>

#include "wifi_data.h"
#include "mqtt_data.h"
#include "lightstripe_data.h"
#include "surveillance_data.h"


class DataContainer
{
  WifiData m_WifiData; ///< Holds the Wi-Fi data
  MqttData m_MqttData; ///< Holds the MQTT data
  LightstripeData m_LightstripeData; ///< Holds the Lightstripe data
  SurveillanceData m_SurveillanceData; ///< Holds the surveillance data
    
  // Add other data members as needed, e.g., for MQTT settings, etc.

public:
  DataContainer(void);
  ~DataContainer() = default;

  WifiData &getWifiData(void);
  MqttData &getMqttData(void);
  LightstripeData &getLightstripeData(void);
  SurveillanceData &getSurveillanceData(void);
};


#endif // DATA_CONTAINER_H

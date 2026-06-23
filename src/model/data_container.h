#ifndef DATA_CONTAINER_H
#define DATA_CONTAINER_H

#include <inttypes.h>

#include "wifi_data.h"


class DataContainer
{
  WifiData m_WifiData; ///< Holds the Wi-Fi data
    
  // Add other data members as needed, e.g., for MQTT settings, etc.

public:
  DataContainer(void);
  ~DataContainer() = default;

  WifiData &getWifiData(void);
};


#endif // DATA_CONTAINER_H

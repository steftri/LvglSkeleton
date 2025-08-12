#ifndef DATA_H
#define DATA_H

#include <inttypes.h>

#include "wifi_data.h"


class Data
{
  WifiData m_WifiData; ///< Holds the Wi-Fi data
    
  // Add other data members as needed, e.g., for MQTT settings, etc.

public:
  Data(void);
  ~Data() = default;

  WifiData *getWifiData(void);
};


#endif // DATA_H

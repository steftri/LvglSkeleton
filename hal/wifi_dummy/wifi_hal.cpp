#include <string.h>

#include "wifi_hal.h"


static const uint8_t NUMBER_OF_NETWORKS = 3;

static const char *NETWORK_LIST[] = 
{
  "Kugelblitz",
  "Hyperraum",
  "Hyperraum-Transit"
};



WifiHal::WifiHal(WifiActionInterface &r_ActionListener)
 : mr_ActionListener(r_ActionListener)
 , mb_Enabled(false)
 , mb_Connected(false)
 , mb_NetworksFound(false)
{
}

void WifiHal::setup()
{
  mb_Connected = false;
  mb_NetworksFound = false;
}

void WifiHal::enable()
{
  mb_Enabled = true;
}

void WifiHal::disable()
{
  mb_Enabled = false;
  mb_Connected = false;
}

void WifiHal::scanNetworks()
{
  mb_NetworksFound = true;
  mr_ActionListener.onWifiNetworksUpdated();
}

uint8_t WifiHal::getAvailableNetworkCount() const
{
  return NUMBER_OF_NETWORKS; 
}

void WifiHal::getAvailableNetworkSSID(char *pc_buffer, size_t bufferSize, uint8_t u8_Index) const
{
  if((pc_buffer == nullptr) || (u8_Index > NUMBER_OF_NETWORKS))
    return;

  strncpy(pc_buffer, NETWORK_LIST[u8_Index], bufferSize);
  pc_buffer[bufferSize] = 0;
}


void WifiHal::getAvailableNetworkSignalStrength(int32_t* ps32_signalStrength, uint8_t u8_index) const
{
  // Implement logic to get the signal strength of the available network at the specified index
}

void WifiHal::connect(const char* ssid, const char* password)
{
  mr_ActionListener.onWifiConnecting();
  mr_ActionListener.onWifiConnected();
  mr_ActionListener.onWifiGotIP();
}

void WifiHal::disconnect()
{
  mr_ActionListener.onWifiDisconnected();
}

bool WifiHal::isConnected() const
{
  return mb_Connected; // Placeholder
}

void WifiHal::getIPAddress(char* buffer, size_t bufferSize) const
{
  strncpy(buffer, "192.168.178.33", bufferSize);
}


int WifiHal::getSignalStrength() const
{
  // Implement logic to get the signal strength of the current Wi-Fi connection
  return 0; // Placeholder
}


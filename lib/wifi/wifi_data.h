#ifndef WIFI_DATA_H
#define WIFI_DATA_H

#include <inttypes.h>
#include <stddef.h>

#include "data.h"


class WifiData : public Data
{
public:
  static const uint8_t MAX_WIFI_NETWORKS = 16; ///< Maximum number of Wi-Fi networks
  static const uint8_t MAX_SSID_LENGTH = 32; ///< Maximum length of SSID (excluding null terminator)
  static const uint8_t MAX_WPA2_PASSWORD_LENGTH = 63; ///< Maximum length of WPA2 password
  static const uint8_t MAX_IP_ADDRESS_LENGTH = 15; ///< Maximum length of IP address string (excluding null terminator)

  enum class EField : uint8_t 
  { 
    ConnectionState = 0,
    AvailableNetworks, 
    SelectedNetwork, 
    IPAddress
  };

  enum class EState
  {
    Disconnected = 0,
    Connecting,
    Connected,
    Error
  };

private:
  bool mb_Enabled; ///< Indicates whether Wi-Fi is enabled or disabled
  EState me_State; ///< Current state of the Wi-Fi connection

  char mac_AvailableNetworks[MAX_WIFI_NETWORKS][MAX_SSID_LENGTH + 1]; ///< SSID of the Wi-Fi network
  uint8_t mu8_NumberOfAvaliableNetworks; ///< Number of available Wi-Fi networks

  struct 
  {
    char ac_SSID[MAX_SSID_LENGTH + 1]; ///< SSID of the Wi-Fi network
    char ac_Password[MAX_WPA2_PASSWORD_LENGTH + 1]; ///< Password of the Wi-Fi network
  } m_SelectedNetwork;

  char mac_IPAddress[MAX_IP_ADDRESS_LENGTH + 1]; ///< IP address of the device

public:
  WifiData();
  ~WifiData() = default;

  void setEnable(bool b_Enable);
  bool isEnabled() const;

  void setState(EState e_State);
  EState getState(void) const;

  void setAvaliableNetworks(const char **ppc_Networks, const uint8_t u8_NetworkCount);

  uint8_t getAvailableNetworkCount(void) const;
  void getAvailableNetwork(char *pc_SsidBuffer, const size_t bufferSize, const uint8_t u8_Index);

  void setSelectedNetwork(const char *pc_SSID, const char *pc_Password);
  void setSelectedNetworkSSID(const char *pc_SSID); 
  void setSelectedNetworkPassword(const char *pc_Password);
  void getSelectedNetwork(char *pc_SSID, const size_t ssidBufferSize, char *pc_Password, const size_t passwordBufferSize);

  void setIPAddress(const char *pc_IPAddress);
  void getIPAddress(char *pc_IPAddress, const size_t bufferSize);
};

#endif // WIFI_DATA_H
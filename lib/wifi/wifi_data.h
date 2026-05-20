#ifndef WIFI_DATA_H
#define WIFI_DATA_H

#include <inttypes.h>
#include <stddef.h>


class WifiData
{
public:
  static const uint8_t MAX_WIFI_NETWORKS = 16; ///< Maximum number of Wi-Fi networks
  static const uint8_t MAX_SSID_LENGTH = 32; ///< Maximum length of SSID
  static const uint8_t MAX_WPA2_PASSWORD_LENGTH = 63; ///< Maximum length of WPA2 password

  enum class EState
  {
    Disabled = 0,
    Enabled,
    Connecting,
    Connected,
    Error
  };

private:
  bool mb_NetworksFetched; ///< Flag to indicate if the available networks have been fetched

  EState me_State; ///< Current state of the Wi-Fi connection

  char mac_AvailableNetworks[MAX_WIFI_NETWORKS][MAX_SSID_LENGTH + 1]; ///< SSID of the Wi-Fi network
  uint8_t mu8_NumberOfAvaliableNetworks; ///< Number of available Wi-Fi networks

  struct 
  {
    char ac_SSID[MAX_SSID_LENGTH + 1]; ///< SSID of the Wi-Fi network
    char ac_Password[MAX_WPA2_PASSWORD_LENGTH + 1]; ///< Password of the Wi-Fi network
  } m_SelectedNetwork;

  uint8_t mau8_IPAddress[4]; ///< IP address of the device

public:
  WifiData();
  ~WifiData() = default;

  void fetchNetworks(void);
  void fetchState(void);

  void setState(EState e_State);
  EState getState(void) const;

  void setAvaliableNetworks(const char **ppc_Networks, const uint8_t u8_NetworkCount);

  uint8_t getAvailableNetworkCount(void) const;
  void getAvailableNetwork(char *pc_SsidBuffer, const size_t bufferSize, const uint8_t u8_Index) const;

  void setSelectedNetwork(uint8_t u8_Index);
  void setNetworkPassword(const char *pc_Password);
  void getSelectedNetwork(char *pc_SSID, char *pc_Password) const;

  void setIPAddress(const uint8_t u8_IPAddr1, const uint8_t u8_IPAddr2, const uint8_t u8_IPAddr3, const uint8_t u8_IPAddr4);
  void getIPAddress(uint8_t *pu8_IPAddress) const;
};

#endif // WIFI_DATA_H
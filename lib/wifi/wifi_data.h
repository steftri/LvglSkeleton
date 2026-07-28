#ifndef WIFI_DATA_H
#define WIFI_DATA_H

#include <inttypes.h>
#include <stddef.h>

#include "data.h"


class WifiData : public Data
{
public:
  static const size_t MAX_WIFI_NETWORKS = 16; ///< Maximum number of Wi-Fi networks
  static const size_t MAX_SSID_LENGTH = 32; ///< Maximum length of SSID (excluding null terminator)
  static const size_t MAX_WPA2_PASSWORD_LENGTH = 63; ///< Maximum length of WPA2 password
  static const size_t MAX_IP_ADDRESS_LENGTH = 15; ///< Maximum length of IP address string (excluding null terminator)
  static const size_t MAX_ERROR_MESSAGE_LENGTH = 64; ///< Maximum length of the error message

  enum class EField : uint8_t 
  { 
    EnableState = 0,
    ConnectionState,
    LastError,
    AvailableNetworks, 
    SelectedNetwork, 
    IPAddress,
    Time
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

  uint8_t mu8_LastErrorCode; ///< Last error code encountered
  char mac_LastErrorMessage[MAX_ERROR_MESSAGE_LENGTH+1]; ///< Last error message encountered


  char mac_AvailableNetworks[MAX_WIFI_NETWORKS][MAX_SSID_LENGTH + 1]; ///< SSID of the Wi-Fi network
  uint8_t mu8_NumberOfAvaliableNetworks; ///< Number of available Wi-Fi networks

  struct 
  {
    char ac_SSID[MAX_SSID_LENGTH + 1]; ///< SSID of the Wi-Fi network
    char ac_Password[MAX_WPA2_PASSWORD_LENGTH + 1]; ///< Password of the Wi-Fi network
  } m_SelectedNetwork;

  char mac_IPAddress[MAX_IP_ADDRESS_LENGTH + 1]; ///< IP address of the device

  time_t m_UtcTimeOffset; ///< Last time offset between the uptime and the UTC time in seconds
  struct tm m_TimeInfo;

public:
  WifiData();
  ~WifiData() = default;

  void setEnable(bool b_Enable);
  bool isEnabled() const;

  void setState(EState e_State, uint8_t u8_ErrorCode = 0, const char *pc_ErrorMessage = nullptr);
  EState getState(void) const;
  uint8_t getLastErrorCode(void) const;
  const char *getLastErrorMessage(void) const;

  void setAvaliableNetworks(const char **ppc_Networks, uint8_t u8_NetworkCount);

  uint8_t getAvailableNetworkCount(void) const;
  void getAvailableNetwork(char *pc_SsidBuffer, const size_t bufferSize, const uint8_t u8_Index);

  void setSelectedNetwork(const char *pc_SSID, const char *pc_Password);
  void setSelectedNetworkSSID(const char *pc_SSID); 
  void setSelectedNetworkPassword(const char *pc_Password);
  void getSelectedNetwork(char *pc_SSID, const size_t ssidBufferSize, char *pc_Password, const size_t passwordBufferSize);

  void setIPAddress(const char *pc_IPAddress);
  void getIPAddress(char *pc_IPAddress, const size_t bufferSize);

  void setLocalTime(const struct tm &r_Timeinfo);
  time_t getTime();
  void getLocalTime(struct tm *p_Timeinfo);
};

#endif // WIFI_DATA_H
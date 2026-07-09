#ifndef WIFI_HAL_H
#define WIFI_HAL_H

#include <WiFi.h>

#include "interfaces/wifi_hal_interface.h"
#include "interfaces/wifi_action_interface.h"

class WifiHal : public WifiHalInterface
{
public:
  WifiHal(WifiActionInterface &actionListener);
  ~WifiHal() = default;

  enum class ERc : int32_t
  {
    Ok = 0,
    Error = -1,
    ConnectionRefused = -2,
    ConnectionTimeout = -3,
    UnacceptableProtocolVersion = -4,
    IdentifierRejected = -5,
    ServerUnavailable = -6,
    BadUserNameOrPassword = -7,
    NotAuthorized = -8,
    UnknownError = -9
  };  

  void setup() override;  

  void enable() override;
  void disable() override;

  void setHostname(const char *pc_Hostname) override;

  void scanNetworks() override;
  uint8_t getAvailableNetworkCount() const override;
  void getAvailableNetworkSSID(char* pc_buffer, size_t bufferSize, uint8_t u8_index) const override;
  void getAvailableNetworkSignalStrength(int32_t* ps32_signalStrength, uint8_t u8_index) const override;

  void connect(const char* pc_ssid, const char* pc_password) override;
  void disconnect() override;
  bool isConnected() const override;
  void getIPAddress(char* pc_buffer, size_t bufferSize) const override;
  int getSignalStrength() const override;

  const char *disconnectReasonToString(uint8_t u8_Reason) const override;
   
private:
  static WifiHal *mp_thisInstance; // Static instance pointer for task access
  WifiActionInterface &m_actionListener; // Reference to the action listener
  static void onEvent(arduino_event_id_t event, arduino_event_info_t info);
};

#endif  // WIFI_HAL_H
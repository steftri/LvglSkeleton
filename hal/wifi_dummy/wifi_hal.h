#ifndef WIFI_H
#define WIFI_H

#include "interfaces/wifi_hal_interface.h"
#include "interfaces/wifi_action_interface.h"

class WifiHal : public WifiHalInterface
{
  WifiActionInterface &mr_ActionListener;
  bool mb_Enabled;
  bool mb_Connected;
  bool mb_NetworksFound;

  enum class EWifiConnectionError : uint8_t
  {
    None = 0,
    WrongPassword,
    NetworkNotFound,
    Timeout
  };

public:
  WifiHal(WifiActionInterface &actionListener);
  ~WifiHal() = default;

  void setup() override;

  void enable() override;
  void disable() override;

  void setHostname(const char *pc_Hostname) override;

  void scanNetworks() override;
  uint8_t getAvailableNetworkCount() const override;
  void getAvailableNetworkSSID(char* buffer, size_t bufferSize, uint8_t index) const override;
  void getAvailableNetworkSignalStrength(int32_t* ps32_signalStrength, uint8_t u8_index) const override;

  void connect(const char* ssid, const char* password) override;
  void disconnect() override;
  bool isConnected() const override;
  void getIPAddress(char* buffer, size_t bufferSize) const override;
  int getSignalStrength() const override;

  const char *disconnectReasonToString(uint8_t u8_Reason) const override;

  void configTime(int32_t s32_GmtOffset, int32_t s32_DstOffset, const char *pc_NtpServer) override;
  int32_t getLocalTime(struct tm *p_Timeinfo, uint32_t u32_TimeoutMs = 5000) const override;  
};

#endif // WIFI_H

#ifndef WIFI_H
#define WIFI_H

#include "interfaces/wifi_hal_interface.h"
#include "interfaces/wifi_action_interface.h"

class WifiHal : public WifiHalInterface
{
public:
  WifiHal(WifiActionInterface &actionListener);
  ~WifiHal() = default;

  void setup() override;

  void enable() override;
  void disable() override;

  void scanNetworks() override;
  uint8_t getAvailableNetworkCount() const override;
  void getAvailableNetworkSSID(char* buffer, size_t bufferSize, uint8_t index) const override;
  void getAvailableNetworkSignalStrength(int32_t* ps32_signalStrength, uint8_t u8_index) const override;

  void connect(const char* ssid, const char* password) override;
  void disconnect() override;
  bool isConnected() const override;
  void getIPAddress(char* buffer, size_t bufferSize) const override;
  int getSignalStrength() const override;
};

#endif // WIFI_H

#ifndef WIFI_H
#define WIFI_H

#include "interfaces/wifi_hal_interface.h"

class WifiHal : public WifiHalInterface
{
public:
  WifiHal();
  ~WifiHal() = default;

  void setup() override;

  void scanNetworks() override;
  uint8_t getAvailableNetworkCount() const override;
  void getAvailableNetworkSSID(char* buffer, size_t bufferSize, uint8_t index) const override;
  void getAvailableNetworkSignalStrength(int* signalStrength, uint8_t index) const override;

  void connect(const char* ssid, const char* password) override;
  void disconnect() override;
  bool isConnected() const override;
  void getIPAddress(char* buffer, size_t bufferSize) const override;
  int getSignalStrength() const override;
};

#endif // WIFI_H

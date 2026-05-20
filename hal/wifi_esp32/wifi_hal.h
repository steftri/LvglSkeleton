#ifndef WIFI_HAL_H
#define WIFI_HAL_H

#include <WiFi.h>

#include "interfaces/wifi_hal_interface.h"

class WifiHal : public WifiHalInterface
{
private:
  static bool b_ScanComplete;

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

private:
  static void onEvent(WiFiEvent_t event);
};

#endif  // WIFI_HAL_INTERFACE_H
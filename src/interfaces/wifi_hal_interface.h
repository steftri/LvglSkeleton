#ifndef WIFI_HAL_INTERFACE_H
#define WIFI_HAL_INTERFACE_H

#include <inttypes.h>
#include <stddef.h>

class WifiHalInterface
{
public:
  virtual ~WifiHalInterface() = default;

  virtual void setup() = 0;

  virtual void scanNetworks() = 0;
  virtual uint8_t getAvailableNetworkCount() const = 0;
  virtual void getAvailableNetworkSSID(char* buffer, size_t bufferSize, uint8_t index) const = 0;
  virtual void getAvailableNetworkSignalStrength(int* signalStrength, uint8_t index) const = 0;

  virtual void connect(const char* ssid, const char* password) = 0;
  virtual void disconnect() = 0;
  virtual bool isConnected() const = 0;
  virtual void getIPAddress(char* buffer, size_t bufferSize) const = 0;
  virtual int getSignalStrength() const = 0;
};


#endif  // WIFI_INTERFACE_H

#ifndef WIFI_HAL_INTERFACE_H
#define WIFI_HAL_INTERFACE_H

#include <inttypes.h>
#include <stddef.h>

class WifiHalInterface
{
public:
  virtual ~WifiHalInterface() = default;

  virtual void setup() = 0;

  virtual void enable() = 0;
  virtual void disable() = 0;

  virtual void setHostname(const char *pc_Hostname) = 0;

  virtual void scanNetworks() = 0;
  virtual uint8_t getAvailableNetworkCount() const = 0;
  virtual void getAvailableNetworkSSID(char* pc_buffer, size_t bufferSize, uint8_t u8_index) const = 0;
  virtual void getAvailableNetworkSignalStrength(int32_t* ps32_signalStrength, uint8_t u8_index) const = 0;

  virtual void connect(const char* pc_ssid, const char* pc_password) = 0;
  virtual void disconnect() = 0;
  virtual bool isConnected() const = 0;
  virtual void getIPAddress(char* pc_buffer, size_t bufferSize) const = 0;
  virtual int getSignalStrength() const = 0;
};


#endif  // WIFI_HAL_INTERFACE_H

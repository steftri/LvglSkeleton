#ifndef WIFI_ACTION_INTERFACE_H
#define WIFI_ACTION_INTERFACE_H

class WifiActionInterface
{
public:
  virtual ~WifiActionInterface() = default;

  virtual void onWifiNetworksUpdated() = 0;
  virtual void onWifiConnecting() = 0;
  virtual void onWifiConnected() = 0;
  virtual void onWifiDisconnected() = 0;
  virtual void onWifiGotIP() = 0;

  virtual void onWifiConnectionFailed(uint8_t u8_Reason) = 0;
};

#endif // WIFI_ACTION_INTERFACE_H

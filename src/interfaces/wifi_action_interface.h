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

  enum class EWifiConnectionError
  {
    None = 0,
    WrongPassword,
    NoSSIDAvailable,
    Timeout,
    UnknownError
  };
  virtual void onWifiConnectionFailed(EWifiConnectionError error) = 0;
};

#endif // WIFI_ACTION_INTERFACE_H

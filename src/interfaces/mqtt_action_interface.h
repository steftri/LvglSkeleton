#ifndef MQTT_ACTION_INTERFACE_H
#define MQTT_ACTION_INTERFACE_H

class MqttActionInterface
{
public:
  virtual ~MqttActionInterface() = default;

  virtual void onConnected() = 0;
  virtual void onDisconnected() = 0;
  virtual void onMessageReceived(const char *topic, const char *message) = 0;

  enum class EConnectionError
  {
    None = 0,
    Timeout,
    UnknownError
  };
  virtual void onConnectionFailed(EConnectionError error) = 0;
};

#endif // MQTT_ACTION_INTERFACE_H

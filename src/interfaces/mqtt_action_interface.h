#ifndef MQTT_ACTION_INTERFACE_H
#define MQTT_ACTION_INTERFACE_H

#include "mqtt_interface.h"


class MqttActionInterface
{
public:
  virtual ~MqttActionInterface() = default;

  virtual void onConnected() = 0;
  virtual void onDisconnected() = 0;

  virtual void onMessageReceived(const char *pc_Topic, const uint8_t *pu8_MessageBuffer, const size_t MessageSize) = 0;
  virtual void onConnectionFailed(int32_t s32_Error) = 0;
};

#endif // MQTT_ACTION_INTERFACE_H

#ifndef MQTT_TASK_H
#define MQTT_TASK_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "mqtt_hal.h"
#include "interfaces/mqtt_action_interface.h"

#include "mqtt_data.h"
#include "mqtt_settings.h"


static const size_t MQTT_TASK_STACK_SIZE = 4096; // Stack size for the task


class MqttTask : public MqttActionInterface
{
private:
  TaskHandle_t mp_TaskHandle;
  StaticTask_t m_TaskBuffer;
  StackType_t m_TaskStack[ MQTT_TASK_STACK_SIZE ];

  static void task(void *pvParameters);  
  static MqttTask *mp_thisInstance; // Static instance pointer for task access

  MqttHal m_MqttHal;
  MqttSettings &m_MqttSettings; // Reference to the MQTT settings in the model
  MqttData &m_MqttData; // Reference to the MQTT data in the model

public:
  MqttTask(MqttSettings &mqttSettings, MqttData &mqttData);

  void begin(void);

  void connect();
  void disconnect();

private:  
  void setup(void);
  void loop(void);

  // Thread-internal methods to perform actions based on notifications
  void actionConnect();
  void actionDisconnect();

  // MqttActionInterface implementation
  void onConnected() override;
  void onDisconnected() override;
  void onMessageReceived(const char *topic, const char *message) override;
  void onConnectionFailed(EConnectionError error) override;
};



#endif // MQTT_TASK_H
#ifndef MQTT_TASK_H
#define MQTT_TASK_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "mqtt_hal.h"
#include "interfaces/mqtt_action_interface.h"
#include "data_observer.h"

#include "system_settings.h"
#include "mqtt_settings.h"
#include "mqtt_data.h"
#include "wifi_data.h"

static const size_t MAX_MQTT_MESSAGE_SIZE = 128; // Maximum supported MQTT payload size in bytes
static const size_t MQTT_TASK_MESSAGE_QUEUE_SIZE = 4; // Size of the message queue for inter-task communication


class MqttTask : public MqttActionInterface, public DataObserverInterface
{
private:
  TaskHandle_t mp_TaskHandle;
  StaticTask_t m_TaskBuffer;

  static const size_t MQTT_TASK_STACK_SIZE = 4096;
  StackType_t m_TaskStack[ MQTT_TASK_STACK_SIZE ];

  static void task(void *pvParameters);  
  static MqttTask *mp_thisInstance; 

  typedef struct
  {
    uint8_t au8_MessageBuffer[MAX_MQTT_MESSAGE_SIZE];
    size_t MessageSize;
    uint8_t u8_QoS;
    bool b_Retain;
  } SMqttMessage;

  SMqttMessage ma_MqttMessageQueueStorage[MQTT_TASK_MESSAGE_QUEUE_SIZE];
  static StaticQueue_t m_MqttStaticQueue;
  QueueHandle_t m_MqttQueueHandle;

  MqttHal m_MqttHal;
  SystemSettings &m_SystemSettings;
  MqttSettings &m_MqttSettings;
  MqttData &m_MqttData;
  WifiData &m_WifiData;

public:
  MqttTask(SystemSettings &systemSettings, MqttSettings &mqttSettings, MqttData &mqttData, WifiData &wifiData);

  void begin();

  void publishNodeData(uint8_t *pu8_MessageBuffer, const size_t MessageSize, uint8_t u8_QoS = 0, bool b_Retain = false);

private:  
  void setup(void);
  void loop(void);

  void connect();
  void disconnect();

  // DataObserver implementation
  void onDataChanged(Data &r_Data, EDataField e_Field) override;
  void onSystemSettingsChanged(EDataField e_Field);
  void onMqttSettingsChanged(EDataField e_Field);
  void onWifiDataChanged(EDataField e_Field);  
  
  // Thread-internal methods to perform actions based on notifications
  void actionChangedWifiConnectionState();
  void actionChangedWifiIPAddress();
  void actionChangeBrokerSettings();
  void actionChangeSparkplugBSettings();

  void actionPublishNodeData(uint8_t *pu8_MessageBuffer, const size_t MessageSize, uint8_t u8_QoS, bool b_Retain);

  // MqttActionInterface implementation
  void onConnected() override;
  void onDisconnected() override;
  void onConnectionFailed(int32_t s32_Error) override;

  void onMessageReceived(const char *pc_Topic, const uint8_t *pu8_MessageBuffer, const size_t MessageSize) override;
};



#endif // MQTT_TASK_H
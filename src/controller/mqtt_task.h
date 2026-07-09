#ifndef MQTT_TASK_H
#define MQTT_TASK_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "mqtt_hal.h"
#include "interfaces/mqtt_action_interface.h"
#include "data_observer.h"

#include "system_settings.h"
#include "mqtt_settings.h"
#include "mqtt_data.h"
#include "wifi_data.h"




class MqttTask : public MqttActionInterface, public DataObserverInterface
{
private:
  TaskHandle_t mp_TaskHandle;
  StaticTask_t m_TaskBuffer;

  static const size_t MQTT_TASK_STACK_SIZE = 4096;
  StackType_t m_TaskStack[ MQTT_TASK_STACK_SIZE ];

  static void task(void *pvParameters);  
  static MqttTask *mp_thisInstance; 

  MqttHal m_MqttHal;
  SystemSettings &m_SystemSettings;
  MqttSettings &m_MqttSettings;
  MqttData &m_MqttData;
  WifiData &m_WifiData;

public:
  MqttTask(SystemSettings &systemSettings, MqttSettings &mqttSettings, MqttData &mqttData, WifiData &wifiData);

  void begin();

  void publish(const char *pc_Topic, const uint8_t *pu8_MessageBuffer, const size_t MessageSize, uint8_t u8_QoS, bool b_Retain);
  void subscribe(const char *pc_Topic);

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

  // MqttActionInterface implementation
  void onConnected() override;
  void onDisconnected() override;

  void onMessageReceived(const char *topic, const char *message) override;
  void onConnectionFailed(int32_t s32_Error) override;
};



#endif // MQTT_TASK_H
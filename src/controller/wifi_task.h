#ifndef WIFI_TASK_H
#define WIFI_TASK_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "wifi_hal.h"
#include "interfaces/wifi_action_interface.h"

#include "wifi_data.h"


static const size_t WIFI_TASK_STACK_SIZE = 4096; // Stack size for the task


class WifiTask : public WifiActionInterface
{
private:
  TaskHandle_t mp_TaskHandle;
  StaticTask_t m_TaskBuffer;
  StackType_t m_TaskStack[ WIFI_TASK_STACK_SIZE ];

  static void task(void *pvParameters);  
  static WifiTask *mp_thisInstance; // Static instance pointer for task access

  WifiHal m_WifiHal;
  WifiData &m_WifiData; // Reference to the Wi-Fi data in the model

public:
  WifiTask(WifiData &wifiData);

  void begin(void);

  void enable();
  void disable();

private:  
  void setup(void);
  void loop(void);

  // WifiActionInterface implementation
  void onWifiNetworksUpdated() override;
  void onWifiConnected() override;
  void onWifiDisconnected() override;
  void onWifiGotIP() override;
  void onWifiConnectionFailed(EWifiConnectionError error) override;
};



#endif // WIFI_TASK_H
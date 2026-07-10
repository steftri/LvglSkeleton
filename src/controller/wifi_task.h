#ifndef WIFI_TASK_H
#define WIFI_TASK_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "wifi_hal.h"
#include "interfaces/wifi_action_interface.h"


#include "system_settings.h"
#include "wifi_settings.h"
#include "wifi_data.h"


static const size_t WIFI_TASK_STACK_SIZE = 4096; // Stack size for the task


class WifiTask : public WifiActionInterface
{
private:
  TaskHandle_t mp_TaskHandle;

  static void task(void *pvParameters);  
  static WifiTask *mp_thisInstance; // Static instance pointer for task access

  WifiHal m_WifiHal;

  SystemSettings &m_SystemSettings;
  WifiSettings &m_WifiSettings;
  WifiData &m_WifiData;

public:
  WifiTask(SystemSettings &systemSettings, WifiSettings &wifiSettings, WifiData &wifiData);

  void begin(void);

  void enable();
  void disable();
  void connect();
  void disconnect();

private:  
  void setup(void);
  void loop(void);

  // Thread-internal methods to perform actions based on notifications
  void actionEnable();
  void actionDisable();
  void actionConnect();
  void actionDisconnect();

  // WifiActionInterface implementation
  void onWifiNetworksUpdated() override;
  void onWifiConnecting() override;
  void onWifiConnected() override;
  void onWifiDisconnected() override;
  void onWifiGotIP() override;
  void onWifiConnectionFailed(uint8_t u8_Reason) override;
};



#endif // WIFI_TASK_H
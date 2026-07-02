#ifndef LV_TAB_SETTINGS_H
#define LV_TAB_SETTINGS_H

#include <lvgl.h>
#include "wifi_data.h"


class LvTabSettings
{
  lv_obj_t *mp_HostName;
  lv_obj_t *mp_SplashScreen;

  lv_obj_t *mp_WlanEnableSwitch;
  //lv_obj_t *mp_WlanStatePanel;
  lv_obj_t *mp_CurrentWlan;
  lv_obj_t *mp_CurrentIp;
  lv_obj_t *mp_SelectWlanButton;
  lv_obj_t *mp_WlanSelectList;
  lv_obj_t *mp_WlanSsidButton[WifiData::MAX_WIFI_NETWORKS];
  lv_obj_t *mp_WlanSsidLabel[WifiData::MAX_WIFI_NETWORKS];
  lv_obj_t *mp_DisconnectButton;

  lv_obj_t *mp_MqttBroker;
  lv_obj_t *mp_MqttPort;
  lv_obj_t *mp_MqttGroupId;


private:
  static void onInputEvent(lv_event_t *p_Event);

  void onInputSystemHostName(const char *pc_HostName);
  void onInputSystemSplashScreen(uint8_t u8_SplashScreen);

  void onInputMqttBrokerCallback(const char *pc_Broker);
  void onInputMqttPortCallback(uint16_t u16_Port);
  void onInputMqttGroupIdCallback(const char *pc_GroupId);

  static void onWlanEnableCallback(lv_event_t *p_Event);
  static void onWlanDisconnectButtonCallback(lv_event_t *p_Event);
  static void onWlanSsidButtonCallback(lv_event_t *p_Event);

public:
  LvTabSettings();
  ~LvTabSettings() = default;

  void setup(lv_obj_t *p_ParentTab);

  void updateWlanStatePanel(void);
  void updateWlanSelectList(void);
};


#endif // LV_TAB_SETTINGS_H

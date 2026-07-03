#ifndef LV_TAB_SETTINGS_H
#define LV_TAB_SETTINGS_H

#include <lvgl.h>
#include "wifi_data.h"


class LvTabSettings
{
  struct 
  {
    lv_obj_t *mp_HostName;
    lv_obj_t *mp_SplashScreen;
  } m_System;

  struct  
  {
    lv_obj_t *mp_EnableSwitch;
    lv_obj_t *mp_StatePanel;
    lv_obj_t *mp_CurrentWlan;
    lv_obj_t *mp_CurrentIp;
    lv_obj_t *mp_SelectList;
    lv_obj_t *mp_SsidButton[WifiData::MAX_WIFI_NETWORKS];
    lv_obj_t *mp_SsidLabel[WifiData::MAX_WIFI_NETWORKS];
    lv_obj_t *mp_DisconnectButton;
    lv_obj_t *mp_PasswordDialog;
    lv_obj_t *mp_SsidPassword;
  } m_Wlan;


  struct 
  {
    lv_obj_t *mp_Broker;
    lv_obj_t *mp_Port;
    lv_obj_t *mp_GroupId;
  } m_Mqtt;

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

  static void onPasswordEnteredEvent(lv_event_t *p_Event);

public:
  LvTabSettings();
  ~LvTabSettings() = default;

  void setup(lv_obj_t *p_ParentTab);

  void showWlanPasswordDialog(const char *pc_Ssid, const char *pc_Password);

  void updateWlanStatePanel(void);
  void updateWlanSelectList(void);
};


#endif // LV_TAB_SETTINGS_H

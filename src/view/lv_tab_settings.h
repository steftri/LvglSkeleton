#ifndef LV_TAB_SETTINGS_H
#define LV_TAB_SETTINGS_H

#include <lvgl.h>
#include "wifi_data.h"


class LvTabSettings
{
  lv_obj_t *mp_WlanEnableSwitch;
  lv_obj_t *mp_WlanStatePanel;
  lv_obj_t *mp_CurrentWlan;
  lv_obj_t *mp_CurrentIp;
  lv_obj_t *mp_WlanSelectList;
  lv_obj_t *mp_WlanSsidButton[WifiData::MAX_WIFI_NETWORKS];
  lv_obj_t *mp_WlanSsidLabel[WifiData::MAX_WIFI_NETWORKS];

private:
  static void onWlanEnableCallback(lv_event_t *p_Event);
  static void onWlanSsidButtonCallback(lv_event_t *p_Event);

public:
  LvTabSettings();

  void setup(lv_obj_t *p_ParentTab);

  void updateWlanStatePanel(void);
  void updateWlanSelectList(void);
};




#endif // LV_TAB_SETTINGS_H
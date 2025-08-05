#ifndef LV_TAB_SETTINGS_H
#define LV_TAB_SETTINGS_H

#include <lvgl.h>


class LvTabSettings
{
  lv_obj_t *mp_WlanEnableSwitch;

private:
  static void wlanEnableCallback(lv_event_t *p_Event);

public:
  LvTabSettings();

  void setup(lv_obj_t *p_ParentTab);
};




#endif // LV_TAB_SETTINGS_H
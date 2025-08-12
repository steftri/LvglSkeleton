#ifndef LV_WLAN_PASSWORD_H
#define LV_WLAN_PASSWORD_H

#include <lvgl.h>

#include "wifi_data.h"

class LvWlanPasswordDialogbox
{
  lv_obj_t *mp_PasswordPanel;
  lv_obj_t *mp_PasswordInput;
  lv_obj_t *mp_SsidLabel;

  static lv_obj_t *mp_Keyboard; // Keyboard for input

public:
  LvWlanPasswordDialogbox();
    
  void setup(lv_obj_t *p_ParentTab);
    
  void show(const char *pc_Ssid, const char *pc_Password = nullptr);
  void hide(void);

private:
  static void onInputEvent(lv_event_t *p_Event);
  static void onOkButtonEvent(lv_event_t *p_Event);
  static void onCancelButtonEvent(lv_event_t *p_Event);
};

#endif // LV_WLAN_PASSWORD_H

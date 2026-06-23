#ifndef LV_WLAN_PASSWD_DIALOG_H
#define LV_WLAN_PASSWD_DIALOG_H

#include <lvgl.h>

#include "wifi_data.h"


class LvMain; // Forward declaration to avoid circular dependency



class LvWlanPasswdDialog
{
  class LvMain &mr_LvMain; // Reference to the main view for showing/hiding the keyboard

  lv_obj_t *mp_PasswordPanel;
  lv_obj_t *mp_PasswordInput;
  lv_obj_t *mp_SsidLabel;

public:
  LvWlanPasswdDialog(LvMain &r_LvMain);
    
  void setup(lv_obj_t *p_ParentTab);
    
  void show(const char *pc_Ssid, const char *pc_Password = nullptr);
  void hide(void);

private:
  static void onInputEvent(lv_event_t *p_Event);
  static void onOkButtonEvent(lv_event_t *p_Event);
  static void onCancelButtonEvent(lv_event_t *p_Event);
};

#endif // LV_WLAN_PASSWD_DIALOG_H

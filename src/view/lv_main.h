#ifndef LV_MAIN_H
#define LV_MAIN_H

#include <lvgl.h>

#include "lv_tab_settings.h"
#include "lv_wlan_password_dialogbox.h"


class LvMain
{
  lv_obj_t *mp_TabView;
  LvTabSettings m_TabSettings;

  LvWlanPasswordDialogbox m_WlanPasswordDialogbox;

  lv_obj_t *mp_WifiSymbol; 

public:
  LvMain();

  void setup(void);

  void showWlanPasswordDialog(void);
  void updateWlanSymbol(void);

  LvTabSettings *getTabSettings(void);
};




#endif // LV_MAIN_H
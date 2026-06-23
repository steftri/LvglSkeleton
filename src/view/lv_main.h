#ifndef LV_MAIN_H
#define LV_MAIN_H

#include <lvgl.h>

#include "lv_keyboard.h"
#include "lv_tab_info.h"
#include "lv_tab_history.h"
#include "lv_tab_settings.h"
#include "lv_wlan_passwd_dialog.h"


class LvMain
{
  LvKeyboard m_Keyboard;

  lv_obj_t *mp_TabView;
  LvTabInfo m_TabInfo;
  LvTabHistory m_TabHistory;
  LvTabSettings m_TabSettings;

  LvWlanPasswdDialog m_WlanPasswdDialog;

  lv_obj_t *mp_WifiSymbol; 

public:
  LvMain();

  void setup(void);

  void setWlanSymbol(bool b_Visible);

  LvKeyboard *getKeyboard(void);

  LvTabInfo *getTabInfo(void);
  LvTabHistory *getTabHistory(void);
  LvTabSettings *getTabSettings(void);

  LvWlanPasswdDialog *getWlanPasswdDialog(void);
};




#endif // LV_MAIN_H
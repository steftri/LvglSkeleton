#ifndef LV_MAIN_H
#define LV_MAIN_H

#include <lvgl.h>

#include "lv_tab_info.h"
#include "lv_tab_history.h"
#include "lv_tab_settings.h"
#include "lv_wlan_passwd_dialog.h"


class LvMain
{
  lv_obj_t *mp_Keyboard; 

  lv_obj_t *mp_TabView;
  LvTabInfo m_TabInfo;
  LvTabHistory m_TabHistory;
  LvTabSettings m_TabSettings;

  LvWlanPasswdDialog m_WlanPasswdDialog;

  lv_obj_t *mp_WifiSymbol; 

public:
  LvMain();

  void setup(void);

  void showKeyboard(lv_obj_t *p_TargetObj);
  void hideKeyboard(void);

  void showWlanPasswdDialog(void);

  void updateWlanSymbol(void);

  LvTabInfo *getTabInfo(void);
  LvTabHistory *getTabHistory(void);
  LvTabSettings *getTabSettings(void);
};




#endif // LV_MAIN_H
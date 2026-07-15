#ifndef LV_MAIN_H
#define LV_MAIN_H

#include <lvgl.h>

#include "lv_keyboard.h"
#include "lv_tab_info.h"
#include "lv_tab_history.h"
#include "lv_tab_settings.h"


class LvMain
{
  LvKeyboard m_Keyboard;

  lv_obj_t *mp_TabView;
  LvTabInfo m_TabInfo;
  LvTabHistory m_TabHistory;
  LvTabSettings m_TabSettings;

  lv_obj_t *mp_WifiSymbol; 
  lv_obj_t *mp_CloudSymbol;

  lv_obj_t *mp_MenuTabImage; 

public:
  LvMain();

  void setup(void);

  void setWlanSymbol(bool b_Visible);
  void setCloudSymbol(bool b_Visible);

  void showMessageBox(const char *pc_Title, const char *pc_Message);

  LvKeyboard *getKeyboard(void);

  LvTabInfo *getTabInfo(void);
  LvTabHistory *getTabHistory(void);
  LvTabSettings *getTabSettings(void);
};




#endif // LV_MAIN_H
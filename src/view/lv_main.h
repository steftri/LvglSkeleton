#ifndef LV_MAIN_H
#define LV_MAIN_H

#include <lvgl.h>

#include "lv_tab_settings.h"


class LvMain
{
  lv_obj_t *mp_TabView;
  LvTabSettings m_TabSettings;

  lv_obj_t *mp_WifiSymbol; 

public:
  LvMain();

  void setup(void);

  void updateWlanSymbol(void);

  LvTabSettings *getTabSettings(void);
};




#endif // LV_MAIN_H
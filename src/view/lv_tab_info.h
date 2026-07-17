#ifndef LV_TAB_INFO_H
#define LV_TAB_INFO_H

#include <lvgl.h>


class LvTabInfo
{
public:
  LvTabInfo();
  ~LvTabInfo() = default;

  void setup(lv_obj_t *p_ParentTab);
};


#endif // LV_TAB_INFO_H

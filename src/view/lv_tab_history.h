#ifndef LV_TAB_HISTORY_H
#define LV_TAB_HISTORY_H

#include <lvgl.h>


class LvTabHistory
{
public:
  LvTabHistory();
  ~LvTabHistory() = default;

  void setup(lv_obj_t *p_ParentTab);

  void updateHistory(const char *pc_History);
};


#endif // LV_TAB_HISTORY_H

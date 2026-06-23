#ifndef LV_TAB_INFO_H
#define LV_TAB_INFO_H

#include <lvgl.h>


class LvTabInfo
{
private:
  lv_obj_t *mp_FreeRTOSInfoLabel;
  lv_obj_t *mp_LVGLInfoLabel;

public:
  LvTabInfo();
  ~LvTabInfo() = default;

  void setup(lv_obj_t *p_ParentTab);

  void updateFreeRTOSInfo();
  void updateLVGLInfo();
  void updateMQTTInfo();
};


#endif // LV_TAB_INFO_H

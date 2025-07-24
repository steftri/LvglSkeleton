#ifndef DISPLAY_BRIDGE_LVGL_LGFX_H
#define DISPLAY_BRIDGE_LVGL_LGFX_H

#include "hw_display_lgfx.h"
#include "lvgl.h"

class DisplayBridgeLvglLgfx
{
private:
  static DisplayLGFX *mp_DisplayLGFX; 

public:
  DisplayBridgeLvglLgfx(DisplayLGFX *p_DisplayLGFX);
  
  static void flushCallback(lv_display_t *disp, const lv_area_t *area, uint8_t * px_map);
  static void readTouchpanelCallback(lv_indev_t *indev_driver, lv_indev_data_t *data);  
};

#endif // DISPLAY_BRIDGE_LVGL_LGFX_H

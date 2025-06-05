#ifndef DISPLAY_BRIDGE_LVGL_SDL_H
#define DISPLAY_BRIDGE_LVGL_SDL_H

#include <SDL2/SDL.h>


#include <lvgl.h>

class DisplayBridgeLvglSdl
{
  static lv_indev_data_t m_IndevData;
  static SDL_Renderer *mp_SdlRenderer;

public:
  DisplayBridgeLvglSdl(void);

  void setRenderer(SDL_Renderer *p_Renderer);
  
  static void flushCallback(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
  static void readTouchpanelCallback(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);  

  void setLvIndevData(uint16_t x, uint16_t y, lv_indev_state_t state);
};

#endif // DISPLAY_BRIDGE_LVGL_SDL_H

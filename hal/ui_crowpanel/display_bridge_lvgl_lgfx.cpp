#include "display_bridge_lvgl_lgfx.h"


DisplayLGFX *DisplayBridgeLvglLgfx::mp_DisplayLGFX;


DisplayBridgeLvglLgfx::DisplayBridgeLvglLgfx(DisplayLGFX *p_DisplayLGFX)
{
  mp_DisplayLGFX = p_DisplayLGFX;
}


void DisplayBridgeLvglLgfx::flushCallback(lv_display_t *disp, const lv_area_t *area, uint8_t * px_map)
{
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);

  if(mp_DisplayLGFX != nullptr)
  {
    lv_draw_sw_rgb565_swap(px_map, w*h);
    mp_DisplayLGFX->pushImageDMA(area->x1, area->y1, w, h, (uint16_t*)px_map);
  }

  if(disp != nullptr)
  {
    lv_disp_flush_ready(disp);
  }
}



void DisplayBridgeLvglLgfx::readTouchpanelCallback(lv_indev_t *indev_driver, lv_indev_data_t *data)
{
  uint16_t u16_TouchX;
  uint16_t u16_TouchY;
  bool b_Touched;

  if((mp_DisplayLGFX == nullptr) || (data == nullptr))
  {
    return;
  }

  b_Touched = mp_DisplayLGFX->getTouch( &u16_TouchX, &u16_TouchY);
  if(!b_Touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = u16_TouchX;
    data->point.y = u16_TouchY;
  }
}

#include "display_bridge_lvgl_lgfx.h"


DisplayLGFX *DisplayBridgeLvglLgfx::mp_DisplayLGFX;


DisplayBridgeLvglLgfx::DisplayBridgeLvglLgfx(DisplayLGFX *p_DisplayLGFX)
{
  mp_DisplayLGFX = p_DisplayLGFX;
}

/* Display flushing */
void DisplayBridgeLvglLgfx::flushCallback(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = ( area->x2 - area->x1 + 1 );
  uint32_t h = ( area->y2 - area->y1 + 1 );

  if(mp_DisplayLGFX != nullptr)
  {
    mp_DisplayLGFX->pushImageDMA(area->x1, area->y1, w, h,(lgfx::rgb565_t*)&color_p->full);
  }

  lv_disp_flush_ready( disp );
}



void DisplayBridgeLvglLgfx::readTouchpanelCallback(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  uint16_t u16_TouchX;
  uint16_t u16_TouchY;
  bool b_Touched;

  if(mp_DisplayLGFX == nullptr)
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

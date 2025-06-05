

#include <iostream>

#include "display_bridge_lvgl_sdl.h"


//DisplayLGFX *DisplayBridgeLvglLgfx::mp_DisplayLGFX;
lv_indev_data_t DisplayBridgeLvglSdl::m_IndevData;
SDL_Renderer *DisplayBridgeLvglSdl::mp_SdlRenderer = nullptr;


DisplayBridgeLvglSdl::DisplayBridgeLvglSdl(void)
{
//  mp_DisplayLGFX = p_DisplayLGFX;
}


void DisplayBridgeLvglSdl::setRenderer(SDL_Renderer *p_Renderer)
{
  mp_SdlRenderer = p_Renderer;
}



/* Display flushing */
void DisplayBridgeLvglSdl::flushCallback(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
//  std::cout << "flushCallback(" << area->x1 << ", " << area->y1 << ", " << area->x2 << ", " << area->y2 << ")" << std::endl;  
  if(mp_SdlRenderer != nullptr)
  {
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    // Create an SDL surface from the LVGL buffer
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom((void*)color_p, w, h, 16, w * sizeof(lv_color_t), SDL_PIXELFORMAT_RGB565);

    if (surface) 
    {
      SDL_Texture* texture = SDL_CreateTextureFromSurface(mp_SdlRenderer, surface);
      if (texture) 
      {
        SDL_Rect dstRect = { area->x1, area->y1, (int)w, (int)h };
        SDL_RenderCopy(mp_SdlRenderer, texture, nullptr, &dstRect);
        SDL_DestroyTexture(texture);
      }
      SDL_FreeSurface(surface);
    }
    SDL_RenderPresent(mp_SdlRenderer);
  }

  lv_disp_flush_ready( disp );
}



void DisplayBridgeLvglSdl::readTouchpanelCallback(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  data->state = m_IndevData.state;

  /*Set the coordinates*/
  data->point.x = m_IndevData.point.x;
  data->point.y = m_IndevData.point.y;
}


void DisplayBridgeLvglSdl::setLvIndevData(uint16_t x, uint16_t y, lv_indev_state_t state)
{
  m_IndevData.point.x = x;
  m_IndevData.point.y = y;
  m_IndevData.state = state;
}
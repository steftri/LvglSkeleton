/**************************LVGL and UI************************
if you want to use the LVGL demo. you need to include <demos/lv_demos.h> and <examples/lv_examples.h>. 
if not, please do not include it. It will waste your Flash space.
**************************************************************/

#include "ui.h"


uint8_t Ui::mau8_DispDrawBuf1[screenWidth * screenHeight / 10] __attribute__((aligned(32)));
uint8_t Ui::mau8_DispDrawBuf2[screenWidth * screenHeight / 10] __attribute__((aligned(32)));


Ui::Ui()
  : m_DisplayBridge(&m_DisplayLGFX)
{
}


void Ui::initDisplay(void)
{
  m_DisplayLGFX.setBrightness(0);
  m_DisplayLGFX.init_without_reset();
}



void Ui::setup()
{
  lv_init();

  initDisplay();
  m_DisplayLGFX.setRotation(1);

  /*------------------------------------
   * Create a display and set a flush_cb
   * -----------------------------------*/
  lv_display_t * disp = lv_display_create(screenHeight, screenWidth);
  lv_display_set_flush_cb(disp, DisplayBridgeLvglLgfx::flushCallback);

  /* Two buffers for partial rendering
   * In flush_cb DMA or similar hardware should be used to update the display in the background.*/
  lv_display_set_buffers(disp, mau8_DispDrawBuf1, mau8_DispDrawBuf2, sizeof(mau8_DispDrawBuf1), LV_DISPLAY_RENDER_MODE_PARTIAL);

  /* Initialize the (dummy) input device driver */
  mp_IndevTouchpad = lv_indev_create();
  lv_indev_set_type(mp_IndevTouchpad, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(mp_IndevTouchpad, DisplayBridgeLvglLgfx::readTouchpanelCallback);

  lv_timer_handler();
  m_DisplayLGFX.setBrightness(255);
}


void Ui::loop()
{
  delay(5);
  lv_tick_inc(5);
  lv_timer_handler();
}



#include "display.h"


/**************************LVGL and UI************************
if you want to use the LVGL demo. you need to include <demos/lv_demos.h> and <examples/lv_examples.h>. 
if not, please do not include it. It will waste your Flash space.
**************************************************************/
#include <lvgl.h>
#include "../hal/crowpanel/hw_display_lgfx.h"
#include "../hal/crowpanel/display_bridge_lvgl_lgfx.h"



DisplayLGFX tft;

LV_ATTRIBUTE_MEM_ALIGN
static uint8_t disp_draw_buf1[screenWidth * screenHeight / 10];

LV_ATTRIBUTE_MEM_ALIGN
static uint8_t disp_draw_buf2[screenWidth * screenHeight / 10];

static lv_indev_t * indev_touchpad;

static DisplayBridgeLvglLgfx displayBridge(&tft);





Display::Display()
{
    // Constructor implementation (if needed)
}

void Display::setup()
{
  //Display Prepare
  tft.begin();
  //tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(200, 240);
  tft.print("booting...");  
  delay(1000);

  lv_init();

  /*------------------------------------
   * Create a display and set a flush_cb
   * -----------------------------------*/
  lv_display_t * disp = lv_display_create(screenHeight, screenWidth);
 // lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);
  lv_display_set_flush_cb(disp, DisplayBridgeLvglLgfx::flushCallback);

  /* Two buffers for partial rendering
   * In flush_cb DMA or similar hardware should be used to update the display in the background.*/
  lv_display_set_buffers(disp, disp_draw_buf1, disp_draw_buf2, sizeof(disp_draw_buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);


  /* Initialize the (dummy) input device driver */
  indev_touchpad = lv_indev_create();
  lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev_touchpad, DisplayBridgeLvglLgfx::readTouchpanelCallback);

  tft.setRotation(1);
}


void Display::loop()
{
  delay(5);
  lv_tick_inc(5);
  lv_timer_handler();
}



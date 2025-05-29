#ifdef ARDUINO 
#include <Arduino.h> 
#else
#include <iostream>
#include <inttypes.h>
#endif


/**************************LVGL and UI************************
if you want to use the LVGL demo. you need to include <demos/lv_demos.h> and <examples/lv_examples.h>. 
if not, please do not include it. It will waste your Flash space.
**************************************************************/
#include <lvgl.h>
#include "hw_display_lgfx.h"
#include "display_bridge_lvgl_lgfx.h"


#include "lv_demo_widgets.h"
/**************************LVGL and UI END************************/

DisplayLGFX tft;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t disp_draw_buf1[screenWidth * screenHeight / 10];
static lv_color_t disp_draw_buf2[screenWidth * screenHeight / 10];
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;

static DisplayBridgeLvglLgfx displayBridge(&tft);


void setup()
{
#ifdef ARDUINO
  Serial.begin(115200);
  Serial.println(APPLICATION_NAME);
  Serial.println(__DATE__ " " __TIME__);
#else
  std::cout << APPLICATION_NAME << std::endl;
  std::cout << __DATE__ " " __TIME__ << std::endl;
#endif

  //GPIO init
  pinMode(38, OUTPUT);
  digitalWrite(38, LOW);
  pinMode(17, OUTPUT);
  digitalWrite(17, LOW);
  pinMode(18, OUTPUT);
  digitalWrite(18, LOW);
  pinMode(42, OUTPUT);
  digitalWrite(42, LOW);


  //Display Prepare
  tft.begin();
  //tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(200, 240);
  tft.print("booting...");  
  delay(1000);

  lv_init();

  lv_disp_draw_buf_init(&draw_buf, disp_draw_buf1, disp_draw_buf2, screenWidth * screenHeight/10);
  /* Initialize the display */
  lv_disp_drv_init(&disp_drv);
  /* Change the following line to your display resolution */
  disp_drv.hor_res = screenHeight;  // swapped to match the display orientation
  disp_drv.ver_res = screenWidth;
  disp_drv.flush_cb = DisplayBridgeLvglLgfx::flushCallback;
  disp_drv.full_refresh = 1;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /* Initialize the (dummy) input device driver */

  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = DisplayBridgeLvglLgfx::readTouchpanelCallback;
  lv_indev_drv_register(&indev_drv);

  tft.setRotation(1);
  lv_demo_widgets();    // LVGL demo
  
  Serial.println( "Setup done" );

}

void loop()
{
    lv_timer_handler();
    delay(5);
}



#ifndef ARDUINO  
int main(int argc, char *argv[])
{
  setup();
  while(true)
    loop();
  return 0;
}
#endif

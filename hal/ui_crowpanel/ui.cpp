/**************************LVGL and UI************************
if you want to use the LVGL demo. you need to include <demos/lv_demos.h> and <examples/lv_examples.h>. 
if not, please do not include it. It will waste your Flash space.
**************************************************************/
#include <Arduino.h>
#include <driver/ledc.h> // Include LEDC driver for ledc_timer_config_t and related functions

#include "ui.h"


uint8_t Ui::mau8_DispDrawBuf1[screenWidth * screenHeight / 10] __attribute__((aligned(32)));
uint8_t Ui::mau8_DispDrawBuf2[screenWidth * screenHeight / 10] __attribute__((aligned(32)));


void lv_log_print_g_cb(lv_log_level_t level, const char *buf)
{
  Serial.print("LVGL Log [");
  Serial.print(level);
  Serial.print("]: ");
  Serial.print(buf);
}


Ui::Ui()
  : m_DisplayBridge(&m_DisplayLGFX)
{
}



void Ui::initBacklight(void)
{
  ledc_timer_config_t LedcTimer = 
  {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .duty_resolution = LEDC_TIMER_13_BIT,
    .timer_num = LEDC_TIMER_0,
    .freq_hz = 1000,
    .clk_cfg = LEDC_AUTO_CLK
  };
  ledc_channel_config_t LedcChannel = 
  {
    .gpio_num = GPIO_NUM_2, // Backlight pin
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .intr_type = LEDC_INTR_DISABLE,
    .timer_sel = LEDC_TIMER_0,
    .duty = 0,
    .hpoint = 0
  };

  ledc_timer_config(&LedcTimer);
  ledc_channel_config(&LedcChannel);
}


void Ui::initDisplay(void)
{
  m_DisplayLGFX.setBrightness(0);
  m_DisplayLGFX.init_without_reset();
}


void Ui::setBrightness(uint8_t brightness)
{
  m_DisplayLGFX.setBrightness(brightness);
}


void Ui::setup()
{
  #if LV_USE_LOG != 0
  lv_log_register_print_cb(lv_log_print_g_cb);
  #endif

  initBacklight();
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

  // m_DisplayLGFX.setBrightness(255); 
}


void Ui::loop()
{
  static uint32_t lastUpdateTime = 0;
  uint32_t currentTime = millis();

  if (currentTime - lastUpdateTime >= 5) // Update every 5 milliseconds
  {
    lv_tick_inc(currentTime - lastUpdateTime); // Increment LVGL tick count
    lv_timer_handler();

    lastUpdateTime = currentTime;
  }
}



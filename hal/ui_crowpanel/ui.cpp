/**************************LVGL and UI************************
if you want to use the LVGL demo. you need to include <demos/lv_demos.h> and <examples/lv_examples.h>. 
if not, please do not include it. It will waste your Flash space.
**************************************************************/
#include <Arduino.h>

#include "ui.h"


uint8_t Ui::mau8_DispDrawBuf1[screenWidth * screenHeight / 10] __attribute__((aligned(32)));
uint8_t Ui::mau8_DispDrawBuf2[screenWidth * screenHeight / 10] __attribute__((aligned(32)));


void lv_log_print_g_cb(lv_log_level_t level, const char *buf)
{
  Serial.print("LVGL Log [");
  Serial.print(level);
  Serial.print("]: ");
  Serial.println(buf);
}


Ui::Ui()
  : m_DisplayBridge(&m_DisplayLGFX)
#ifdef USE_FREERTOS  
  , mp_lvglTaskHandle(nullptr)
#endif  
{
}


#ifdef USE_FREERTOS
void Ui::lvglTask(void *pvParameters)
{
  Ui *p_ui = static_cast<Ui *>(pvParameters);
  if(p_ui == nullptr)
  {
    Serial.println("Error: UI interface is null");
    vTaskDelete(nullptr);
    return;
  }
  
  Serial.println("LVGL Task started");

  p_ui->setBrightness(255);
  while (true)
  {
    lv_tick_inc(5);
    lv_timer_handler();
    vTaskDelay(pdMS_TO_TICKS(5)); // 5 ms Delay für regelmäßige Updates
  }
}
#endif


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

#ifdef USE_FREERTOS
  // Create a FreeRTOS task for LVGL updates
  mp_lvglTaskHandle = xTaskCreateStatic(
     lvglTask,         // Task function
     "LVGL",           // Task name
     LVGL_TASK_STACK_SIZE, // Stack size
     this,                  // Parameters
     tskIDLE_PRIORITY + 1,     // Priority
     m_lvglTaskStack,  // Task handle
     &m_lvglTaskBuffer // Static task buffer
  );

#else
  m_DisplayLGFX.setBrightness(255);
#endif  
}


void Ui::loop()
{
  // If using FreeRTOS, the UI updates are handled in the lvglTask; 
  // otherwise, call the lvgl update functions directly
#ifndef USE_FREERTOS    
  delay(5);
  lv_tick_inc(5);
  lv_timer_handler();
#endif  
}



#ifndef UI_H
#define UI_H

#include <inttypes.h>
#include <lvgl.h>

#ifdef USE_FREERTOS
#include <freertos/FreeRTOS.h>
#endif


#include "hw_display_lgfx.h"
#include "display_bridge_lvgl_lgfx.h"

#include "interfaces/ui_interface.h"



#ifdef USE_FREERTOS
static const size_t LVGL_TASK_STACK_SIZE = 8192; // Stack size for the task
#endif



class Ui : public UiInterface
{ 
  DisplayBridgeLvglLgfx m_DisplayBridge;

  static uint8_t mau8_DispDrawBuf1[screenWidth * screenHeight / 10];
  static uint8_t mau8_DispDrawBuf2[screenWidth * screenHeight / 10];

  lv_indev_t *mp_IndevTouchpad;

#ifdef USE_FREERTOS
  TaskHandle_t mp_lvglTaskHandle;
  StaticTask_t m_lvglTaskBuffer;
  StackType_t m_lvglTaskStack[ LVGL_TASK_STACK_SIZE ];
  static void lvglTask(void *pvParameters);
#endif

public:
  Ui();

  // Initialize the display
  void setup() override;

  // Update the display
  void loop() override; 

  void setBrightness(uint8_t brightness);

protected:
  DisplayLGFX m_DisplayLGFX;

  virtual void initDisplay(void);

private: 
  void initBacklight(void);
};


#endif // UI_H
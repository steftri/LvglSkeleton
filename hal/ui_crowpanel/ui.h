#ifndef UI_H
#define UI_H

#include <lvgl.h>

#include "hw_display_lgfx.h"
#include "display_bridge_lvgl_lgfx.h"

#include "interfaces/ui_interface.h"


class Ui : public UiInterface
{ 
  DisplayBridgeLvglLgfx m_DisplayBridge;

  static uint8_t mau8_DispDrawBuf1[screenWidth * screenHeight / 10];
  static uint8_t mau8_DispDrawBuf2[screenWidth * screenHeight / 10];

  lv_indev_t *mp_IndevTouchpad;

public:
  Ui();

  // Initialize the display
  void setup() override;

  // Update the display
  void loop() override; 

protected:
  DisplayLGFX m_DisplayLGFX;

  virtual void initDisplay(void);
};


#endif // UI_H
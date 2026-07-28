#ifndef HW_DISPLAY_LGFX_H
#define HW_DISPLAY_LGFX_H


#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <driver/i2c.h>

static constexpr uint16_t screenWidth = 800;
static constexpr uint16_t screenHeight = 480;


class DisplayLGFX : public lgfx::LGFX_Device
{
  lgfx::Bus_RGB     m_BusInstance;
  lgfx::Panel_RGB   m_PanelInstance;
  lgfx::Light_PWM   m_BacklightInstance;
  lgfx::Touch_GT911 m_TouchInstance;

public:
  DisplayLGFX(void);
};

#endif

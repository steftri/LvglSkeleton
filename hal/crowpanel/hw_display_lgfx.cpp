#include "hw_display_lgfx.h"

DisplayLGFX::DisplayLGFX(void)
{
  {
    auto PanelConfig = m_PanelInstance.config();
    PanelConfig.memory_width = screenWidth;
    PanelConfig.memory_height = screenHeight;
    PanelConfig.panel_width = screenWidth;
    PanelConfig.panel_height = screenHeight;
    PanelConfig.offset_x = 0;
    PanelConfig.offset_y = 0;
    m_PanelInstance.config(PanelConfig);
  }

  {
    auto BusConfig = m_BusInstance.config();
    BusConfig.panel = &m_PanelInstance;

    BusConfig.pin_d0 = GPIO_NUM_8;  // B0
    BusConfig.pin_d1 = GPIO_NUM_3;  // B1
    BusConfig.pin_d2 = GPIO_NUM_46; // B2
    BusConfig.pin_d3 = GPIO_NUM_9;  // B3
    BusConfig.pin_d4 = GPIO_NUM_1;  // B4

    BusConfig.pin_d5 = GPIO_NUM_5;  // G0
    BusConfig.pin_d6 = GPIO_NUM_6;  // G1
    BusConfig.pin_d7 = GPIO_NUM_7;  // G2
    BusConfig.pin_d8 = GPIO_NUM_15; // G3
    BusConfig.pin_d9 = GPIO_NUM_16; // G4
    BusConfig.pin_d10 = GPIO_NUM_4; // G5

    BusConfig.pin_d11 = GPIO_NUM_45; // R0
    BusConfig.pin_d12 = GPIO_NUM_48; // R1
    BusConfig.pin_d13 = GPIO_NUM_47; // R2
    BusConfig.pin_d14 = GPIO_NUM_21; // R3
    BusConfig.pin_d15 = GPIO_NUM_14; // R4

    BusConfig.pin_henable = GPIO_NUM_40;
    BusConfig.pin_vsync = GPIO_NUM_41;
    BusConfig.pin_hsync = GPIO_NUM_39;
    BusConfig.pin_pclk = GPIO_NUM_0;
    BusConfig.freq_write = 12000000;

    BusConfig.hsync_polarity    = 0;
    BusConfig.hsync_front_porch = 8;
    BusConfig.hsync_pulse_width = 4;
    BusConfig.hsync_back_porch  = 43;
    
    BusConfig.vsync_polarity    = 0;
    BusConfig.vsync_front_porch = 8;
    BusConfig.vsync_pulse_width = 4;
    BusConfig.vsync_back_porch  = 12;

    BusConfig.pclk_active_neg = 1;
    BusConfig.de_idle_high = 0;
    BusConfig.pclk_idle_high = 0;

    m_BusInstance.config(BusConfig);
    m_PanelInstance.setBus(&m_BusInstance);
  }

  {
    auto BacklightConfig = m_BacklightInstance.config();
    BacklightConfig.pin_bl = GPIO_NUM_2;
    m_BacklightInstance.config(BacklightConfig);
    m_PanelInstance.light(&m_BacklightInstance);
  }

  {
    auto TouchConfig = m_TouchInstance.config();
    TouchConfig.x_min      = 0;
    TouchConfig.x_max      = 799;
    TouchConfig.y_min      = 0;
    TouchConfig.y_max      = 479;
    TouchConfig.pin_int    = -1;
    TouchConfig.pin_rst    = -1;
    TouchConfig.bus_shared = true;
    TouchConfig.offset_rotation = 0;
    TouchConfig.i2c_port   = I2C_NUM_1;
    TouchConfig.pin_sda    = GPIO_NUM_19;
    TouchConfig.pin_scl    = GPIO_NUM_20;
    TouchConfig.freq       = 400000;
    TouchConfig.i2c_addr   = 0x14;
    m_TouchInstance.config(TouchConfig);
    m_PanelInstance.setTouch(&m_TouchInstance);
  }
  setPanel(&m_PanelInstance);
}

#include "lightstripe_hal.h"


LightstripeHal::LightstripeHal(uint8_t u8_Pin, uint16_t u16_NumPixels)
    : m_NeoPixel(u16_NumPixels, u8_Pin, NEO_GRB + NEO_KHZ800)
    , mu16_NumPixels(u16_NumPixels)
    , mu8_Pin(u8_Pin)
{
}



void LightstripeHal::setup(void)
{
  m_NeoPixel.begin(); // Initialize the NeoPixel library
  m_NeoPixel.show();  // Turn off all pixels
}


void LightstripeHal::enable(void)
{
  m_NeoPixel.setBrightness(63); // Set brightness to maximum
}

void LightstripeHal::disable(void)
{
  m_NeoPixel.setBrightness(0); // Set brightness to zero
}

void LightstripeHal::setPixelColor(uint16_t u16_Index, uint32_t u32_Color)
{
  if (u16_Index < mu16_NumPixels)
  {
    m_NeoPixel.setPixelColor(u16_Index, u32_Color);
  }
}

void LightstripeHal::show(void)
{
  m_NeoPixel.show(); // Update the strip to display the new colors
}

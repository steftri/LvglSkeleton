#include "Arduino.h"
#include "lightstripe_hal.h"


LightstripeHal::LightstripeHal(uint8_t u8_Pin, uint16_t u16_NumPixels)
    : mu8_Pin(u8_Pin)
{
  (void)u16_NumPixels; // Unused parameter
}



void LightstripeHal::setup(void)
{
  pinMode(mu8_Pin, OUTPUT);
  digitalWrite(mu8_Pin, LOW); // Ensure the pin is low initially
}




void LightstripeHal::enable(void)
{
  digitalWrite(mu8_Pin, HIGH);
}

void LightstripeHal::disable(void)
{
  digitalWrite(mu8_Pin, LOW);
}

void LightstripeHal::setPixelColor(uint16_t u16_Index, uint32_t u32_Color)
{
}

void LightstripeHal::show(void)
{
}

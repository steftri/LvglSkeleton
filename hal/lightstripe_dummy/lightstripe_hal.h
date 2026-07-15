#ifndef LIGHTSTRIPE_HAL_H
#define LIGHTSTRIPE_HAL_H

#include "interfaces/lightstripe_hal_interface.h"


class LightstripeHal : public LightstripeHalInterface
{
  uint16_t mu16_NumPixels;
  uint8_t mu8_Pin;

public:
  LightstripeHal(uint8_t u8_Pin, uint16_t u16_NumPixels);
  ~LightstripeHal() = default;

  void setup() override;  

  void enable() override;
  void disable() override;

  void setPixelColor(uint16_t u16_Index, uint32_t u32_Color) override;
  void show() override;
};


#endif  // LIGHTSTRIPE_HAL_H
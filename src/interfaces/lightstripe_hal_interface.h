#ifndef LIGHTSTRIPE_HAL_INTERFACE_H
#define LIGHTSTRIPE_HAL_INTERFACE_H

#include <inttypes.h>


class LightstripeHalInterface
{
public:
  virtual ~LightstripeHalInterface() = default;

  virtual void setup() = 0;

  virtual void enable() = 0;
  virtual void disable() = 0;

  virtual void setPixelColor(uint16_t u16_Index, uint32_t u32_Color) = 0;
  virtual void show() = 0;
};


#endif  // LIGHTSTRIPE_HAL_INTERFACE_H
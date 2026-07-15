#ifndef SYSTEM_DATA_H
#define SYSTEM_DATA_H

#include <inttypes.h>

#include "data.h"


class SystemData : public Data
{
public:
  enum class EField : uint8_t
  {
    SocPercent      = 0,
    ChargingCurrent = 1,
    RemainingTime   = 2
  };

private:
  float    mf32_SocPercent;    ///< State of charge in percent (0–100)
  float    mf32_ChargingCurrentA; ///< Charging current in amperes
  uint16_t mu16_RemainingTimeMin; ///< Estimated remaining charging time in minutes (-1 = unknown)

public:
  SystemData();
  ~SystemData() = default;

  void setSocPercent(float f32_SocPercent);
  float getSocPercent(void) const;

  void  setChargingCurrentA(float f32_ChargingCurrentA);
  float getChargingCurrentA(void) const;

  void setRemainingTimeMin(uint16_t u16_RemainingTimeMin);
  uint16_t  getRemainingTimeMin(void) const;
};


#endif // SYSTEM_DATA_H

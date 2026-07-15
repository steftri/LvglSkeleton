#ifndef SYSTEM_DATA_H
#define SYSTEM_DATA_H

#include <inttypes.h>

#include "data.h"


class SystemData : public Data
{
public:
  enum class EField : uint8_t
  {
    SocPercent          = 0,
    ChargingCurrent     = 1,
    DurationMin         = 2,
    PowerConsumptionKWh = 3,
    ChargingSpeedKW     = 4
  };

private:
  float    mf32_SocPercent;         ///< State of charge in percent (0–100)
  float    mf32_ChargingCurrentA;    ///< Charging current in amperes
  uint16_t mu16_DurationMin;         ///< Charging duration in minutes
  float    mf32_PowerConsumptionKWh; ///< Power consumption in kWh
  float    mf32_ChargingSpeedKW;     ///< Charging speed in kW

public:
  SystemData();
  ~SystemData() = default;

  void setSocPercent(float f32_SocPercent);
  float getSocPercent(void) const;

  void  setChargingCurrentA(float f32_ChargingCurrentA);
  float getChargingCurrentA(void) const;

  void     setDurationMin(uint16_t u16_DurationMin);
  uint16_t getDurationMin(void) const;

  void  setPowerConsumptionKWh(float f32_PowerConsumptionKWh);
  float getPowerConsumptionKWh(void) const;

  void  setChargingSpeedKW(float f32_ChargingSpeedKW);
  float getChargingSpeedKW(void) const;
};


#endif // SYSTEM_DATA_H

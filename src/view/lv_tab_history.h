#ifndef LV_TAB_HISTORY_H
#define LV_TAB_HISTORY_H

#include <lvgl.h>


class LvTabHistory
{
private:
  lv_obj_t *mp_BatteryArc;
  lv_obj_t *mp_SocLabel;
  lv_obj_t *mp_CurrentValueLabel;
  lv_obj_t *mp_DurationLabel;
  lv_obj_t *mp_PowerConsumptionLabel;
  lv_obj_t *mp_ChargingSpeedLabel;

public:
  LvTabHistory();
  ~LvTabHistory() = default;

  void setup(lv_obj_t *p_ParentTab);

  void updateChargingState(float f32_SocPercent);
  void updateChargingCurrent(float f32_CurrentAmpere);
  void updateDuration(uint16_t u16_DurationMin);
  void updatePowerConsumption(float f32_PowerConsumptionKWh);
  void updateChargingSpeed(float f32_ChargingSpeedKW);
};


#endif // LV_TAB_HISTORY_H

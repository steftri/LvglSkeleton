#ifndef LV_TAB_HISTORY_H
#define LV_TAB_HISTORY_H

#include <lvgl.h>


class LvTabHistory
{
private:
  lv_obj_t *mp_BatteryArc;
  lv_obj_t *mp_SocLabel;
  lv_obj_t *mp_CurrentValueLabel;
  lv_obj_t *mp_RemainingTimeLabel;

public:
  LvTabHistory();
  ~LvTabHistory() = default;

  void setup(lv_obj_t *p_ParentTab);

  void updateChargingState(int i_SocPercent);
  void updateChargingCurrent(float f_CurrentAmpere);
  void updateRemainingTime(int i_RemainingMinutes);
};


#endif // LV_TAB_HISTORY_H

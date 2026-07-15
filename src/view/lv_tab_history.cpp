#include "lv_tab_history.h"
#include <lvgl.h>
#include <stdio.h>

LvTabHistory::LvTabHistory()
    : mp_BatteryArc(nullptr)
    , mp_SocLabel(nullptr)
    , mp_CurrentValueLabel(nullptr)
    , mp_DurationLabel(nullptr)
    , mp_PowerConsumptionLabel(nullptr)
    , mp_ChargingSpeedLabel(nullptr)
{
}

void LvTabHistory::setup(lv_obj_t *p_ParentTab)
{
  // No layout on the tab itself – the wrapper will be centered via lv_obj_center()
  lv_obj_remove_flag(p_ParentTab, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_pad_all(p_ParentTab, 0, 0);

  // === WRAPPER: Arc + cards tightly grouped and centered on the screen ===
  lv_obj_t *p_ContentWrapper = lv_obj_create(p_ParentTab);
  lv_obj_remove_style_all(p_ContentWrapper);
  lv_obj_set_size(p_ContentWrapper, lv_pct(95), LV_SIZE_CONTENT);
  lv_obj_center(p_ContentWrapper);
  lv_obj_set_flex_flow(p_ContentWrapper, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(p_ContentWrapper, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_row(p_ContentWrapper, 8, 0);

  // --- State of charge arc ---
  mp_BatteryArc = lv_arc_create(p_ContentWrapper);
  lv_obj_set_size(mp_BatteryArc, 280, 280);
  lv_arc_set_rotation(mp_BatteryArc, 135);
  lv_arc_set_bg_angles(mp_BatteryArc, 0, 270);
  lv_arc_set_range(mp_BatteryArc, 0, 100);
  lv_arc_set_value(mp_BatteryArc, 0);
  lv_obj_remove_flag(mp_BatteryArc, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_style_bg_opa(mp_BatteryArc, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_arc_color(mp_BatteryArc, lv_color_hex(0x2ECC71), LV_PART_INDICATOR);
  lv_obj_set_style_arc_width(mp_BatteryArc, 20, LV_PART_INDICATOR);
  lv_obj_set_style_arc_color(mp_BatteryArc, lv_color_hex(0x2C3E50), LV_PART_MAIN);
  lv_obj_set_style_arc_width(mp_BatteryArc, 20, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(mp_BatteryArc, LV_OPA_TRANSP, LV_PART_KNOB);
  lv_obj_set_style_shadow_opa(mp_BatteryArc, LV_OPA_TRANSP, LV_PART_KNOB);

  // Labels as children of the arc so they stay centered inside it
  mp_SocLabel = lv_label_create(mp_BatteryArc);
  lv_label_set_text(mp_SocLabel, "-- %");
  lv_obj_set_style_text_font(mp_SocLabel, &lv_font_montserrat_32, 0);
  lv_obj_align(mp_SocLabel, LV_ALIGN_CENTER, 0, -10);

  lv_obj_t *p_SocSubLabel = lv_label_create(mp_BatteryArc);
  lv_label_set_text(p_SocSubLabel, "State of charge");
  lv_obj_set_style_text_color(p_SocSubLabel, lv_color_hex(0x95A5A6), 0);
  lv_obj_align(p_SocSubLabel, LV_ALIGN_CENTER, 0, 22);

  // === BOTTOM: Two info cards side by side ===
  lv_obj_t *p_BottomRow = lv_obj_create(p_ContentWrapper);
  lv_obj_remove_style_all(p_BottomRow);
  lv_obj_set_size(p_BottomRow, lv_pct(100), LV_SIZE_CONTENT);
  lv_obj_set_flex_flow(p_BottomRow, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(p_BottomRow, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_column(p_BottomRow, 12, 0);

  // --- Charging current card (left) ---
  lv_obj_t *p_CurrentCard = lv_obj_create(p_BottomRow);
  lv_obj_set_style_flex_grow(p_CurrentCard, 1, 0);
  lv_obj_set_height(p_CurrentCard, LV_SIZE_CONTENT);
  lv_obj_set_flex_flow(p_CurrentCard, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(p_CurrentCard, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  lv_obj_t *p_CurrentTitle = lv_label_create(p_CurrentCard);
  lv_label_set_text(p_CurrentTitle, "Charging current");
  lv_obj_set_style_text_color(p_CurrentTitle, lv_color_hex(0x95A5A6), 0);

  mp_CurrentValueLabel = lv_label_create(p_CurrentCard);
  lv_label_set_text(mp_CurrentValueLabel, "-- A");
  lv_obj_set_style_text_font(mp_CurrentValueLabel, &lv_font_montserrat_24, 0);
  lv_obj_set_style_text_color(mp_CurrentValueLabel, lv_color_white(), 0);

  // --- Charging speed card (right) ---
  lv_obj_t *p_SpeedCard = lv_obj_create(p_BottomRow);
  lv_obj_set_style_flex_grow(p_SpeedCard, 1, 0);
  lv_obj_set_height(p_SpeedCard, LV_SIZE_CONTENT);
  lv_obj_set_flex_flow(p_SpeedCard, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(p_SpeedCard, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  lv_obj_t *p_SpeedTitle = lv_label_create(p_SpeedCard);
  lv_label_set_text(p_SpeedTitle, "Charging speed");
  lv_obj_set_style_text_color(p_SpeedTitle, lv_color_hex(0x95A5A6), 0);

  mp_ChargingSpeedLabel = lv_label_create(p_SpeedCard);
  lv_label_set_text(mp_ChargingSpeedLabel, "-- kW");
  lv_obj_set_style_text_font(mp_ChargingSpeedLabel, &lv_font_montserrat_24, 0);
  lv_obj_set_style_text_color(mp_ChargingSpeedLabel, lv_color_white(), 0);

  // === SECOND ROW: Power consumption and Charging speed ===
  lv_obj_t *p_BottomRow2 = lv_obj_create(p_ContentWrapper);
  lv_obj_remove_style_all(p_BottomRow2);
  lv_obj_set_size(p_BottomRow2, lv_pct(100), LV_SIZE_CONTENT);
  lv_obj_set_flex_flow(p_BottomRow2, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(p_BottomRow2, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_column(p_BottomRow2, 12, 0);

  // --- Power consumption card (left) ---
  lv_obj_t *p_PowerCard = lv_obj_create(p_BottomRow2);
  lv_obj_set_style_flex_grow(p_PowerCard, 1, 0);
  lv_obj_set_height(p_PowerCard, LV_SIZE_CONTENT);
  lv_obj_set_flex_flow(p_PowerCard, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(p_PowerCard, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  lv_obj_t *p_PowerTitle = lv_label_create(p_PowerCard);
  lv_label_set_text(p_PowerTitle, "Power consumption");
  lv_obj_set_style_text_color(p_PowerTitle, lv_color_hex(0x95A5A6), 0);

  mp_PowerConsumptionLabel = lv_label_create(p_PowerCard);
  lv_label_set_text(mp_PowerConsumptionLabel, "-- kWh");
  lv_obj_set_style_text_font(mp_PowerConsumptionLabel, &lv_font_montserrat_24, 0);
  lv_obj_set_style_text_color(mp_PowerConsumptionLabel, lv_color_white(), 0);

  // --- Duration card (right) ---
  lv_obj_t *p_DurationCard = lv_obj_create(p_BottomRow2);
  lv_obj_set_style_flex_grow(p_DurationCard, 1, 0);
  lv_obj_set_height(p_DurationCard, LV_SIZE_CONTENT);
  lv_obj_set_flex_flow(p_DurationCard, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(p_DurationCard, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);  

  lv_obj_t *p_DurationTitle = lv_label_create(p_DurationCard);
  lv_label_set_text(p_DurationTitle, "Duration");
  lv_obj_set_style_text_color(p_DurationTitle, lv_color_hex(0x95A5A6), 0);

  mp_DurationLabel = lv_label_create(p_DurationCard);
  lv_label_set_text(mp_DurationLabel, "-- min");
  lv_obj_set_style_text_font(mp_DurationLabel, &lv_font_montserrat_24, 0);
  lv_obj_set_style_text_color(mp_DurationLabel, lv_color_white(), 0);
}

void LvTabHistory::updateChargingState(float f32_SocPercent)
{
  if (mp_BatteryArc == nullptr || mp_SocLabel == nullptr)
    return;

  if (f32_SocPercent < 0.0f)   f32_SocPercent = 0.0f;
  if (f32_SocPercent > 100.0f) f32_SocPercent = 100.0f;

  lv_arc_set_value(mp_BatteryArc, static_cast<int>(f32_SocPercent+0.5f));

  lv_color_t arcColor;
  if (f32_SocPercent <= 20.0f)
    arcColor = lv_color_hex(0xE74C3C); // rot
  else if (f32_SocPercent <= 60.0f)
    arcColor = lv_color_hex(0xF1C40F); // gelb
  else
    arcColor = lv_color_hex(0x2ECC71); // gruen

  lv_obj_set_style_arc_color(mp_BatteryArc, arcColor, LV_PART_INDICATOR);
  lv_obj_set_style_text_color(mp_SocLabel, arcColor, 0);

  char ac_Buf[8];
  snprintf(ac_Buf, sizeof(ac_Buf), "%d %%", static_cast<int>(f32_SocPercent+0.5f));
  lv_label_set_text(mp_SocLabel, ac_Buf);
}

void LvTabHistory::updateChargingCurrent(float f32_ChargingCurrentA)
{
  if (mp_CurrentValueLabel == nullptr)
    return;

  char ac_Buf[16];
  snprintf(ac_Buf, sizeof(ac_Buf), "%.1f A", f32_ChargingCurrentA);
  lv_label_set_text(mp_CurrentValueLabel, ac_Buf);
}

void LvTabHistory::updateDuration(uint16_t u16_DurationMin)
{
  if (mp_DurationLabel == nullptr)
    return;

  char ac_Buf[16];
  if (u16_DurationMin >= 60)
  {
    int i_Hours = u16_DurationMin / 60;
    int i_Mins  = u16_DurationMin % 60;
    snprintf(ac_Buf, sizeof(ac_Buf), "%d:%02d h", i_Hours, i_Mins);
  }
  else
  {
    snprintf(ac_Buf, sizeof(ac_Buf), "%d min", u16_DurationMin);
  }
  lv_label_set_text(mp_DurationLabel, ac_Buf);
}

void LvTabHistory::updatePowerConsumption(float f32_PowerConsumptionKWh)
{
  if (mp_PowerConsumptionLabel == nullptr)
    return;

  char ac_Buf[16];
  snprintf(ac_Buf, sizeof(ac_Buf), "%.2f kWh", f32_PowerConsumptionKWh);
  lv_label_set_text(mp_PowerConsumptionLabel, ac_Buf);
}

void LvTabHistory::updateChargingSpeed(float f32_ChargingSpeedKW)
{
  if (mp_ChargingSpeedLabel == nullptr)
    return;

  char ac_Buf[16];
  snprintf(ac_Buf, sizeof(ac_Buf), "%.2f kW", f32_ChargingSpeedKW);
  lv_label_set_text(mp_ChargingSpeedLabel, ac_Buf);
}

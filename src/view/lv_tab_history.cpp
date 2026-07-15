#include "lv_tab_history.h"
#include <lvgl.h>
#include <stdio.h>

LvTabHistory::LvTabHistory()
    : mp_BatteryArc(nullptr)
    , mp_SocLabel(nullptr)
    , mp_CurrentValueLabel(nullptr)
    , mp_RemainingTimeLabel(nullptr)
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
  lv_obj_set_size(p_ContentWrapper, lv_pct(90), LV_SIZE_CONTENT);
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
  lv_obj_set_style_bg_color(p_CurrentCard, lv_color_hex(0x0D231A), 0);
  lv_obj_set_style_border_color(p_CurrentCard, lv_color_hex(0x2ECC71), 0);
  lv_obj_set_style_border_width(p_CurrentCard, 2, 0);
  lv_obj_set_style_radius(p_CurrentCard, 12, 0);
  lv_obj_set_style_pad_all(p_CurrentCard, 14, 0);
  lv_obj_set_flex_flow(p_CurrentCard, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(p_CurrentCard, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  lv_obj_t *p_CurrentTitle = lv_label_create(p_CurrentCard);
  lv_label_set_text(p_CurrentTitle, "Charging current");
  lv_obj_set_style_text_color(p_CurrentTitle, lv_color_hex(0x95A5A6), 0);

  mp_CurrentValueLabel = lv_label_create(p_CurrentCard);
  lv_label_set_text(mp_CurrentValueLabel, "-- A");
  lv_obj_set_style_text_font(mp_CurrentValueLabel, &lv_font_montserrat_24, 0);
  lv_obj_set_style_text_color(mp_CurrentValueLabel, lv_color_hex(0x2ECC71), 0);

  // --- Remaining time card (right) ---
  lv_obj_t *p_TimeCard = lv_obj_create(p_BottomRow);
  lv_obj_set_style_flex_grow(p_TimeCard, 1, 0);
  lv_obj_set_height(p_TimeCard, LV_SIZE_CONTENT);
  lv_obj_set_style_bg_color(p_TimeCard, lv_color_hex(0x0D1523), 0);
  lv_obj_set_style_border_color(p_TimeCard, lv_color_hex(0x3498DB), 0);
  lv_obj_set_style_border_width(p_TimeCard, 2, 0);
  lv_obj_set_style_radius(p_TimeCard, 12, 0);
  lv_obj_set_style_pad_all(p_TimeCard, 14, 0);
  lv_obj_set_flex_flow(p_TimeCard, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(p_TimeCard, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  lv_obj_t *p_TimeTitle = lv_label_create(p_TimeCard);
  lv_label_set_text(p_TimeTitle, "Remaining time");
  lv_obj_set_style_text_color(p_TimeTitle, lv_color_hex(0x95A5A6), 0);

  mp_RemainingTimeLabel = lv_label_create(p_TimeCard);
  lv_label_set_text(mp_RemainingTimeLabel, "-- min");
  lv_obj_set_style_text_font(mp_RemainingTimeLabel, &lv_font_montserrat_24, 0);
  lv_obj_set_style_text_color(mp_RemainingTimeLabel, lv_color_hex(0x3498DB), 0);
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

void LvTabHistory::updateRemainingTime(int32_t s32_RemainingMinutes)
{
  if (mp_RemainingTimeLabel == nullptr)
    return;

  char ac_Buf[16];
  if (s32_RemainingMinutes == -1)
  {
    lv_label_set_text(mp_RemainingTimeLabel, "-- min");
  }
  else if (s32_RemainingMinutes >= 60)
  {
    int i_Hours = s32_RemainingMinutes / 60;
    int i_Mins  = s32_RemainingMinutes % 60;
    snprintf(ac_Buf, sizeof(ac_Buf), "%d:%02d h", i_Hours, i_Mins);
    lv_label_set_text(mp_RemainingTimeLabel, ac_Buf);
  }
  else
  {
    snprintf(ac_Buf, sizeof(ac_Buf), "%d min", s32_RemainingMinutes);
    lv_label_set_text(mp_RemainingTimeLabel, ac_Buf);
  }
}

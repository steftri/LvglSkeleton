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
  // Disable scrolling; use horizontal flex layout
  lv_obj_remove_flag(p_ParentTab, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_flex_flow(p_ParentTab, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(p_ParentTab, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_all(p_ParentTab, 16, 0);
  lv_obj_set_style_pad_column(p_ParentTab, 20, 0);

  // === LEFT: Arc gauge container ===
  lv_obj_t *p_ArcContainer = lv_obj_create(p_ParentTab);
  lv_obj_remove_style_all(p_ArcContainer);
  lv_obj_set_size(p_ArcContainer, 280, lv_pct(100));

  mp_BatteryArc = lv_arc_create(p_ArcContainer);
  lv_obj_set_size(mp_BatteryArc, 220, 220);
  lv_obj_center(mp_BatteryArc);
  lv_arc_set_rotation(mp_BatteryArc, 135);
  lv_arc_set_bg_angles(mp_BatteryArc, 0, 270);
  lv_arc_set_range(mp_BatteryArc, 0, 100);
  lv_arc_set_value(mp_BatteryArc, 0);
  lv_obj_remove_flag(mp_BatteryArc, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_style_bg_opa(mp_BatteryArc, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_arc_color(mp_BatteryArc, lv_color_hex(0x2ECC71), LV_PART_INDICATOR);
  lv_obj_set_style_arc_width(mp_BatteryArc, 18, LV_PART_INDICATOR);
  lv_obj_set_style_arc_color(mp_BatteryArc, lv_color_hex(0x2C3E50), LV_PART_MAIN);
  lv_obj_set_style_arc_width(mp_BatteryArc, 18, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(mp_BatteryArc, LV_OPA_TRANSP, LV_PART_KNOB);
  lv_obj_set_style_shadow_opa(mp_BatteryArc, LV_OPA_TRANSP, LV_PART_KNOB);

  // Labels as children of the arc so they stay centered inside it
  mp_SocLabel = lv_label_create(mp_BatteryArc);
  lv_label_set_text(mp_SocLabel, "--%");
  lv_obj_set_style_text_font(mp_SocLabel, &lv_font_montserrat_32, 0);
  lv_obj_align(mp_SocLabel, LV_ALIGN_CENTER, 0, -10);

  lv_obj_t *p_SocSubLabel = lv_label_create(mp_BatteryArc);
  lv_label_set_text(p_SocSubLabel, "Ladestand");
  lv_obj_set_style_text_color(p_SocSubLabel, lv_color_hex(0x95A5A6), 0);
  lv_obj_align(p_SocSubLabel, LV_ALIGN_CENTER, 0, 22);

  // === RIGHT: Info cards ===
  lv_obj_t *p_InfoPanel = lv_obj_create(p_ParentTab);
  lv_obj_remove_style_all(p_InfoPanel);
  lv_obj_set_style_flex_grow(p_InfoPanel, 1, 0);
  lv_obj_set_height(p_InfoPanel, lv_pct(100));
  lv_obj_set_flex_flow(p_InfoPanel, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(p_InfoPanel, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_row(p_InfoPanel, 16, 0);

  // --- Ladestrom card ---
  lv_obj_t *p_CurrentCard = lv_obj_create(p_InfoPanel);
  lv_obj_set_width(p_CurrentCard, lv_pct(90));
  lv_obj_set_height(p_CurrentCard, LV_SIZE_CONTENT);
  lv_obj_set_style_bg_color(p_CurrentCard, lv_color_hex(0x0D231A), 0);
  lv_obj_set_style_border_color(p_CurrentCard, lv_color_hex(0x2ECC71), 0);
  lv_obj_set_style_border_width(p_CurrentCard, 2, 0);
  lv_obj_set_style_radius(p_CurrentCard, 12, 0);
  lv_obj_set_style_pad_all(p_CurrentCard, 14, 0);
  lv_obj_set_flex_flow(p_CurrentCard, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(p_CurrentCard, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  lv_obj_t *p_CurrentTitle = lv_label_create(p_CurrentCard);
  lv_label_set_text(p_CurrentTitle, "Ladestrom");
  lv_obj_set_style_text_color(p_CurrentTitle, lv_color_hex(0x95A5A6), 0);

  mp_CurrentValueLabel = lv_label_create(p_CurrentCard);
  lv_label_set_text(mp_CurrentValueLabel, "-- A");
  lv_obj_set_style_text_font(mp_CurrentValueLabel, &lv_font_montserrat_24, 0);
  lv_obj_set_style_text_color(mp_CurrentValueLabel, lv_color_hex(0x2ECC71), 0);

  // --- Restzeit card ---
  lv_obj_t *p_TimeCard = lv_obj_create(p_InfoPanel);
  lv_obj_set_width(p_TimeCard, lv_pct(90));
  lv_obj_set_height(p_TimeCard, LV_SIZE_CONTENT);
  lv_obj_set_style_bg_color(p_TimeCard, lv_color_hex(0x0D1523), 0);
  lv_obj_set_style_border_color(p_TimeCard, lv_color_hex(0x3498DB), 0);
  lv_obj_set_style_border_width(p_TimeCard, 2, 0);
  lv_obj_set_style_radius(p_TimeCard, 12, 0);
  lv_obj_set_style_pad_all(p_TimeCard, 14, 0);
  lv_obj_set_flex_flow(p_TimeCard, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(p_TimeCard, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  lv_obj_t *p_TimeTitle = lv_label_create(p_TimeCard);
  lv_label_set_text(p_TimeTitle, "Restzeit");
  lv_obj_set_style_text_color(p_TimeTitle, lv_color_hex(0x95A5A6), 0);

  mp_RemainingTimeLabel = lv_label_create(p_TimeCard);
  lv_label_set_text(mp_RemainingTimeLabel, "-- min");
  lv_obj_set_style_text_font(mp_RemainingTimeLabel, &lv_font_montserrat_24, 0);
  lv_obj_set_style_text_color(mp_RemainingTimeLabel, lv_color_hex(0x3498DB), 0);
}

void LvTabHistory::updateChargingState(int i_SocPercent)
{
  if (mp_BatteryArc == nullptr || mp_SocLabel == nullptr)
    return;

  if (i_SocPercent < 0)   i_SocPercent = 0;
  if (i_SocPercent > 100) i_SocPercent = 100;

  lv_arc_set_value(mp_BatteryArc, i_SocPercent);

  lv_color_t arcColor;
  if (i_SocPercent <= 20)
    arcColor = lv_color_hex(0xE74C3C); // rot
  else if (i_SocPercent <= 50)
    arcColor = lv_color_hex(0xE67E22); // orange
  else
    arcColor = lv_color_hex(0x2ECC71); // gruen

  lv_obj_set_style_arc_color(mp_BatteryArc, arcColor, LV_PART_INDICATOR);
  lv_obj_set_style_text_color(mp_SocLabel, arcColor, 0);

  char ac_Buf[8];
  snprintf(ac_Buf, sizeof(ac_Buf), "%d%%", i_SocPercent);
  lv_label_set_text(mp_SocLabel, ac_Buf);
}

void LvTabHistory::updateChargingCurrent(float f_CurrentAmpere)
{
  if (mp_CurrentValueLabel == nullptr)
    return;

  char ac_Buf[16];
  snprintf(ac_Buf, sizeof(ac_Buf), "%.1f A", f_CurrentAmpere);
  lv_label_set_text(mp_CurrentValueLabel, ac_Buf);
}

void LvTabHistory::updateRemainingTime(int i_RemainingMinutes)
{
  if (mp_RemainingTimeLabel == nullptr)
    return;

  char ac_Buf[16];
  if (i_RemainingMinutes < 0)
  {
    lv_label_set_text(mp_RemainingTimeLabel, "-- min");
  }
  else if (i_RemainingMinutes >= 60)
  {
    int i_Hours = i_RemainingMinutes / 60;
    int i_Mins  = i_RemainingMinutes % 60;
    snprintf(ac_Buf, sizeof(ac_Buf), "%d:%02d h", i_Hours, i_Mins);
    lv_label_set_text(mp_RemainingTimeLabel, ac_Buf);
  }
  else
  {
    snprintf(ac_Buf, sizeof(ac_Buf), "%d min", i_RemainingMinutes);
    lv_label_set_text(mp_RemainingTimeLabel, ac_Buf);
  }
}

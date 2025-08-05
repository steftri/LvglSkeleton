#include "lv_main.h"




LvTabSettings::LvTabSettings(void)
  : mp_WlanEnableSwitch(nullptr)
{
}



void LvTabSettings::setup(lv_obj_t *p_ParentTab)
{
  lv_obj_t *p_WlanPanel = lv_obj_create(p_ParentTab);
  {
    // lv_obj_set_height(p_WlanPanel, LV_SIZE_CONTENT);
    lv_obj_set_size(p_WlanPanel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(p_WlanPanel, LV_FLEX_FLOW_COLUMN);

    lv_obj_t *p_WlanLabel = lv_label_create(p_WlanPanel);
    lv_label_set_text(p_WlanLabel, "WLAN");

    lv_obj_t *p_WlanEnablePanel = lv_obj_create(p_WlanPanel);
    {
      lv_obj_set_size(p_WlanEnablePanel, lv_pct(100), LV_SIZE_CONTENT);
      lv_obj_set_flex_flow(p_WlanEnablePanel, LV_FLEX_FLOW_ROW);

      lv_obj_t *p_WlanEnableLabel = lv_label_create(p_WlanEnablePanel);
      lv_label_set_text(p_WlanEnableLabel, "WLAN");

      mp_WlanEnableSwitch = lv_switch_create(p_WlanEnablePanel);
      lv_obj_add_event_cb(mp_WlanEnableSwitch, wlanEnableCallback, LV_EVENT_CLICKED, NULL);
      lv_obj_align_to(p_WlanEnableLabel, mp_WlanEnableSwitch, LV_ALIGN_OUT_TOP_MID, 0, -15);
    }
  }
}


void LvTabSettings::wlanEnableCallback(lv_event_t *p_Event)
{
  lv_obj_t *p_Switch = lv_event_get_target_obj(p_Event);
  bool b_IsChecked = lv_obj_has_state(p_Switch, LV_STATE_CHECKED);
  
  LV_LOG_USER("WLAN Enable Switch is %s", b_IsChecked ? "ON" : "OFF");
}

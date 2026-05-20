#include "lv_tab_settings.h"
#include "lv_main.h"


#include "controller.h"
extern Controller g_controller;


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
      lv_obj_add_event_cb(mp_WlanEnableSwitch, onWlanEnableCallback, LV_EVENT_CLICKED, NULL);
      lv_obj_align_to(p_WlanEnableLabel, mp_WlanEnableSwitch, LV_ALIGN_OUT_TOP_MID, 0, -15);
    }

    mp_WlanStatePanel = lv_obj_create(p_WlanPanel);
    {
      lv_obj_set_size(mp_WlanStatePanel, lv_pct(100), LV_SIZE_CONTENT);
      lv_obj_set_flex_flow(mp_WlanStatePanel, LV_FLEX_FLOW_COLUMN);

      mp_CurrentWlan = lv_label_create(mp_WlanStatePanel); 
      lv_label_set_text(mp_CurrentWlan, "");
      mp_CurrentIp = lv_label_create(mp_WlanStatePanel); 
      lv_label_set_text(mp_CurrentIp, "");
    }

    mp_WlanSelectList = lv_list_create(p_WlanPanel);
    {
      lv_obj_set_size(mp_WlanSelectList, lv_pct(100), LV_SIZE_CONTENT);

      //lv_obj_set_style_pad_row(p_WlanSelectList, 4, 0);

      /*Add buttons to the list*/
      for(uint8_t i = 0; i < WifiData::MAX_WIFI_NETWORKS; i++) 
      {
        mp_WlanSsidButton[i] = lv_list_add_button(mp_WlanSelectList, nullptr, nullptr);
        lv_obj_add_event_cb(mp_WlanSsidButton[i], onWlanSsidButtonCallback, LV_EVENT_CLICKED, nullptr);
        mp_WlanSsidLabel[i] = lv_label_create(mp_WlanSsidButton[i]);
        lv_obj_add_flag(mp_WlanSsidButton[i], LV_OBJ_FLAG_HIDDEN);
      }
    }  
    updateWlanStatePanel();
    updateWlanSelectList(); // Populate the Wi-Fi list with available networks
      
  }
}


void LvTabSettings::updateWlanStatePanel(void)
{
  WifiData &WifiData = g_controller.getModel().getData().getWifiData();
  char ac_Ssid[WifiData::MAX_SSID_LENGTH + 1];
  WifiData::EState e_WifiState = WifiData.getState();

  if(e_WifiState == WifiData::EState::Disabled)
  {
    lv_obj_add_flag(mp_WlanStatePanel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(mp_WlanSelectList, LV_OBJ_FLAG_HIDDEN);
  }
  else
  {
    if(e_WifiState == WifiData::EState::Connected || e_WifiState == WifiData::EState::Connecting)
    {
      lv_obj_clear_flag(mp_WlanStatePanel, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
      lv_obj_add_flag(mp_WlanStatePanel, LV_OBJ_FLAG_HIDDEN);
    }
    lv_obj_clear_flag(mp_WlanSelectList, LV_OBJ_FLAG_HIDDEN);
  }

  WifiData.getSelectedNetwork(ac_Ssid, nullptr);
  lv_label_set_text(mp_CurrentWlan, ac_Ssid);

  char ac_IPAddress[WifiData::MAX_IP_ADDRESS_LENGTH + 1];
  WifiData.getIPAddress(ac_IPAddress, sizeof(ac_IPAddress));
  lv_label_set_text(mp_CurrentIp, ac_IPAddress);
}



void LvTabSettings::updateWlanSelectList(void)
{
  uint8_t u8_NumberOfNetworks = g_controller.getModel().getData().getWifiData().getAvailableNetworkCount();

  Serial.printf("Updating WLAN select list with %d networks\n", u8_NumberOfNetworks);
    
  for(uint8_t i = 0; i < WifiData::MAX_WIFI_NETWORKS; i++) 
  {
    if(i<u8_NumberOfNetworks)
    {
      char ac_Ssid[WifiData::MAX_SSID_LENGTH + 1];
      g_controller.getModel().getData().getWifiData().getAvailableNetwork(ac_Ssid, sizeof(ac_Ssid), i);
      lv_obj_clear_flag(mp_WlanSsidButton[i], LV_OBJ_FLAG_HIDDEN);
      lv_label_set_text(mp_WlanSsidLabel[i], ac_Ssid);
    } 
    else 
    {
      lv_obj_add_flag(mp_WlanSsidButton[i], LV_OBJ_FLAG_HIDDEN);
    }
  }
}



void LvTabSettings::onWlanEnableCallback(lv_event_t *p_Event)
{
  lv_obj_t *p_Switch = lv_event_get_target_obj(p_Event);
  bool b_IsChecked = lv_obj_has_state(p_Switch, LV_STATE_CHECKED);
  
  LV_LOG_USER("WLAN Enable Switch is %s", b_IsChecked ? "ON" : "OFF");
  if(b_IsChecked)
  {
    g_controller.getWifi().enable(); // Enable Wi-Fi when the switch is turned on
  }
  else
  {
    g_controller.getWifi().disable(); // Disable Wi-Fi when the switch is turned off
  }

}


void LvTabSettings::onWlanSsidButtonCallback(lv_event_t *p_Event)
{
  lv_obj_t *p_Button = lv_event_get_target_obj(p_Event);
  lv_obj_t *p_List = lv_obj_get_parent(p_Button);
  uint8_t u8_Index;

  if(p_List == nullptr)
  {
    LV_LOG_ERROR("Parent list not found for the button");
    return;
  }

  // Find the index of the button in the list
  for(u8_Index = 0; u8_Index < lv_obj_get_child_count(p_List); u8_Index++) 
  {
    if(p_Button == lv_obj_get_child(p_List, u8_Index)) 
    {
      break;
    }
  }

  if(u8_Index < WifiData::MAX_WIFI_NETWORKS) 
  {
    LV_LOG_USER("Selected Wi-Fi network %d", u8_Index);
    g_controller.getModel().getData().getWifiData().setSelectedNetwork(u8_Index);
    g_controller.getView().updateWlanState();

    g_controller.getView().showWlanPasswdDialog();
  }
}

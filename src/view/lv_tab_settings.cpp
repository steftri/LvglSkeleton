#include <Arduino.h>

#include "lv_tab_settings.h"
#include "lv_main.h"


#include "controller.h"

extern Controller g_controller;

extern LvMain g_ViewLvMain; 



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

      lv_obj_t *p_ButtonPanel = lv_obj_create(mp_WlanStatePanel);
      {
        lv_obj_remove_style_all(p_ButtonPanel);
        lv_obj_set_size(p_ButtonPanel, lv_pct(100), LV_SIZE_CONTENT);

        // Create the disconnect button
        mp_DisconnectButton = lv_btn_create(p_ButtonPanel);
        {
          lv_obj_align(mp_DisconnectButton, LV_ALIGN_RIGHT_MID, 0, 0);
          lv_obj_t *p_Label = lv_label_create(mp_DisconnectButton);
          lv_label_set_text(p_Label, "Disconnect");
          lv_obj_add_event_cb(mp_DisconnectButton, onWlanDisconnectButtonCallback, LV_EVENT_CLICKED, NULL);
        }
      }
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
  WifiData &r_WifiData = g_controller.getModel().getData().getWifiData();

  if(!r_WifiData.isEnabled())
  {
    lv_obj_add_flag(mp_WlanStatePanel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(mp_WlanSelectList, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_state(mp_WlanEnableSwitch, LV_STATE_CHECKED, false);
  }
  else
  {
    WifiData::EState e_WifiState = r_WifiData.getState();
    char ac_Ssid[WifiData::MAX_SSID_LENGTH + 1];

    lv_obj_set_state(mp_WlanEnableSwitch, LV_STATE_CHECKED, true);

    r_WifiData.getSelectedNetwork(ac_Ssid, sizeof(ac_Ssid), nullptr, 0);
    lv_label_set_text(mp_CurrentWlan, ac_Ssid);

    if(e_WifiState == WifiData::EState::Connected)
    {
      char ac_IPAddress[WifiData::MAX_IP_ADDRESS_LENGTH + 1];
      r_WifiData.getIPAddress(ac_IPAddress, sizeof(ac_IPAddress));
      lv_label_set_text(mp_CurrentIp, ac_IPAddress);

      lv_obj_clear_flag(mp_WlanStatePanel, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(mp_WlanSelectList, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
      lv_obj_add_flag(mp_WlanStatePanel, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(mp_WlanSelectList, LV_OBJ_FLAG_HIDDEN);
    }
  }
}



void LvTabSettings::updateWlanSelectList(void)
{
  WifiData &r_WifiData = g_controller.getModel().getData().getWifiData();
  uint8_t u8_NumberOfNetworks = r_WifiData.getAvailableNetworkCount();

  if (!r_WifiData.isEnabled())
  {
    u8_NumberOfNetworks = 0; // No networks available when Wi-Fi is disabled
    Serial.println("Wi-Fi is disabled, hiding available networks");
  }
    
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
  
  LV_LOG_USER("WLAN Enable Switch is %s", b_IsChecked ? "ON" : "OFF");;
  if(b_IsChecked)
  {
    g_controller.getWifi().enable(); // Enable Wi-Fi when the switch is turned on
  }
  else
  {
    g_controller.getWifi().disable(); // Disable Wi-Fi when the switch is turned off
  }
}


void LvTabSettings::onWlanDisconnectButtonCallback(lv_event_t *p_Event)
{
  LV_LOG_USER("WLAN Disconnect button clicked");
  g_controller.getWifi().disconnect(); // Disconnect from Wi-Fi
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
    auto &r_WifiData = g_controller.getModel().getData().getWifiData();

    char ac_Ssid[WifiData::MAX_SSID_LENGTH + 1];
    char ac_Password[WifiData::MAX_WPA2_PASSWORD_LENGTH + 1];

    r_WifiData.getAvailableNetwork(ac_Ssid, sizeof(ac_Ssid), u8_Index);
    r_WifiData.setSelectedNetworkSSID(ac_Ssid);
    g_controller.getModel().getWifiPassword(ac_Password, sizeof(ac_Password), ac_Ssid);

    g_ViewLvMain.getWlanPasswdDialog()->show(ac_Ssid, ac_Password);
  }
}

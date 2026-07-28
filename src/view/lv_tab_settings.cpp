#include <stdlib.h>


#include "lv_tab_settings.h"
#include "lv_main.h"


#include "controller.h"

extern Controller g_controller;

extern LvMain g_ViewLvMain; 


const static uint8_t CONTAINER_PADDING = 12;

const static uint32_t ICON_COLOR = 0x8f8f8f; // Gray color for icons



LvTabSettings::LvTabSettings(void)
{
}



void LvTabSettings::setup(lv_obj_t *p_ParentTab)
{
  static const lv_coord_t a_GridColumns[] = {132, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

  lv_obj_set_flex_flow(p_ParentTab, LV_FLEX_FLOW_COLUMN);

  lv_obj_t *p_SystemPanel = lv_obj_create(p_ParentTab);
  {
    lv_obj_set_style_pad_all(p_SystemPanel, CONTAINER_PADDING, 0);

    static const lv_coord_t a_GridRows[] = {LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};

    lv_obj_set_size(p_SystemPanel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_layout(p_SystemPanel, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(p_SystemPanel, a_GridColumns, a_GridRows);

    // Host name
    lv_obj_t *p_HostnameLabel = lv_label_create(p_SystemPanel);
    lv_label_set_text(p_HostnameLabel, "Hostname");
    lv_obj_set_grid_cell(p_HostnameLabel, LV_GRID_ALIGN_START, 0, 1, //column
                                          LV_GRID_ALIGN_CENTER, 0, 1); //row

    m_System.mp_HostName = lv_textarea_create(p_SystemPanel);
    lv_textarea_set_one_line(m_System.mp_HostName, true);
    lv_obj_set_grid_cell(m_System.mp_HostName, LV_GRID_ALIGN_STRETCH, 1, 1, //column
                                               LV_GRID_ALIGN_CENTER, 0, 1); //row
    lv_obj_set_width(m_System.mp_HostName, lv_pct(100));
    lv_obj_add_event_cb(m_System.mp_HostName, onInputEvent, LV_EVENT_ALL, this);
  }


  lv_obj_t *p_WlanPanel = lv_obj_create(p_ParentTab);
  {
    lv_obj_set_style_pad_all(p_WlanPanel, CONTAINER_PADDING, 0);

    lv_obj_set_size(p_WlanPanel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(p_WlanPanel, LV_FLEX_FLOW_COLUMN);

    // WiFi symbol image
    LV_IMAGE_DECLARE(wifi_32x26);
    lv_obj_t * p_imgWifi = lv_image_create(p_WlanPanel);
    lv_image_set_src(p_imgWifi, &wifi_32x26);
    lv_obj_set_style_image_recolor(p_imgWifi, lv_color_hex(ICON_COLOR), LV_PART_MAIN);
    //lv_obj_align(p_imgWifi, LV_ALIGN_CENTER, 0, 0);
    //lv_obj_set_size(p_imgWifi, 34, 34);
    lv_image_set_inner_align(p_imgWifi, LV_IMAGE_ALIGN_CENTER);

    // Wi-Fi enable switch
    lv_obj_t *p_WlanEnablePanel = lv_obj_create(p_WlanPanel);
    {
      lv_obj_remove_style_all(p_WlanEnablePanel);
      lv_obj_set_style_margin_right(p_WlanEnablePanel, 16, 0);
      lv_obj_set_size(p_WlanEnablePanel, lv_pct(100), LV_SIZE_CONTENT);
      lv_obj_set_flex_align(p_WlanEnablePanel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
 
      lv_obj_t *p_WlanEnableLabel = lv_label_create(p_WlanEnablePanel);
      lv_obj_set_style_margin_right(p_WlanEnableLabel, 8, 0);
      lv_label_set_text(p_WlanEnableLabel, "Wi-Fi");
      m_Wlan.mp_EnableSwitch = lv_switch_create(p_WlanEnablePanel);
      lv_obj_add_event_cb(m_Wlan.mp_EnableSwitch, onWlanEnableCallback, LV_EVENT_CLICKED, NULL);
    }

    // Wi-Fi state panel
    m_Wlan.mp_StatePanel = lv_obj_create(p_WlanPanel);
    {
      //lv_obj_remove_style_all(p_WlanEnablePanel);
      lv_obj_set_size(m_Wlan.mp_StatePanel, lv_pct(100), LV_SIZE_CONTENT);
      lv_obj_set_flex_flow(m_Wlan.mp_StatePanel, LV_FLEX_FLOW_COLUMN);

      lv_obj_t *p_CurrentWlanPanel = lv_obj_create(m_Wlan.mp_StatePanel);
      {
        lv_obj_remove_style_all(p_CurrentWlanPanel);
        lv_obj_set_size(p_CurrentWlanPanel, lv_pct(100), LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(p_CurrentWlanPanel, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(p_CurrentWlanPanel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

        lv_obj_t *p_CurrentWlanInfoPanel = lv_obj_create(p_CurrentWlanPanel);
        {
          lv_obj_remove_style_all(p_CurrentWlanInfoPanel);
          lv_obj_set_size(p_CurrentWlanInfoPanel, lv_pct(100), LV_SIZE_CONTENT);
          lv_obj_set_flex_flow(p_CurrentWlanInfoPanel, LV_FLEX_FLOW_COLUMN);
          lv_obj_set_flex_grow(p_CurrentWlanInfoPanel, 1);

          m_Wlan.mp_CurrentWlan = lv_label_create(p_CurrentWlanInfoPanel); 
          lv_label_set_text(m_Wlan.mp_CurrentWlan, "");
          m_Wlan.mp_CurrentIp = lv_label_create(p_CurrentWlanInfoPanel); 
          lv_label_set_text(m_Wlan.mp_CurrentIp, "");
        }

        // Create the disconnect button
        m_Wlan.mp_DisconnectButton = lv_btn_create(p_CurrentWlanPanel);
        {
          lv_obj_align(m_Wlan.mp_DisconnectButton, LV_ALIGN_RIGHT_MID, 0, 0);
          lv_obj_t *p_Label = lv_label_create(m_Wlan.mp_DisconnectButton);
          lv_label_set_text(p_Label, "Disconnect");
          lv_obj_add_event_cb(m_Wlan.mp_DisconnectButton, onWlanDisconnectButtonCallback, LV_EVENT_CLICKED, NULL);
        }        

      }
    }

    m_Wlan.mp_SelectList = lv_list_create(p_WlanPanel);
    {
      lv_obj_set_size(m_Wlan.mp_SelectList, lv_pct(100), LV_SIZE_CONTENT);
      lv_obj_set_style_max_height(m_Wlan.mp_SelectList, 180, 0);

      /*Add buttons to the list*/
      for(uint8_t i = 0; i < WifiData::MAX_WIFI_NETWORKS; i++) 
      {
        m_Wlan.mp_SsidButton[i] = lv_list_add_button(m_Wlan.mp_SelectList, nullptr, nullptr);
        lv_obj_add_event_cb(m_Wlan.mp_SsidButton[i], onWlanSsidButtonCallback, LV_EVENT_CLICKED, nullptr);
        m_Wlan.mp_SsidLabel[i] = lv_label_create(m_Wlan.mp_SsidButton[i]);
        lv_obj_add_flag(m_Wlan.mp_SsidButton[i], LV_OBJ_FLAG_HIDDEN);
      }
    }

    updateWlanStatePanel();
    updateWlanSelectList(); 
  }

  lv_obj_t *p_MqttPanel = lv_obj_create(p_ParentTab);
  {
    lv_obj_set_style_pad_all(p_MqttPanel, CONTAINER_PADDING, 0);

    static const lv_coord_t a_GridRows[] = {LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};

    lv_obj_set_size(p_MqttPanel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_layout(p_MqttPanel, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(p_MqttPanel, a_GridColumns, a_GridRows);

    // Icon
    LV_IMAGE_DECLARE(mqtt_28x28);
    lv_obj_t * p_imgMqtt = lv_image_create(p_MqttPanel);
    lv_image_set_src(p_imgMqtt, &mqtt_28x28);
    lv_obj_set_style_image_recolor(p_imgMqtt, lv_color_hex(ICON_COLOR), LV_PART_MAIN);
    //lv_obj_align(p_imgMqtt, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_grid_cell(p_imgMqtt, LV_GRID_ALIGN_START, 0, 1, //column
                                    LV_GRID_ALIGN_CENTER, 0, 1); //row

    // MQTT Broker Address
    lv_obj_t *p_MqttBrokerLabel = lv_label_create(p_MqttPanel);
    lv_label_set_text(p_MqttBrokerLabel, "MQTT Broker");
    lv_obj_set_grid_cell(p_MqttBrokerLabel, LV_GRID_ALIGN_START, 0, 1, //column
                                          LV_GRID_ALIGN_CENTER, 1, 1); //row

    m_Mqtt.mp_Broker = lv_textarea_create(p_MqttPanel);
    lv_textarea_set_one_line(m_Mqtt.mp_Broker, true);
    lv_textarea_set_accepted_chars(m_Mqtt.mp_Broker, "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-");
    lv_obj_set_grid_cell(m_Mqtt.mp_Broker, LV_GRID_ALIGN_STRETCH, 1, 1, //column
                                               LV_GRID_ALIGN_CENTER, 1, 1); //row
    lv_obj_set_width(m_Mqtt.mp_Broker, lv_pct(100));
    lv_obj_add_event_cb(m_Mqtt.mp_Broker, onInputEvent, LV_EVENT_ALL, this);

    // MQTT Port
    lv_obj_t *p_MqttPortLabel = lv_label_create(p_MqttPanel);
    lv_label_set_text(p_MqttPortLabel, "Port");
    lv_obj_set_grid_cell(p_MqttPortLabel, LV_GRID_ALIGN_START, 0, 1, //column
                                          LV_GRID_ALIGN_CENTER, 2, 1); //row

    m_Mqtt.mp_Port = lv_textarea_create(p_MqttPanel);
    lv_textarea_set_one_line(m_Mqtt.mp_Port, true);
    lv_textarea_set_accepted_chars(m_Mqtt.mp_Port, "0123456789");
    lv_obj_set_grid_cell(m_Mqtt.mp_Port, LV_GRID_ALIGN_STRETCH, 1, 1, //column
                                               LV_GRID_ALIGN_CENTER, 2, 1); //row
    lv_obj_set_width(m_Mqtt.mp_Port, 80);
    lv_obj_add_event_cb(m_Mqtt.mp_Port, onInputEvent, LV_EVENT_ALL, this);

    // MQTT Sparkplug-B Group ID
    lv_obj_t *p_MqttGroupIdLabel = lv_label_create(p_MqttPanel);
    lv_label_set_text(p_MqttGroupIdLabel, "Group ID");
    lv_obj_set_grid_cell(p_MqttGroupIdLabel, LV_GRID_ALIGN_START, 0, 1, //column
                                          LV_GRID_ALIGN_CENTER, 3, 1); //row

    m_Mqtt.mp_GroupId = lv_textarea_create(p_MqttPanel);
    lv_textarea_set_one_line(m_Mqtt.mp_GroupId, true);
    lv_textarea_set_accepted_chars(m_Mqtt.mp_GroupId, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-");
    lv_obj_set_grid_cell(m_Mqtt.mp_GroupId, LV_GRID_ALIGN_STRETCH, 1, 1, //column
                                               LV_GRID_ALIGN_CENTER, 3, 1); //row
    lv_obj_set_width(m_Mqtt.mp_GroupId, lv_pct(100));
    lv_obj_add_event_cb(m_Mqtt.mp_GroupId, onInputEvent, LV_EVENT_ALL, this);
  }  

}



void LvTabSettings::showWlanPasswordDialog(const char *pc_Ssid, const char *pc_Password)
{
  m_Wlan.mp_PasswordDialog = lv_msgbox_create(lv_screen_active());

  lv_obj_set_size(m_Wlan.mp_PasswordDialog, lv_pct(80), LV_SIZE_CONTENT);
  //lv_obj_set_style_pad_all(m_Wlan.mp_PasswordDialog, 8, LV_PART_MAIN);

  lv_msgbox_add_title(m_Wlan.mp_PasswordDialog, "Wi-Fi");

  lv_obj_t * mp_PasswordPanel = lv_msgbox_get_content(m_Wlan.mp_PasswordDialog);
  {
    lv_obj_set_style_pad_all(mp_PasswordPanel, 8, LV_PART_MAIN);

    lv_obj_t *p_SsidLabel = lv_label_create(mp_PasswordPanel);
    //lv_label_set_text(mp_SsidLabel, "SSID:");
    lv_obj_align(p_SsidLabel, LV_ALIGN_TOP_LEFT, 0, 0);

    lv_label_set_text_fmt(p_SsidLabel, "Password for %s:", pc_Ssid);

    // Create the password input field
    m_Wlan.mp_SsidPassword = lv_textarea_create(mp_PasswordPanel);
    lv_textarea_set_password_mode(m_Wlan.mp_SsidPassword, true);
    lv_textarea_set_one_line(m_Wlan.mp_SsidPassword, true);
    lv_obj_set_width(m_Wlan.mp_SsidPassword, lv_pct(100));
    //lv_obj_align(m_Wlan.mp_SsidPassword, LV_ALIGN_TOP_LEFT, 10, 50);
    lv_obj_add_event_cb(m_Wlan.mp_SsidPassword, onInputEvent, LV_EVENT_ALL, this);

    if (pc_Password)
    {
        lv_textarea_set_text(m_Wlan.mp_SsidPassword, pc_Password);
    }
    else
    {
        lv_textarea_set_text(m_Wlan.mp_SsidPassword, "");
    }
  }
  lv_msgbox_add_close_button(m_Wlan.mp_PasswordDialog);

  lv_obj_t *p_ConnectButton = lv_msgbox_add_footer_button(m_Wlan.mp_PasswordDialog, "Connect");
  lv_obj_add_event_cb(p_ConnectButton, onPasswordEnteredEvent, LV_EVENT_CLICKED, this);

  //lv_obj_t *p_CancelButton = lv_msgbox_add_footer_button(m_Wlan.mp_PasswordDialog, "Cancel");
  //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, NULL);
}



void LvTabSettings::updateSystemSettingsPanel(void)
{
  SystemSettings &r_SystemSettings = g_controller.getModel().getSettings().getSystemSettings();

  lv_textarea_set_text(m_System.mp_HostName, r_SystemSettings.getHostName());
}



void LvTabSettings::updateWlanSettingsPanel(void)
{
  WifiSettings &r_WifiSettings = g_controller.getModel().getSettings().getWifiSettings();

  // Update the Wi-Fi enable switch state
  lv_obj_set_state(m_Wlan.mp_EnableSwitch, LV_STATE_CHECKED, r_WifiSettings.getEnable());
}



void LvTabSettings::updateMqttSettingsPanel(void)
{
  MqttSettings &r_MqttSettings = g_controller.getModel().getSettings().getMqttSettings();

  lv_textarea_set_text(m_Mqtt.mp_Broker, r_MqttSettings.getBrokerAddr());

  char ac_Port[6];
  snprintf(ac_Port, sizeof(ac_Port), "%d", r_MqttSettings.getBrokerPort());
  ac_Port[sizeof(ac_Port) - 1] = '\0'; // Ensure null-termination

  lv_textarea_set_text(m_Mqtt.mp_Port, ac_Port);

  lv_textarea_set_text(m_Mqtt.mp_GroupId, r_MqttSettings.getGroupId());
}




void LvTabSettings::onPasswordEnteredEvent(lv_event_t *p_Event)
{
    LvTabSettings *p_Instance = static_cast<LvTabSettings *>(lv_event_get_user_data(p_Event));
    const char *pc_Passwd = lv_textarea_get_text(p_Instance->m_Wlan.mp_SsidPassword);
    LV_LOG_USER("Password \"%s\" entered", pc_Passwd);
    g_controller.getModel().getData().getWifiData().setSelectedNetworkPassword(pc_Passwd);
    lv_msgbox_close(p_Instance->m_Wlan.mp_PasswordDialog); // Close the message box

    // Trigger the Wi-Fi connection attempt with the entered password
    g_controller.getWifi().connect(); // Trigger the Wi-Fi connection attempt with the entered password
}



void LvTabSettings::onInputEvent(lv_event_t *p_Event)
{
  lv_event_code_t code = lv_event_get_code(p_Event);
  LvTabSettings *p_This = static_cast<LvTabSettings*>(lv_event_get_user_data(p_Event));

  if(code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) 
  {
    lv_obj_t *p_Target = lv_event_get_target_obj(p_Event);
    lv_keyboard_mode_t e_Mode = (p_Target == p_This->m_Mqtt.mp_Port)
        ? LV_KEYBOARD_MODE_NUMBER
        : LV_KEYBOARD_MODE_TEXT_LOWER;
    g_ViewLvMain.getKeyboard()->show(p_Target, e_Mode);
  }
  else if(code == LV_EVENT_DEFOCUSED) 
  {
    g_ViewLvMain.getKeyboard()->hide();
  }
  else if(code == LV_EVENT_CANCEL)
  {
    g_ViewLvMain.getKeyboard()->hide();
    LV_LOG_USER("Input cancelled\n");
  }
  else if(code == LV_EVENT_READY)
  {
    lv_obj_t *p_Textarea = lv_event_get_target_obj(p_Event);
    g_ViewLvMain.getKeyboard()->hide();
    if(p_Textarea == p_This->m_Mqtt.mp_Broker)
    {    
      p_This->onInputMqttBrokerCallback(lv_textarea_get_text(p_Textarea));
    } 
    else if (p_Textarea == p_This->m_Mqtt.mp_Port)
    {
      p_This->onInputMqttPortCallback(static_cast<uint16_t>(atoi(lv_textarea_get_text(p_Textarea))));
    }
    else if (p_Textarea == p_This->m_Mqtt.mp_GroupId)
    {
      p_This->onInputMqttGroupIdCallback(lv_textarea_get_text(p_Textarea));
    }
    else if (p_Textarea == p_This->m_System.mp_HostName)
    {
      p_This->onInputSystemHostName(lv_textarea_get_text(p_Textarea));
    }
    else
    {
      LV_LOG_USER("Unhandled input event for object\n");
    }
  }
}



void LvTabSettings::onInputSystemHostName(const char *pc_HostName)
{
  SystemSettings &r_SystemSettings = g_controller.getModel().getSettings().getSystemSettings();

  LV_LOG_USER("new Hostname: %s\n", pc_HostName);
  r_SystemSettings.setHostName(pc_HostName); // Update the hostname in the settings
}





void LvTabSettings::onInputMqttBrokerCallback(const char *pc_Broker)
{
  MqttSettings &r_MqttSettings = g_controller.getModel().getSettings().getMqttSettings();

  LV_LOG_USER("new Broker: %s\n", pc_Broker);
  r_MqttSettings.setBrokerAddr(pc_Broker); // Update the MQTT broker address in the settings 
}



void LvTabSettings::onInputMqttPortCallback(uint16_t u16_Port)
{
  MqttSettings &r_MqttSettings = g_controller.getModel().getSettings().getMqttSettings();

  LV_LOG_USER("new Port: %d\n", u16_Port);
  r_MqttSettings.setBrokerPort(u16_Port);
}



void LvTabSettings::onInputMqttGroupIdCallback(const char *pc_GroupId)
{
  MqttSettings &r_MqttSettings = g_controller.getModel().getSettings().getMqttSettings();

  LV_LOG_USER("new GroupId: %s\n", pc_GroupId);
  r_MqttSettings.setGroupId(pc_GroupId);
}


void LvTabSettings::updateWlanStatePanel(void)
{
  WifiData &r_WifiData = g_controller.getModel().getData().getWifiData();

  if(!r_WifiData.isEnabled())
  {
    lv_obj_add_flag(m_Wlan.mp_StatePanel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(m_Wlan.mp_SelectList, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_state(m_Wlan.mp_EnableSwitch, LV_STATE_CHECKED, false);
  }
  else
  {
    WifiData::EState e_WifiState = r_WifiData.getState();
    char ac_Ssid[WifiData::MAX_SSID_LENGTH + 1];

    lv_obj_set_state(m_Wlan.mp_EnableSwitch, LV_STATE_CHECKED, true);

    r_WifiData.getSelectedNetwork(ac_Ssid, sizeof(ac_Ssid), nullptr, 0);
    lv_label_set_text(m_Wlan.mp_CurrentWlan, ac_Ssid);

    if(e_WifiState == WifiData::EState::Connected)
    {
      char ac_IPAddress[WifiData::MAX_IP_ADDRESS_LENGTH + 1];
      r_WifiData.getIPAddress(ac_IPAddress, sizeof(ac_IPAddress));
      lv_label_set_text(m_Wlan.mp_CurrentIp, ac_IPAddress);

      lv_obj_clear_flag(m_Wlan.mp_StatePanel, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(m_Wlan.mp_SelectList, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
      lv_obj_add_flag(m_Wlan.mp_StatePanel, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(m_Wlan.mp_SelectList, LV_OBJ_FLAG_HIDDEN);
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
//    Serial.println("Wi-Fi is disabled, hiding available networks");
  }
    
  for(uint8_t i = 0; i < WifiData::MAX_WIFI_NETWORKS; i++) 
  {
    if(i<u8_NumberOfNetworks)
    {
      char ac_Ssid[WifiData::MAX_SSID_LENGTH + 1];
      g_controller.getModel().getData().getWifiData().getAvailableNetwork(ac_Ssid, sizeof(ac_Ssid), i);
      lv_obj_clear_flag(m_Wlan.mp_SsidButton[i], LV_OBJ_FLAG_HIDDEN);
      lv_label_set_text(m_Wlan.mp_SsidLabel[i], ac_Ssid);
    } 
    else 
    {
      lv_obj_add_flag(m_Wlan.mp_SsidButton[i], LV_OBJ_FLAG_HIDDEN);
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

    //g_ViewLvMain.getWlanPasswdDialog()->show(ac_Ssid, ac_Password);
    g_ViewLvMain.getTabSettings()->showWlanPasswordDialog(ac_Ssid, ac_Password);
  }
}

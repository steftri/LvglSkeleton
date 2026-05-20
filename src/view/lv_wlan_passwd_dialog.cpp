#include "lv_wlan_passwd_dialog.h"
#include "controller.h"

extern Controller g_controller;



LvWlanPasswdDialog::LvWlanPasswdDialog()
    : mp_PasswordPanel(nullptr), mp_PasswordInput(nullptr), mp_SsidLabel(nullptr)
{
}

void LvWlanPasswdDialog::setup(lv_obj_t *p_ParentTab)
{
    // Create the password panel
    mp_PasswordPanel = lv_obj_create(p_ParentTab);
    lv_obj_set_size(mp_PasswordPanel, lv_pct(80), LV_SIZE_CONTENT);
    lv_obj_align(mp_PasswordPanel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_flex_flow(mp_PasswordPanel, LV_FLEX_FLOW_COLUMN);

    // Create the SSID label
    mp_SsidLabel = lv_label_create(mp_PasswordPanel);
    //lv_label_set_text(mp_SsidLabel, "SSID:");
    lv_obj_align(mp_SsidLabel, LV_ALIGN_TOP_LEFT, 0, 0);

    // Create the password input field
    mp_PasswordInput = lv_textarea_create(mp_PasswordPanel);
    lv_textarea_set_password_mode(mp_PasswordInput, true);
    lv_textarea_set_one_line(mp_PasswordInput, true);
    lv_obj_set_width(mp_PasswordInput, lv_pct(100));
    //lv_obj_align(mp_PasswordInput, LV_ALIGN_TOP_LEFT, 10, 50);
    lv_obj_add_event_cb(mp_PasswordInput, onInputEvent, LV_EVENT_ALL, this);

    // Create button panel
    lv_obj_t *p_ButtonPanel = lv_obj_create(mp_PasswordPanel);
    {
      lv_obj_remove_style_all(p_ButtonPanel);
      lv_obj_set_size(p_ButtonPanel, lv_pct(100), LV_SIZE_CONTENT);

      // Create the OK button
      lv_obj_t *p_OkButton = lv_btn_create(p_ButtonPanel);
      {
        lv_obj_align(p_OkButton, LV_ALIGN_LEFT_MID, 0, 0);
        lv_obj_t *p_OkLabel = lv_label_create(p_OkButton);
        lv_label_set_text(p_OkLabel, "Connect");
        lv_obj_add_event_cb(p_OkButton, onOkButtonEvent, LV_EVENT_CLICKED, this);
      }

      // Create the Cancel button
      lv_obj_t *p_CancelButton = lv_btn_create(p_ButtonPanel);
      {
        lv_obj_align(p_CancelButton, LV_ALIGN_RIGHT_MID, 0, 0);
        lv_obj_t *p_OkLabel = lv_label_create(p_CancelButton);
        lv_label_set_text(p_OkLabel, "Cancel");
        lv_obj_add_event_cb(p_CancelButton, onCancelButtonEvent, LV_EVENT_CLICKED, this);
      }      
    }

    lv_obj_add_flag(mp_PasswordPanel, LV_OBJ_FLAG_HIDDEN); // Initially hide the dialog box
}

void LvWlanPasswdDialog::show(const char *pc_Ssid, const char *pc_Password)
{
    if (mp_PasswordPanel)
    {
        lv_label_set_text_fmt(mp_SsidLabel, "Password for %s:", pc_Ssid);
        if (pc_Password)
        {
            lv_textarea_set_text(mp_PasswordInput, pc_Password);
        }
        else
        {
            lv_textarea_set_text(mp_PasswordInput, "");
        }
        lv_obj_clear_flag(mp_PasswordPanel, LV_OBJ_FLAG_HIDDEN);
    }
}

void LvWlanPasswdDialog::hide(void)
{
    if (mp_PasswordPanel)
    {
        lv_obj_add_flag(mp_PasswordPanel, LV_OBJ_FLAG_HIDDEN);
        g_controller.getView().getLvMain()->hideKeyboard();
    }
}


void LvWlanPasswdDialog::onInputEvent(lv_event_t *p_Event)
{
    lv_event_code_t code = lv_event_get_code(p_Event);
    lv_obj_t *p_TargetObj = lv_event_get_target_obj(p_Event);   

    if(code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) 
    {
        g_controller.getView().getLvMain()->showKeyboard(p_TargetObj);
    }
    else if(code == LV_EVENT_DEFOCUSED) 
    {
        g_controller.getView().getLvMain()->hideKeyboard();
    }
    else if(code == LV_EVENT_READY) 
    {
        LvWlanPasswdDialog *p_Instance = static_cast<LvWlanPasswdDialog *>(lv_event_get_user_data(p_Event));
        const char *pc_Passwd = lv_textarea_get_text(p_TargetObj);
        p_Instance->hide();
        LV_LOG_USER("Password \"%s\" entered", pc_Passwd);
        g_controller.getModel().getData().getWifiData().setNetworkPassword(pc_Passwd);
    }
    else if(code == LV_EVENT_CANCEL)
    {
        LvWlanPasswdDialog *p_Instance = static_cast<LvWlanPasswdDialog *>(lv_event_get_user_data(p_Event));
        p_Instance->hide();
        LV_LOG_USER("Password input cancelled\n");
    }
}


void LvWlanPasswdDialog::onOkButtonEvent(lv_event_t *p_Event)
{
    LvWlanPasswdDialog *p_Instance = static_cast<LvWlanPasswdDialog *>(lv_event_get_user_data(p_Event));
    const char *pc_Passwd = lv_textarea_get_text(p_Instance->mp_PasswordInput);
    LV_LOG_USER("Password \"%s\" entered", pc_Passwd);
    g_controller.getModel().getData().getWifiData().setNetworkPassword(pc_Passwd);
    p_Instance->hide();
}


void LvWlanPasswdDialog::onCancelButtonEvent(lv_event_t *p_Event)
{
    LvWlanPasswdDialog *p_Instance = static_cast<LvWlanPasswdDialog *>(lv_event_get_user_data(p_Event));
    p_Instance->hide();
    LV_LOG_USER("Password input cancelled\n");
}

/*

#include "lv_wlan_password.h"

LvWlanPasswordDialogbox::LvWlanPasswordDialogbox()
    : mp_PasswordPanel(nullptr), mp_PasswordInput(nullptr), mp_SsidLabel(nullptr)
{
}

void LvWlanPasswordDialogbox::setup(lv_obj_t *p_ParentTab)
{
    // Create the password panel
    mp_PasswordPanel = lv_obj_create(p_ParentTab);
    lv_obj_set_size(mp_PasswordPanel, 300, 200);
    lv_obj_align(mp_PasswordPanel, LV_ALIGN_CENTER, 0, 0);

    // Create the SSID label
    mp_SsidLabel = lv_label_create(mp_PasswordPanel);
    lv_label_set_text(mp_SsidLabel, "SSID:");
    lv_obj_align(mp_SsidLabel, LV_ALIGN_TOP_LEFT, 10, 10);

    // Create the password input field
    mp_PasswordInput = lv_textarea_create(mp_PasswordPanel);
    lv_textarea_set_password_mode(mp_PasswordInput, true);
    lv_textarea_set_one_line(mp_PasswordInput, true);
    lv_obj_set_width(mp_PasswordInput, 280);
    lv_obj_align(mp_PasswordInput, LV_ALIGN_TOP_LEFT, 10, 50);

    // Create the OK button
    lv_obj_t *p_OkButton = lv_btn_create(mp_PasswordPanel);
    lv_obj_set_size(p_OkButton, 100, 40);
    lv_obj_align(p_OkButton, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    lv_obj_t *p_OkLabel = lv_label_create(p_OkButton);
    lv_label_set_text(p_OkLabel, "Connect");
    lv_obj_center(p_OkLabel);
    lv_obj_add_event_cb(p_OkButton, onOkButtonEvent, LV_EVENT_CLICKED, this);

    // Create the Cancel button
    lv_obj_t *p_CancelButton = lv_btn_create(mp_PasswordPanel);
    lv_obj_set_size(p_CancelButton, 100, 40);
    lv_obj_align(p_CancelButton, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
    lv_obj_t *p_CancelLabel = lv_label_create(p_CancelButton);
    lv_label_set_text(p_CancelLabel, "Cancel");
    lv_obj_center(p_CancelLabel);
    lv_obj_add_event_cb(p_CancelButton, onCancelButtonEvent, LV_EVENT_CLICKED, this);

  //  hide(); // Initially hide the dialog box
}

void LvWlanPasswordDialogbox::show(const char *pc_Ssid, const char *pc_Password)
{
    if (mp_PasswordPanel)
    {
        lv_label_set_text(mp_SsidLabel, pc_Ssid);
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

void LvWlanPasswordDialogbox::hide(void)
{
    if (mp_PasswordPanel)
    {
        lv_obj_add_flag(mp_PasswordPanel, LV_OBJ_FLAG_HIDDEN);
    }
}

void LvWlanPasswordDialogbox::onOkButtonEvent(lv_event_t *p_Event)
{
    LvWlanPasswordDialogbox *p_Instance = static_cast<LvWlanPasswordDialogbox *>(lv_event_get_user_data(p_Event));
    if (p_Instance && p_Instance->mp_PasswordInput)
    {
        const char *password = lv_textarea_get_text(p_Instance->mp_PasswordInput);
        // Handle the password (e.g., save it or pass it to another component)
        // ...
        p_Instance->hide();
    }
}

void LvWlanPasswordDialogbox::onCancelButtonEvent(lv_event_t *p_Event)
{
    LvWlanPasswordDialogbox *p_Instance = static_cast<LvWlanPasswordDialogbox *>(lv_event_get_user_data(p_Event));
    if (p_Instance)
    {
        p_Instance->hide();
    }
}
*/
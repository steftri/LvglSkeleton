#include "lv_keyboard.h"


LvKeyboard::LvKeyboard()
    : mp_Keyboard(nullptr)
{
}


void LvKeyboard::setup(void)
{
  // Create the keyboard

  mp_Keyboard = lv_keyboard_create(lv_screen_active());
  lv_obj_set_size(mp_Keyboard, LV_HOR_RES, LV_HOR_RES/2);
  lv_obj_add_flag(mp_Keyboard, LV_OBJ_FLAG_HIDDEN); // Initially hide the keyboard
}

void LvKeyboard::show(lv_obj_t *p_TargetObj, lv_keyboard_mode_t e_Mode)
{
  if(mp_Keyboard == nullptr)
    return;

  lv_keyboard_set_mode(mp_Keyboard, e_Mode);
  lv_keyboard_set_textarea(mp_Keyboard, p_TargetObj);
  lv_obj_clear_flag(mp_Keyboard, LV_OBJ_FLAG_HIDDEN); 
}


void LvKeyboard::hide(void)
{
  if(mp_Keyboard == nullptr)
    return;
  lv_obj_add_flag(mp_Keyboard, LV_OBJ_FLAG_HIDDEN); 
}


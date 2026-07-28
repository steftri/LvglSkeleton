#ifndef _LV_KEYBOARD_H_
#define _LV_KEYBOARD_H_

#include <lvgl.h>

class LvKeyboard
{
  lv_obj_t *mp_Keyboard; 

public:
  LvKeyboard();
    
  void setup(void);
    
  void show(lv_obj_t *p_TargetObj, lv_keyboard_mode_t e_Mode = LV_KEYBOARD_MODE_TEXT_LOWER);
  void hide(void);
};

#endif // _LV_KEYBOARD_H_

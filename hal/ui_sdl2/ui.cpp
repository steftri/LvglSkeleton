#include <iostream>
#include <inttypes.h>

/**************************LVGL and UI************************
if you want to use the LVGL demo. you need to include <demos/lv_demos.h> and <examples/lv_examples.h>. 
if not, please do not include it. It will waste your Flash space.
**************************************************************/
#include <lvgl.h>

#define SDL_MAIN_HANDLED        /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>
#include "drivers/sdl/lv_sdl_mouse.h"
#include "drivers/sdl/lv_sdl_mousewheel.h"
#include "drivers/sdl/lv_sdl_keyboard.h"

#include "ui.h"


static lv_display_t *lvDisplay;
static lv_indev_t *lvMouse;
static lv_indev_t *lvMouseWheel;
static lv_indev_t *lvKeyboard;



static constexpr uint16_t screenHeight = 800;
static constexpr uint16_t screenWidth = 480;





Ui::Ui()
{
    // Constructor implementation (if needed)
}



void Ui::setup()
{
  // Workaround for sdl2 `-m32` crash
  // https://bugs.launchpad.net/ubuntu/+source/libsdl2/+bug/1775067/comments/7
  #ifndef WIN32
      setenv("DBUS_FATAL_WARNINGS", "0", 1);
  #endif

  #if LV_USE_LOG != 0
  lv_log_register_print_cb(lv_log_print_g_cb);
  #endif

  /* Add a display
   * Use the 'monitor' driver which creates window on PC's monitor to simulate a display */
  lvDisplay = lv_sdl_window_create(screenWidth, screenHeight);
  lvMouse = lv_sdl_mouse_create();
  lvMouseWheel = lv_sdl_mousewheel_create();
  lvKeyboard = lv_sdl_keyboard_create();

  mu32_LastTick = SDL_GetTicks();
}


void Ui::loop()
{
  uint32_t u32_CurrentTick;

  SDL_Delay(5);
  u32_CurrentTick = SDL_GetTicks();
  lv_tick_inc(u32_CurrentTick - mu32_LastTick); // Update the tick timer. Tick is new for LVGL 9
  mu32_LastTick = u32_CurrentTick;
  lv_timer_handler(); // Update the UI
}


#include <iostream>
#include <inttypes.h>

/**************************LVGL and UI************************
if you want to use the LVGL demo. you need to include <demos/lv_demos.h> and <examples/lv_examples.h>. 
if not, please do not include it. It will waste your Flash space.
**************************************************************/
#include <lvgl.h>

#define SDL_MAIN_HANDLED        /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>
//#include "drivers/sdl/lv_sdl_mouse.h"
//#include "drivers/sdl/lv_sdl_mousewheel.h"
//#include "drivers/sdl/lv_sdl_keyboard.h"

#include "display_bridge_lvgl_sdl.h"

#include "display.h"


static DisplayBridgeLvglSdl displayBridge;
static constexpr uint16_t screenWidth = 480;
static constexpr uint16_t screenHeight = 800;



static lv_disp_draw_buf_t draw_buf;
static lv_color_t disp_draw_buf1[screenWidth * screenHeight / 10];
static lv_color_t disp_draw_buf2[screenWidth * screenHeight / 10];
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;
static lv_indev_data_t indev_data;




SDL_Window *window;
SDL_Renderer *renderer;



const int BG_R = 128;
const int BG_G = 153;
const int BG_B = 151;

bool sdl_utils_Init(const char *title, SDL_Window **window, SDL_Renderer **renderer, bool textureFiltering)
{
    if (SDL_Init(SDL_INIT_VIDEO ) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    if (textureFiltering)
    {
        if (!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }
    }

    *window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if (window == 0)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == 0)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    SDL_SetRenderDrawColor(*renderer, BG_R, BG_G, BG_B, 0xFF);

    displayBridge.setRenderer(*renderer);
       
    return 1;
}

void sdl_utils_Quit(SDL_Window *window, SDL_Renderer *renderer)
{
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);

    SDL_Quit();
}


void lv_log_print_g_cb(const char *buf)
{
    std::cout << buf << std::endl;
}


Display::Display()
{
    // Constructor implementation (if needed)
}





void Display::setup()
{
  // Workaround for sdl2 `-m32` crash
  // https://bugs.launchpad.net/ubuntu/+source/libsdl2/+bug/1775067/comments/7
  #ifndef WIN32
      setenv("DBUS_FATAL_WARNINGS", "0", 1);
  #endif

  #if LV_USE_LOG != 0
  lv_log_register_print_cb(lv_log_print_g_cb);
  #endif

  lv_init();

  lv_disp_draw_buf_init(&draw_buf, disp_draw_buf1, disp_draw_buf2, screenWidth * screenHeight/10);
  /* Initialize the display */
  lv_disp_drv_init(&disp_drv);
  /* Change the following line to your display resolution */
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = DisplayBridgeLvglSdl::flushCallback;
  disp_drv.full_refresh = 1;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /* Initialize the (dummy) input device driver */

  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = DisplayBridgeLvglSdl::readTouchpanelCallback;
  lv_indev_drv_register(&indev_drv);
  
  /* Add a display
   * Use the 'monitor' driver which creates window on PC's monitor to simulate a display */
  //lvDisplay = lv_sdl_window_create(screenWidth, screenHeight);
  sdl_utils_Init("LVGL Skeleton", &window, &renderer, 0);

  mu32_LastTick = SDL_GetTicks();
}


void Display::loop()
{
  uint32_t u32_CurrentTick;
  SDL_Event event;

  SDL_Delay(5);

  if (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT) 
    {
      // Handle quit event
      std::cout << "SDL_QUIT event received. Exiting..." << std::endl;
      sdl_utils_Quit(window, renderer);
      exit(0);
    }
    else if (event.type == SDL_KEYDOWN)
    {
      // Handle key down event
      std::cout << "Key down: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
      // Handle mouse button down event
      std::cout << "Mouse button down at (" << event.button.x << ", " << event.button.y << ")" << std::endl;
      displayBridge.setLvIndevData(event.button.x, event.button.y, LV_INDEV_STATE_PR);
      indev_data.point.x = event.button.x;
      indev_data.point.y = event.button.y;
  //    DisplayBridgeLvglSdl::readTouchpanelCallback(&disp_drv, &indev_data); // Call the touch panel callback
    }
    else if (event.type == SDL_MOUSEBUTTONUP)
    {
      // Handle mouse button up event
      std::cout << "Mouse button up at (" << event.button.x << ", " << event.button.y << ")" << std::endl;
      displayBridge.setLvIndevData(event.button.x, event.button.y, LV_INDEV_STATE_REL);
      indev_data.point.x = event.button.x;
      indev_data.point.y = event.button.y;
    }
  } 

  u32_CurrentTick = SDL_GetTicks();
  lv_tick_inc(u32_CurrentTick - mu32_LastTick); // Update the tick timer. Tick is new for LVGL 9
  mu32_LastTick = u32_CurrentTick;
  lv_timer_handler(); // Update the UI
}


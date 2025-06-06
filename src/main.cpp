#ifdef ARDUINO 
#include <Arduino.h> 
#else
#include <iostream>
#include <inttypes.h>
#endif


#include <lvgl.h>

#ifdef ARDUINO
#include "../hal/crowpanel/display.h"
#else
#define SDL_MAIN_HANDLED        /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>
#include "../hal/sdl2/display.h"
#endif


#include "ui/ui.h"
/**************************LVGL and UI END************************/


extern lv_obj_t * ui_Screen1;
extern lv_obj_t * ui_Screen2;
extern lv_obj_t * ui_Screen3;


Display g_Display;


void setup()
{
#ifdef ARDUINO
  Serial.begin(115200);
  Serial.println(APPLICATION_NAME);
  Serial.println(__DATE__ " " __TIME__);
#else
  std::cout << APPLICATION_NAME << std::endl;
  std::cout << __DATE__ " " __TIME__ << std::endl;
#endif

/*  //GPIO init
  pinMode(38, OUTPUT);
  digitalWrite(38, LOW);
  pinMode(17, OUTPUT);
  digitalWrite(17, LOW);
  pinMode(18, OUTPUT);
  digitalWrite(18, LOW);
  pinMode(42, OUTPUT);
  digitalWrite(42, LOW);
*/
  lv_init();

  g_Display.setup();  // Initialize the display

  ui_init();    // LVGL demo
  
//  Serial.println( "Setup done" );

}

void loop()
{
  static bool b_SplashScreen = true;

#ifdef ARDUINO
  if(b_SplashScreen && millis()>4000) 
#else
  if(b_SplashScreen && SDL_GetTicks()>2000)  // Show the splash screen for 2 seconds
#endif
  {
    lv_disp_load_scr(ui_Screen3); 
    b_SplashScreen = false;
  }
  g_Display.loop();  // Update the display
}



#ifndef ARDUINO  
int main(int argc, char *argv[])
{
  setup();
  while(true)
    loop();
  return 0;
}
#endif

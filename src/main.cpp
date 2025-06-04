#ifdef ARDUINO 
#include <Arduino.h> 
#else
#include <iostream>
#include <inttypes.h>
#endif


#include <lvgl.h>

#include "../hal/crowpanel/display.h"

#include "lv_demo_widgets.h"
/**************************LVGL and UI END************************/



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
  g_Display.setup();  // Initialize the display

  lv_demo_widgets();    // LVGL demo
  
//  Serial.println( "Setup done" );

}

void loop()
{
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

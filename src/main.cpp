#ifdef ARDUINO 
#include <Arduino.h> 
#else
#include <iostream>
#include <inttypes.h>
#endif

#include "gfx_conf.h"


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

  //Display Prepare
  tft.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  delay(100);

  tft.fillScreen(TFT_BLUE);
  delay(1000);
  tft.fillScreen(TFT_YELLOW);
  delay(1000);
  tft.fillScreen(TFT_GREEN);
  delay(1000);
  tft.fillScreen(TFT_WHITE);
  delay(1000);
  tft.fillScreen(TFT_BLACK);
  tft.fillCircle    ( 100, 100      , 50, TFT_YELLOW);
  tft.setCursor(200, 240);
  tft.print("Hello, Elecrow");
  Serial.println( "Hello, my Display" );
}



void loop()
{
  uint16_t touchX, touchY;
  bool touched = tft.getTouch( &touchX, &touchY);
  if( touched )
  {
    tft.fillCircle(touchX, touchY, 10, TFT_WHITE);
    Serial.print( "Data x " );
    Serial.println( touchX );

    Serial.print( "Data y " );
    Serial.println( touchY );
  }
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

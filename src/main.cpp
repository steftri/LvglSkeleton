#ifdef ARDUINO 
#include <Arduino.h> 
#else
#include <iostream>
#include <inttypes.h>
#endif



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
}



void loop()
{
  static volatile uint8_t value = 0;
  value = value+1;   // do something very simple
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

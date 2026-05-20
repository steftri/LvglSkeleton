#include <Arduino.h>

#include "os.h"


Os::Os()
{
}



void Os::setup()
{
}


void Os::loop()
{
  // In an Arduino context, the loop function can be used for periodic tasks if needed,
  // but for now, we should keep it empty.
}


void Os::log(const char *pc_Message)
{
    // Log the message to the console or a file
    Serial.println(pc_Message);
}

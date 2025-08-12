#include <Arduino.h> 

#include "model.h"
#include "view.h"
#include "controller.h"


static Model g_model;
static View g_view;
Controller g_controller(&g_model, &g_view);


void setup()
{
  Serial.begin(115200);
  Serial.println(APPLICATION_NAME);
  Serial.println(__DATE__ " " __TIME__);

  g_controller.setup();
}

void loop()
{
  g_controller.loop();
}


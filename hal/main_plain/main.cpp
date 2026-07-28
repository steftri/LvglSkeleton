#include <iostream>
#include <inttypes.h>


#include "model.h"
#include "view.h"
#include "controller.h"



static Model g_model;
static View g_view;
Controller g_controller(g_model, g_view);



int main(int argc, char *argv[])
{
  std::cout << APPLICATION_NAME << " " << APPLICATION_VERSION << std::endl;
  std::cout << __DATE__ " " __TIME__ << std::endl;

  g_controller.setup();
  g_controller.begin();
  while (true)
  {
    g_controller.loop();
  }

}

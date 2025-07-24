#include <iostream>
#include <inttypes.h>


#include "model.h"
#include "view.h"
#include "controller.h"



Model g_model;
View g_view;
Controller g_controller(&g_model, &g_view);



int main(int argc, char *argv[])
{
  std::cout << APPLICATION_NAME << std::endl;
  std::cout << __DATE__ " " __TIME__ << std::endl;

  g_controller.setup();
  while (true)
  {
    g_controller.loop();
  }

}

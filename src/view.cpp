#include <lvgl.h>

#include "view.h"

#include "view/lv_demo_widgets.h"




void View::setup(void)
{
  // Initialize the view components here
  // For example, setting up UI elements, loading resources, etc.

  lv_init();  
  m_ui.setup();
  
  m_LvMain.setup();
}


void View::loop(void)
{
  m_ui.loop();
}

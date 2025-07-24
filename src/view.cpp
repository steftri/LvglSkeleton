#include <lvgl.h>

#include "view.h"

#include "view/lv_demo_widgets.h"


void View::setup(void)
{
  // Initialize the view components here
  // For example, setting up UI elements, loading resources, etc.

  lv_init();

  m_ui.setup();

  lv_demo_widgets();    // LVGL demo

}


void View::loop(void)
{
  // Update the view state here
  // This could involve refreshing UI elements, handling user input, etc.
  // For example, you might call a method to redraw the UI or update the display.

  m_ui.loop();
}

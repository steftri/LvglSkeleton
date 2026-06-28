#include "view.h"
#include "controller.h"


extern Controller g_controller; // Declare the global controller instance defined in main.cpp




void View::setup(void)
{
  // Initialization is performed inside the UI task (UiTask::setup)
}


void View::begin(void)
{
  m_Task.begin();
}


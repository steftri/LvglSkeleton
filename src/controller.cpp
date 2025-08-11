#include "controller.h"


Controller::Controller(Model *p_model, View *p_view)
  : mp_model(p_model)
  , mp_view(p_view)
{
  // Constructor implementation (if needed)
}


void Controller::setup(void)
{
  m_os.setup(); // Initialize the OS interface

  // Initialize the model and view
  if(mp_model != nullptr)
  {
    mp_model->setup();
  }
  if(mp_view != nullptr)
  {
    mp_view->setup();
  }
}


void Controller::loop(void)
{
  m_os.loop(); // Call the OS loop function

  // Update the model and view
  if(mp_model != nullptr)
  {
    mp_model->loop();
  }
  if(mp_view != nullptr)
  {
    mp_view->loop();
  }
}


Model *Controller::getModel(void) const
{
  return mp_model;
}


View *Controller::getView(void) const
{
  return mp_view;
}


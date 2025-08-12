#include "controller.h"


Controller::Controller(Model *p_model, View *p_view)
  : mp_model(p_model)
  , mp_view(p_view)
{
  // Constructor implementation (if needed)
}


void Controller::setup(void)
{
  const char *pac_AvailableNetworks[] = {
    "Network1",
    "Network2",
    "Network3"
  };
  mp_model->getData()->getWifiData()->setAvaliableNetworks(pac_AvailableNetworks, 3); // Example networks
  mp_model->getData()->getWifiData()->setSelectedNetwork(0); // Select the first network
  mp_model->getData()->getWifiData()->setNetworkPassword("password123"); // Set password for the selected network
  mp_model->getData()->getWifiData()->setIPAddress(192, 168, 1, 100); // Example IP address

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


#include <Arduino.h>

#include "controller.h"


Controller::Controller(Model &model, View &view)
  : m_model(model)
  , m_view(view)
  , m_surveillanceTask(model.getData().getSurveillanceData())
  , m_wifiTask(model.getSettings().getWifiSettings(), model.getData().getWifiData()) 
{
}


void Controller::setup(void)
{
  m_os.setup(); // Initialize the OS interface

  // Initialize the model and view
  m_model.setup();
  m_view.setup();
}


void Controller::begin(void)
{
  m_surveillanceTask.begin(); // Start the surveillance task
  m_wifiTask.begin(); // Start the Wi-Fi task

  m_model.begin(); // Start any model-related threads
  m_view.begin(); // Start any view-related threads
}



void Controller::loop(void)
{
  delay(10); // Yield CPU time; keep Arduino loop responsive without busy spinning
}


Model &Controller::getModel(void) const
{
  return m_model;
}


View &Controller::getView(void) const
{
  return m_view;
}



WifiTask &Controller::getWifi(void)
{
  return m_wifiTask;
}

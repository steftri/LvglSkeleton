#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model.h"
#include "view.h"


#include "os.h"

#include "controller/surveillance_task.h"
#include "controller/wifi_task.h"
#include "controller/mqtt_task.h"

#include "wifi_hal.h"



class Controller
{
  Model &m_model; // Reference to the model
  View &m_view;   // Reference to the view

  Os m_os; // OS interface

  SurveillanceTask m_surveillanceTask; // Surveillance task
  WifiTask m_wifiTask; // Wi-Fi task
  MqttTask m_mqttTask; // MQTT task

public:
  // Constructor
  Controller(Model &model, View &view);

  // Destructor
  ~Controller() = default;

  // Initialize the controller
  void setup(void);

  // Start the controllers' threads
  void begin(void);

  // Main loop for the controller (Arduino context: called from the main loop)
  void loop(void);

  Model &getModel(void) const;

  View &getView(void) const;

  WifiTask &getWifi(void);
  MqttTask &getMqtt(void);
};

#endif
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model.h"
#include "view.h"


#include "os.h"





class Controller
{
  Model *mp_model; // Pointer to the model
  View *mp_view;   // Pointer to the view

  Os m_os; // OS interface

public:
  // Constructor
  Controller(Model *p_model, View *p_view);

  // Destructor
  ~Controller() = default;

  // Initialize the controller
  void setup(void);

  // Update the controller state
  void loop(void);

  Model *getModel(void) const;

  View *getView(void) const;

};

#endif
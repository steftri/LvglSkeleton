#ifndef VIEW_H
#define VIEW_H

#ifdef ARDUINO
#include "ui_c64.h"
#else
#include "ui.h"
#endif

class View
{
#ifdef ARDUINO
  UiC64 m_ui; // Use C64 UI for Arduino 
#else
  Ui m_ui;
#endif

public:
  // Constructor
  View(void) = default;

  // Destructor
  ~View(void) = default;

  // Initialize the view
  void setup(void);

  // Update the view
  void loop(void);
};

#endif // VIEW_H
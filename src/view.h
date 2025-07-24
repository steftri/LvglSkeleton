#ifndef VIEW_H
#define VIEW_H

#include "ui.h"


class View
{
  Ui m_ui;

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
#ifndef VIEW_H
#define VIEW_H

#include "view/view_task.h"




class View
{
  ViewTask m_Task;

public:
  // Constructor
  View(void) = default;

  // Destructor
  ~View(void) = default;

  // Initialize the view
  void setup(void);

  // Start the view's threads (if any)
  void begin(void);
};

#endif // VIEW_H

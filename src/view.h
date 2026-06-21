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

  // Update the view
  void loop(void);

  LvMain *getLvMain(void);

  void updateSystemInfo(void);

  void updateWlanState(void);
  void updateWlanList(void);

  void showWlanPasswdDialog(void);
};

#endif // VIEW_H

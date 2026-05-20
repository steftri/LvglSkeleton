#ifndef VIEW_H
#define VIEW_H

#include "view/ui_task.h"




class View
{
  UiTask m_UiTask;

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

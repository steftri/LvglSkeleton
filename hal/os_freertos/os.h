#ifndef OS_H
#define OS_H

#include "interfaces/os_interface.h"



class Os : public OsInterface
{
public:
  Os();

  // Setup function to initialize tasks
  void setup();

  // Loop function for periodic updates
  void loop();

  static void log(const char *pc_Message);  
};

#endif // OS_H

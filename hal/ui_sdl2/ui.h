#ifndef UI_H
#define UI_H

#include <inttypes.h>

#include "interfaces/ui_interface.h"



class Ui : public UiInterface
{ 
  uint32_t mu32_LastTick;
public:
    // Constructor
    Ui();

    // Destructor
    virtual ~Ui() = default;

    // Initialize the display
    void setup() override;

    // Update the display
    void loop() override;
};


#endif

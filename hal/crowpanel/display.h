#ifndef DISPLAY_H
#define DISPLAY_H

#include "display_interface.h"


class Display : public DisplayInterface
{ 
public:
    // Constructor
    Display();

    // Destructor
    virtual ~Display() = default;

    // Initialize the display
    void setup() override;

    // Update the display
    void loop() override;
};


#endif

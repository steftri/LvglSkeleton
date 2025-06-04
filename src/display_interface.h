#ifndef DISPLAY_INTERFACE_H
#define DISPLAY_INTERFACE_H

class DisplayInterface
{
public:
    virtual ~DisplayInterface() = default;

    // Initialize the display
    virtual void setup() = 0;

    // Update the display
    virtual void loop() = 0;
};


#endif  // DISPLAY_INTERFACE_H

#ifndef OS_INTERFACE_H
#define OS_INTERFACE_H

class OsInterface
{
public:
    virtual ~OsInterface() = default;

    // Initialize the display
    virtual void setup() = 0;

    // Update the display
    virtual void loop() = 0;
};


#endif  // OS_INTERFACE_H

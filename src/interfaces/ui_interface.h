#ifndef UI_INTERFACE_H
#define UI_INTERFACE_H

class UiInterface
{
public:
    virtual ~UiInterface() = default;

    // Initialize the display
    virtual void setup() = 0;

    // Update the display
    virtual void loop() = 0;

    virtual void setBrightness(uint8_t u8_Brightness) = 0;
};


#endif  // UI_INTERFACE_H

#ifndef UI_C64_H
#define UI_C64_H

#include "ui.h"


const static uint8_t MAX_C64_COLUMNS = 40; // Maximum columns for C64
const static uint8_t MAX_C64_ROWS = 25;    // Maximum rows for C


class UiC64 : public Ui
{
  uint32_t mau32_ScreenBuffer[MAX_C64_COLUMNS][MAX_C64_ROWS]; // C64 screen buffer, 40 columns x 25 rows, codepoint values
  uint8_t mu8_CursorX; // Current cursor X position
  uint8_t mu8_CursorY; // Current cursor Y position

public: 
  UiC64();
  void clearScreen(void);
  void flushScreen(void);
  void scrollScreenUp(void);
  void setCursor(uint8_t u8_Column, uint8_t u8_Row);
  void print(const char *pc_Text);

private:
  void initDisplay(void) override;
  void drawCharacter(uint8_t u8_Column, uint8_t u8_Row);
};


#endif // UI_C64_H
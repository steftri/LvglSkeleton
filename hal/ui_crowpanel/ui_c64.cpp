#include "ui_c64.h"

#include <8bitfonts.h>

// Standardfarben des C64 (Originalfarben, approximiert für 16-bit RGB)
const static uint16_t C64_COLOR_BLACK        = 0x0000;
const static uint16_t C64_COLOR_WHITE        = 0xFFFF;
const static uint16_t C64_COLOR_RED          = 0xF800;
const static uint16_t C64_COLOR_CYAN         = 0x07FF;
const static uint16_t C64_COLOR_PURPLE       = 0x801F;
const static uint16_t C64_COLOR_GREEN        = 0x07E0;
const static uint16_t C64_COLOR_BLUE         = 0x001F;
const static uint16_t C64_COLOR_YELLOW       = 0xFFE0;
const static uint16_t C64_COLOR_ORANGE       = 0xFD20;
const static uint16_t C64_COLOR_BROWN        = 0xA145;
const static uint16_t C64_COLOR_LIGHT_RED    = 0xF81F;
const static uint16_t C64_COLOR_DARK_GRAY    = 0x4208;
const static uint16_t C64_COLOR_GRAY         = 0x8410;
const static uint16_t C64_COLOR_LIGHT_GREEN  = 0x87F0;
const static uint16_t C64_COLOR_LIGHT_BLUE   = 0x7D7C;
const static uint16_t C64_COLOR_LIGHT_GRAY   = 0xC618;


UiC64::UiC64()
  : mu8_CursorX(0)
  , mu8_CursorY(0)
{
  randomSeed(analogRead(0));

  // Initialize the screen buffer
  for (uint8_t u8_Row = 0; u8_Row < MAX_C64_ROWS; u8_Row++)
  {
    for (uint8_t u8_Column = 0; u8_Column < MAX_C64_COLUMNS; u8_Column++)
    {
      mau32_ScreenBuffer[u8_Column][u8_Row] = random(256); // Initialize with random codepoint values
    }
  }  
}


void UiC64::initDisplay(void)
{
  Ui::initDisplay();

  m_DisplayLGFX.fillRect(0, 0, screenWidth, screenHeight, C64_COLOR_LIGHT_BLUE);
  flushScreen();
  clearScreen();
  setCursor(1, 4);
  print("**** COMMODORE 64 BASIC V2 ****");
  setCursor(3, 1);
  print("64K RAM SYSTEM  38911 BASIC BYTES FREE");
  setCursor(5, 0);
  print("READY.\n");
  delay(600);
  print("LOAD\"*\",8,1\n\n");
  print("SEARCHING FOR *\n");
  delay(200);
  print("LOADING\n");
  delay(200);
  print("READY.\n");
  
  delay(600);
  print("RUN");

  delay(600);
}


void UiC64::flushScreen(void)
{
  for (uint8_t u8_Row = 0; u8_Row < MAX_C64_ROWS; u8_Row++)
  {
    for (uint8_t u8_Column = 0; u8_Column < MAX_C64_COLUMNS; u8_Column++)
    {
      drawCharacter(u8_Column, u8_Row);  // Draw the character immediately
    }
  }
}


void UiC64::clearScreen(void)
{
  for (uint8_t u8_Row = 0; u8_Row < MAX_C64_ROWS; u8_Row++)
  {
    for (uint8_t u8_Column = 0; u8_Column < MAX_C64_COLUMNS; u8_Column++)
    {
      mau32_ScreenBuffer[u8_Column][u8_Row]=' ';
    }
  }
  mu8_CursorX = 0; // Reset cursor X position
  mu8_CursorY = 0; // Reset cursor Y position
  flushScreen();
}

void UiC64::setCursor(uint8_t u8_Row, uint8_t u8_Column)
{
  if(u8_Column < MAX_C64_COLUMNS)
  {
    mu8_CursorX = u8_Column;
  }
  if(u8_Row < MAX_C64_ROWS)
  {
    mu8_CursorY = u8_Row;
  }
}


void UiC64::print(const char *pc_Text)
{
  if (pc_Text == nullptr || mu8_CursorX >= MAX_C64_COLUMNS || mu8_CursorY >= MAX_C64_ROWS)
  {
    return; // Invalid input or cursor position
  }

  for (size_t i = 0; i < Utf8::strLength(pc_Text); i++) 
  {
    uint32_t u32_Codepoint = Utf8::toCodepoint(pc_Text, i);

    if (u32_Codepoint == '\n') 
    {
      mu8_CursorX=0;
      if(mu8_CursorY >= MAX_C64_ROWS - 1)
      {
        // scroll the screen up
      }
      else
      {
        mu8_CursorY++;
      }
      continue;
    }
    mau32_ScreenBuffer[mu8_CursorX][mu8_CursorY] = u32_Codepoint;
    drawCharacter(mu8_CursorX, mu8_CursorY); // Draw the character immediately

    mu8_CursorX++;
    if (mu8_CursorX >= MAX_C64_COLUMNS)
    {
      mu8_CursorX = 0; // Reset cursor X position
      if(mu8_CursorY >= MAX_C64_ROWS - 1)
      {
        // scroll the screen up
      }
      else
      {
        mu8_CursorY++;
      }
    }
  }
}

void UiC64::drawCharacter(uint8_t u8_Column, uint8_t u8_Row)
{
  static FontC64 font;  

  if ((u8_Column < MAX_C64_COLUMNS) && (u8_Row < MAX_C64_ROWS))
  {
    uint8_t au8_Bitmap[8];
    uint8_t u8_Width;
    if(font.getCharacterData(&u8_Width, au8_Bitmap, sizeof(au8_Bitmap), mau32_ScreenBuffer[u8_Column][u8_Row]) == Font::ERc::OK) 
    {
      for (uint8_t u8_col = 0; u8_col < u8_Width; u8_col++) 
      {
        uint8_t u8_ColumnData = au8_Bitmap[u8_col]; 
        for(uint8_t i = 0; i < 8; i++) 
        {
          // Draw each bit of the column
          if (u8_ColumnData & (1 << i)) 
          {
            // Set pixel for this bit
            m_DisplayLGFX.drawRect((u8_Column*8+u8_col+(screenWidth/2-8*40)/2)*2, (u8_Row*8+i+(screenHeight/2-8*25)/2)*2, 2, 2, C64_COLOR_LIGHT_BLUE);                     // Use white for set pixels
          } 
          else 
          {
            m_DisplayLGFX.drawRect((u8_Column*8+u8_col+(screenWidth/2-8*40)/2)*2, (u8_Row*8+i+(screenHeight/2-8*25)/2)*2, 2, 2, C64_COLOR_BLUE);                     // Use white for set pixels
          }
        }
      }
    }
  }
} 

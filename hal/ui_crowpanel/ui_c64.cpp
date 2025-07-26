#include "ui_c64.h"

#include <8bitfonts.h>

// Quelle: C64 Farbpalette, bestmöglich auf 5-6-5 RGB gemappt
const static uint16_t C64_COLOR_BLACK        = 0x0000; // #000000 -> R:0 G:0 B:0
const static uint16_t C64_COLOR_WHITE        = 0xFFFF; // #FFFFFF -> R:31 G:31 B:31
const static uint16_t C64_COLOR_RED          = 0xA208; // #813338 -> R:20 G:4 B:8
const static uint16_t C64_COLOR_CYAN         = 0x7BFE; // #75cec8 -> R:15 G:31 B:30
const static uint16_t C64_COLOR_PURPLE       = 0xB0B0; // #8e3c97 -> R:22 G:7 B:16
const static uint16_t C64_COLOR_GREEN        = 0x4FEC; // #56ac4d -> R:10 G:31 B:6
const static uint16_t C64_COLOR_BLUE         = ((0b00011<<11)|(0b001011<<5)|0b01110); // #2e2c9b -> R:5 G:11 B:14
const static uint16_t C64_COLOR_YELLOW       = 0xFFF3; // #edf171 -> R:31 G:31 B:3
const static uint16_t C64_COLOR_ORANGE       = 0xB929; // #8e5029 -> R:22 G:10 B:9
const static uint16_t C64_COLOR_BROWN        = 0x5520; // #553800 -> R:10 G:7 B:0
const static uint16_t C64_COLOR_LIGHT_RED    = 0xE8B2; // #c46c71 -> R:28 G:17 B:18
const static uint16_t C64_COLOR_DARK_GRAY    = 0x4A4A; // #4a4a4a -> R:9 G:9 B:9
const static uint16_t C64_COLOR_GRAY         = 0x7BDE; // #7b7b7b -> R:15 G:15 B:15
const static uint16_t C64_COLOR_LIGHT_GREEN  = 0xBFFF; // #a9ff9f -> R:23 G:31 B:31
const static uint16_t C64_COLOR_LIGHT_BLUE   = ((0b10001<<11)|(0b011010<<5)|0b11011); // #706deb -> R:14 G:27 B:27
const static uint16_t C64_COLOR_LIGHT_GRAY   = 0xB6DA; // #b2b2b2 -> R:22 G:13 B:10

// LIGHT_BLUE:  #706deb = 112, 109, 235 -> R: 112*31/255 = 14, G: 109*63/255 = 27, B: 235*31/255 = 27 -> ((0b01110<<11)|(0b011011<<5)|0b11011)
// BLUE:        #2e2c9b = 46, 44, 155 -> R: 46*31/255 = 5, G: 44*63/255 = 11, B: 155*31/255 = 14 -> ((0b00010<<11)|(0b001011<<5)|0b01110)

static const uint16_t C64_COLOR_PALETTE[] = {
  C64_COLOR_BLACK,        // 0
  C64_COLOR_WHITE,        // 1
  C64_COLOR_RED,          // 2
  C64_COLOR_CYAN,         // 3
  C64_COLOR_PURPLE,       // 4
  C64_COLOR_GREEN,        // 5
  C64_COLOR_BLUE,         // 6
  C64_COLOR_YELLOW,       // 7
  C64_COLOR_ORANGE,       // 8
  C64_COLOR_BROWN,        // 9
  C64_COLOR_LIGHT_RED,    // A
  C64_COLOR_DARK_GRAY,    // B
  C64_COLOR_GRAY,         // C
  C64_COLOR_LIGHT_GREEN,  // D
  C64_COLOR_LIGHT_BLUE,   // E
  C64_COLOR_LIGHT_GRAY    // F
};




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
      mau32_ScreenBuffer[u8_Column][u8_Row] = random(128); // Initialize with random codepoint values
      mau8_ScreenColors[u8_Column][u8_Row] = random(256); // Initialize with random color values (vg and bg)
    }
  }  
}


void UiC64::initDisplay(void)
{
  Ui::initDisplay();

  m_DisplayLGFX.fillRect(0, 0, screenWidth, screenHeight, C64_COLOR_PALETTE[static_cast<uint8_t>(C64Color::LIGHT_BLUE)]);
  flushScreen();
  m_DisplayLGFX.setBrightness(255);
  setColor(C64Color::LIGHT_BLUE, C64Color::BLUE); 
  clearScreen();
  delay(400);
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
  delay(1000);
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


void UiC64::scrollScreenUp(void)
{
  // Scroll the screen up by one row
  for (uint8_t u8_Row = 0; u8_Row < MAX_C64_ROWS - 1; u8_Row++)
  {
    for (uint8_t u8_Column = 0; u8_Column < MAX_C64_COLUMNS; u8_Column++)
    {
      mau32_ScreenBuffer[u8_Column][u8_Row] = mau32_ScreenBuffer[u8_Column][u8_Row + 1];
      mau8_ScreenColors[u8_Column][u8_Row] = mau8_ScreenColors[u8_Column][u8_Row + 1];
    }
  }
  
  // Clear the last row
  for (uint8_t u8_Column = 0; u8_Column < MAX_C64_COLUMNS; u8_Column++)
  {
    mau32_ScreenBuffer[u8_Column][MAX_C64_ROWS - 1] = ' ';
    mau8_ScreenColors[u8_Column][MAX_C64_ROWS - 1] = mu8_Color;
  }
  
  flushScreen();
}


void UiC64::clearScreen(void)
{
  for (uint8_t u8_Row = 0; u8_Row < MAX_C64_ROWS; u8_Row++)
  {
    for (uint8_t u8_Column = 0; u8_Column < MAX_C64_COLUMNS; u8_Column++)
    {
      mau32_ScreenBuffer[u8_Column][u8_Row]=' ';
      mau8_ScreenColors[u8_Column][u8_Row] = mu8_Color;
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

void UiC64::setColor(C64Color e_FgColor, C64Color e_BgColor)
{
  mu8_Color = (static_cast<uint8_t>(e_FgColor) << 4) | static_cast<uint8_t>(e_BgColor); 
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
        scrollScreenUp();
      }
      else
      {
        mu8_CursorY++;
      }
      continue;
    }
    mau32_ScreenBuffer[mu8_CursorX][mu8_CursorY] = u32_Codepoint;
    mau8_ScreenColors[mu8_CursorX][mu8_CursorY] = mu8_Color; // Set the color for the character
    drawCharacter(mu8_CursorX, mu8_CursorY); // Draw the character immediately

    mu8_CursorX++;
    if (mu8_CursorX >= MAX_C64_COLUMNS)
    {
      mu8_CursorX = 0; // Reset cursor X position
      if(mu8_CursorY >= MAX_C64_ROWS - 1)
      {
        scrollScreenUp();
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
    uint16_t u16_ColorFg = C64_COLOR_PALETTE[(mau8_ScreenColors[u8_Column][u8_Row] >> 4) & 0x0F]; // upper nibble: foreground color
    uint16_t u16_ColorBg = C64_COLOR_PALETTE[mau8_ScreenColors[u8_Column][u8_Row] & 0x0F]; // lower nibble: background color
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
            m_DisplayLGFX.drawRect((u8_Column*8+u8_col+(screenWidth/2-8*40)/2)*2, (u8_Row*8+i+(screenHeight/2-8*25)/2)*2, 2, 2, u16_ColorFg);                     // Use white for set pixels
          } 
          else 
          {
            m_DisplayLGFX.drawRect((u8_Column*8+u8_col+(screenWidth/2-8*40)/2)*2, (u8_Row*8+i+(screenHeight/2-8*25)/2)*2, 2, 2, u16_ColorBg);                     // Use white for set pixels
          }
        }
      }
    }
  }
} 

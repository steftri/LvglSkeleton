#include <Arduino.h>
#include <EEPROM.h>

#include "data.h"

static const uint16_t EEPROM_SIZE = 512;  // must be anything between 4 and 4096



Data::Data()
  : mu16_EepromPos(0)
{
}


Data::ERc Data::open(const EMode e_Mode)
{
  // Open the data interface, e.g., initialize EEPROM
  EEPROM.begin(EEPROM_SIZE);
  mu16_EepromPos = 0; // Reset position for reading/writing

  return ERc::Ok;
}


Data::ERc Data::close(void)
{
  // Close the data interface, e.g., finalize EEPROM
  EEPROM.commit(); // Ensure data is written to EEPROM
  EEPROM.end();
  return ERc::Ok;
}


Data::ERc Data::write(const uint8_t u8_Data)
{
  EEPROM.write(mu16_EepromPos++, u8_Data);
  return ERc::Ok;
}



Data::ERc Data::write(const uint8_t *pu8_Buffer, const uint16_t u16_Size)
{
  if (pu8_Buffer == nullptr || u16_Size == 0) 
  {
    return ERc::Error;
  }

  for (uint16_t i = 0; i < u16_Size; i++) 
  {
    EEPROM.write(mu16_EepromPos++, pu8_Buffer[i]);
  }
  
  return ERc::Ok;
}



uint8_t Data::read(void)
{
  return EEPROM.read(mu16_EepromPos++);
}


Data::ERc Data::read(uint8_t *pu8_Buffer, const uint16_t u16_Size)
{
  if (pu8_Buffer == nullptr || u16_Size == 0) {
    return ERc::Error;
  }

  for (uint16_t i = 0; i < u16_Size; ++i) {
    pu8_Buffer[i] = EEPROM.read(mu16_EepromPos++);
  }
  
  return ERc::Ok;
}

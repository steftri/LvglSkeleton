#include <Arduino.h>
#include <EEPROM.h>

#include "datastorage.h"

static const uint16_t EEPROM_SIZE = 1024;  // must be anything between 4 and 4096



DataStorage::DataStorage()
  : mu16_EepromPos(0)
{
}


DataStorage::ERc DataStorage::open(const EMode e_Mode)
{
  // Open the data interface, e.g., initialize EEPROM
  EEPROM.begin(EEPROM_SIZE);
  mu16_EepromPos = 0; // Reset position for reading/writing

  return ERc::Ok;
}


DataStorage::ERc DataStorage::close(void)
{
  // Close the data interface, e.g., finalize EEPROM
  EEPROM.commit(); // Ensure data is written to EEPROM
  EEPROM.end();
  return ERc::Ok;
}


DataStorage::ERc DataStorage::write(const uint8_t u8_Data)
{
  EEPROM.write(mu16_EepromPos++, u8_Data);
  return ERc::Ok;
}



DataStorage::ERc DataStorage::write(const uint8_t *pu8_Buffer, const uint16_t u16_Size)
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



uint8_t DataStorage::read(void)
{
  return EEPROM.read(mu16_EepromPos++);
}


DataStorage::ERc DataStorage::read(uint8_t *pu8_Buffer, const uint16_t u16_Size)
{
  if (pu8_Buffer == nullptr || u16_Size == 0) {
    return ERc::Error;
  }

  for (uint16_t i = 0; i < u16_Size; ++i) {
    pu8_Buffer[i] = EEPROM.read(mu16_EepromPos++);
  }
  
  return ERc::Ok;
}

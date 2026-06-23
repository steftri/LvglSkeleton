#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include "interfaces/datastorage_interface.h"


class DataStorage : public DataStorageInterface
{
  uint16_t mu16_EepromPos; // Position in EEPROM for reading/writing
  EMode me_Mode; // Current mode of the data interface (Read/Write)

public:
  DataStorage();

  ERc open(const EMode e_Mode) override;
  ERc close(void) override;

  ERc write(const uint8_t u8_Data) override;  
  ERc write(const uint8_t *pu8_Buffer, const uint16_t u16_Size) override;

  uint8_t read(void) override;
  ERc read(uint8_t *pu8_Buffer, const uint16_t u16_Size) override;
};


#endif // DATASTORAGE_H

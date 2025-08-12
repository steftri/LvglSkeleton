#ifndef DATA_H
#define DATA_H

#include <stdio.h>

#include "interfaces/data_interface.h"


class Data : public DataInterface
{
  FILE *mp_File; // Pointer to the file for reading/writing
  EMode me_Mode; // Current mode of the data interface (Read/Write)

public:
  Data();

  ERc open(const EMode e_Mode) override;
  ERc close(void) override;

  ERc write(const uint8_t u8_Data) override;  
  ERc write(const uint8_t *pu8_Buffer, const uint16_t u16_Size) override;

  uint8_t read(void) override;
  ERc read(uint8_t *pu8_Buffer, const uint16_t u16_Size) override;
};


#endif // DATA_H

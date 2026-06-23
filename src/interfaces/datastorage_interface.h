#ifndef DATASTORAGE_INTERFACE_H
#define DATASTORAGE_INTERFACE_H

#include <inttypes.h>


class DataStorageInterface
{
public:
  enum class ERc
  {
    Ok = 0,
    Error
  };

  enum class EMode
  {
    Read = 0,
    Write
  };

  virtual ~DataStorageInterface() = default; 

  virtual ERc open(const EMode e_Mode) = 0;
  virtual ERc close(void) = 0;

  virtual ERc write(const uint8_t u8_Data) = 0;
  virtual ERc write(const uint8_t *pu8_Buffer, const uint16_t u16_Size) = 0;

  virtual uint8_t read(void) = 0;
  virtual ERc read(uint8_t *pu8_Buffer, const uint16_t u16_Size) = 0;
};  



#endif  // DATASTORAGE_INTERFACE_H
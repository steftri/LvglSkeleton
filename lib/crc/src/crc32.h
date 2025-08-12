#ifndef CRC_H
#define CRC_H

#include <inttypes.h>

class CRC32
{
  uint32_t mu32_CRC;
  static const uint32_t mau32_LookupTable[16];

public:
  CRC32(void);
  ~CRC32() = default;
  
  void init(void);

  void add(const uint8_t value);

  void add(const uint8_t *pu8_Buffer, const uint16_t u16_Size);

  uint32_t getResult(void);

  static uint32_t calc(const uint8_t *pu8_Buffer, const uint16_t u16_Size);
};

#endif // CRC_H
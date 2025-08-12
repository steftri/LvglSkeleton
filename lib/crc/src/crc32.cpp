
#include "crc32.h"



static const uint32_t CRC32_INITIAL = 0xFFFFFFFF;
static const uint32_t CRC32_XOR_OUT = 0xFFFFFFFF; // Final XOR value for CRC32


// This implementation uses a nibble-based approach for CRC32 calculation.
// Each byte is processed in two 4-bit chunks (nibbles), which reduces the size of the lookup table to 16 entries.
// The lookup table is generated based on the polynomial used for CRC32.
const uint32_t CRC32::mau32_LookupTable[16] = 
{
  0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
  0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
  0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
  0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};


CRC32::CRC32(void)
  : mu32_CRC(CRC32_INITIAL) // Initialize CRC with the initial value
{
}


void CRC32::init(void)
{
  mu32_CRC = CRC32_INITIAL; // Set the initial CRC value
}


void CRC32::add(const uint8_t value)
{
  // Update the CRC with a single byte using the nibble-based approach
  // Process the lower nibble (4 bits) of the byte
  mu32_CRC = (mu32_CRC >> 4) ^ mau32_LookupTable[(mu32_CRC & 0x0F) ^ (value & 0x0F)];
  // Process the upper nibble (4 bits) of the byte
  mu32_CRC = (mu32_CRC >> 4) ^ mau32_LookupTable[(mu32_CRC & 0x0F) ^ (value >> 4)];
}


void CRC32::add(const uint8_t *pu8_Buffer, const uint16_t u16_Size)
{
  if (pu8_Buffer == nullptr || u16_Size == 0) 
  {
    return; // No data to add
  }

  // Update the CRC with a buffer of bytes
  for (uint16_t i = 0; i < u16_Size; i++)
  {
    add(pu8_Buffer[i]);
  }
}

uint32_t CRC32::getResult(void)
{
  // Return the current CRC value
  return mu32_CRC ^ CRC32_XOR_OUT; // XOR with initial value to finalize
}


uint32_t CRC32::calc(const uint8_t *pu8_Buffer, const uint16_t u16_Size)
{
  CRC32 crc;
  crc.add(pu8_Buffer, u16_Size);
  return crc.getResult();
}
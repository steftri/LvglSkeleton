

#include "datastorage.h"


DataStorage::DataStorage()
  : mp_File(nullptr) // Initialize file pointer to nullptr
{
}


DataStorage::ERc DataStorage::open(const EMode e_Mode)
{
  mp_File = fopen("datafile.bin", (e_Mode==EMode::Read)?"rb":"wb"); // Open file in binary mode
  if (mp_File == nullptr) 
  {
    return ERc::Error; // Return error if file could not be opened
  }

  return ERc::Ok;
}


DataStorage::ERc DataStorage::close(void)
{
  if (mp_File != nullptr) 
  {
    fclose(mp_File); // Close the file if it was opened
    mp_File = nullptr; // Reset file pointer
  }
  return ERc::Ok;
}


DataStorage::ERc DataStorage::write(const uint8_t u8_Data)
{
  if (mp_File == nullptr) 
  {
    return ERc::Error; // Return error if file is not open
  }

  size_t written = fwrite(&u8_Data, sizeof(uint8_t), 1, mp_File);
  if (written != 1)
  {
    return ERc::Error; // Return error if data could not be written
  }
  
  return ERc::Ok;
}


DataStorage::ERc DataStorage::write(const uint8_t *pu8_Buffer, const uint16_t u16_Size)
{
  if (mp_File == nullptr || pu8_Buffer == nullptr || u16_Size == 0) 
  {
    return ERc::Error;
  }

  size_t written = fwrite(pu8_Buffer, sizeof(uint8_t), u16_Size, mp_File);
  if (written != u16_Size)
  {
    return ERc::Error; // Return error if not all data could be written
  }
  
  return ERc::Ok;
}


uint8_t DataStorage::read(void)
{
  if (mp_File == nullptr) 
  {
    return 0; // Return error if file is not open
  }

  uint8_t u8_Data;
  size_t read = fread(&u8_Data, sizeof(uint8_t), 1, mp_File);
  if (read != 1)
  {
    return 0; // Return error if data could not be read
  }
  
  return u8_Data;
}


DataStorage::ERc DataStorage::read(uint8_t *pu8_Buffer, const uint16_t u16_Size)
{
  if (mp_File == nullptr || pu8_Buffer == nullptr || u16_Size == 0) 
  {
    return ERc::Error;
  }

  size_t read = fread(pu8_Buffer, sizeof(uint8_t), u16_Size, mp_File);
  if (read != u16_Size)
  {
    return ERc::Error; // Return error if not all data could be read
  }
  
  return ERc::Ok;
}

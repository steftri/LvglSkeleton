#ifndef WORKER_SETTINGS_H
#define WORKER_SETTINGS_H

#include <inttypes.h>

#include "data.h"

class WorkerSettings : public Data
{
  bool mb_EnableState; ///< Indicates whether the Wi-Fi station mode is enabled
  
public:
  static const uint16_t WORKER_SETTINGS_SIZE = 1UL; 
  enum class EField : uint8_t 
  { 
    EnableState = 0
  };

  WorkerSettings(void);


  void init(void);
  void setEnable(bool b_Enable);
  bool getEnable(void) const;

  uint16_t serialize(uint8_t *pu8_Buffer, const uint16_t u16_BufferSize);
  void unserialize(const uint8_t *pu8_Buffer, const uint16_t u16_Size);
};

#endif

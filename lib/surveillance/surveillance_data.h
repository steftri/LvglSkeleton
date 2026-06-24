#ifndef SURVEILLANCE_DATA_H
#define SURVEILLANCE_DATA_H

#include <inttypes.h>
#include <stddef.h>

#include "data.h"


class SurveillanceData : public Data
{
public:
  enum class EField : uint8_t 
  { 
    Uptime = 0,
    Tasks,
    FreeHeapSize
  };


private:
  uint32_t mu32_Uptime; ///< Uptime in seconds
  uint32_t mu32_Tasks; ///< Number of tasks
  uint32_t mu32_FreeHeapSize; ///< Free heap size in bytes
  uint32_t mu32_MinimumEverFreeHeapSize; ///< Minimum ever free heap size in bytes

public:
  SurveillanceData();
  ~SurveillanceData() = default;

  void setUptime(uint32_t u32_Uptime);
  uint32_t getUptime(void) const;

  void setTasks(uint32_t u32_Tasks);
  uint32_t getTasks(void) const;

  void setFreeHeapSize(uint32_t u32_FreeHeapSize, uint32_t u32_MinimumEverFreeHeapSize);
  uint32_t getFreeHeapSize(void) const;
  uint32_t getMinimumEverFreeHeapSize(void) const;
};

#endif // SURVEILLANCE_DATA_H
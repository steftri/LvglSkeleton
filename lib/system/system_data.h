#ifndef SYSTEM_DATA_H
#define SYSTEM_DATA_H

#include <inttypes.h>

#include "data.h"


class SystemData : public Data
{
public:
  enum class EField : uint8_t
  {
  };

private:

public:
  SystemData();
  ~SystemData() = default;
};


#endif // SYSTEM_DATA_H

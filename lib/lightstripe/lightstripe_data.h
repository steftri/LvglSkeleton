#ifndef LIGHTSTRIPE_DATA_H
#define LIGHTSTRIPE_DATA_H

#include <inttypes.h>
#include <stddef.h>

#include "data.h"


class LightstripeData : public Data
{
public:
  LightstripeData();
  ~LightstripeData() = default;
};

#endif // LIGHTSTRIPE_DATA_H
#ifndef WORKER_DATA_H
#define WORKER_DATA_H

#include <inttypes.h>
#include <stddef.h>

#include "data.h"


class WorkerData : public Data
{
public:
  WorkerData();
  ~WorkerData() = default;
};

#endif // WORKER_DATA_H
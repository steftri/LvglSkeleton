
#include "data.h"


Data::Data(void)
  : mu8_ObserverCount(0)
{
}



void Data::registerObserver(DataObserverInterface *p_Observer)
{
  std::lock_guard<std::mutex> lock(m_ObserverMutex);
  if (mu8_ObserverCount < MAX_OBSERVERS)
  {
    map_Observers[mu8_ObserverCount++] = p_Observer;
  }
}


void Data::unregisterObserver(DataObserverInterface *p_Observer)
{
  std::lock_guard<std::mutex> lock(m_ObserverMutex);
  for (uint8_t i = 0; i < mu8_ObserverCount; ++i)
  {
    if (map_Observers[i] == p_Observer)
    {
      map_Observers[i] = map_Observers[--mu8_ObserverCount];
      break;
    }
  }
}


void Data::notifyObservers(EDataField e_Field)
{
  DataObserverInterface *snapshot[MAX_OBSERVERS];
  uint8_t u8_Count = 0;

  {
    std::lock_guard<std::mutex> lock(m_ObserverMutex);
    u8_Count = mu8_ObserverCount;
    for (uint8_t i = 0; i < u8_Count; ++i)
    {
      snapshot[i] = map_Observers[i];
    }
  }

  for (uint8_t i = 0; i < u8_Count; ++i)
  {
    snapshot[i]->onDataChanged(*this, e_Field);
  }
}


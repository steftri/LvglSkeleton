
#include "data.h"


Data::Data(void)
  : mu8_ObserverCount(0)
{
}


void Data::init(void)
{
}


void Data::registerObserver(DataObserverInterface *p_Observer)
{
  std::lock_guard<std::mutex> lock(m_ObserverMutex);
  if (mu8_ObserverCount < MAX_OBSERVERS)
  {
    m_Observers[mu8_ObserverCount++] = p_Observer;
  }
}


void Data::unregisterObserver(DataObserverInterface *p_Observer)
{
  std::lock_guard<std::mutex> lock(m_ObserverMutex);
  for (uint8_t i = 0; i < mu8_ObserverCount; ++i)
  {
    if (m_Observers[i] == p_Observer)
    {
      m_Observers[i] = m_Observers[--mu8_ObserverCount];
      break;
    }
  }
}


void Data::notifyObservers(void)
{
  DataObserverInterface *snapshot[MAX_OBSERVERS];
  uint8_t u8_Count = 0;

  { 
    std::lock_guard<std::mutex> lock(m_ObserverMutex);
    u8_Count = mu8_ObserverCount;
    for (uint8_t i = 0; i < u8_Count; ++i)
    {
      snapshot[i] = m_Observers[i];
    }
  }

  for (uint8_t i = 0; i < u8_Count; ++i)
  {
    snapshot[i]->onDataChanged(*this);
  }
}


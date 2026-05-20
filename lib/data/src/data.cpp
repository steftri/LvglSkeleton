
#include "data.h"

#include <algorithm>


Data::Data(void)
{
}


void Data::init(void)
{
}


void Data::registerObserver(DataObserverInterface *p_Observer)
{
  std::lock_guard<std::mutex> lock(m_ObserverMutex);
  m_Observers.push_back(p_Observer);
}


void Data::unregisterObserver(DataObserverInterface *p_Observer)
{
  std::lock_guard<std::mutex> lock(m_ObserverMutex);
  m_Observers.erase(
    std::remove(m_Observers.begin(), m_Observers.end(), p_Observer),
    m_Observers.end()
  );
}


void Data::notifyObservers(void)
{
  std::vector<DataObserverInterface *> snapshot;
  {
    std::lock_guard<std::mutex> lock(m_ObserverMutex);
    snapshot = m_Observers;
  }

  for (auto *p_Observer : snapshot)
  {
    p_Observer->onDataChanged(*this);
  }
}


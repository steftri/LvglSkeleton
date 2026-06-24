#include "surveillance_data.h"


SurveillanceData::SurveillanceData()
  : mu32_Uptime(0)
{
}



void SurveillanceData::setUptime(uint32_t u32_Uptime)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  if(mu32_Uptime != u32_Uptime)
  {
    mu32_Uptime = u32_Uptime;
    notifyObservers(static_cast<EDataField>(EField::Uptime));
  }
}



uint32_t SurveillanceData::getUptime(void) const
{
  return mu32_Uptime;
}


void SurveillanceData::setTasks(uint32_t u32_Tasks)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  if(mu32_Tasks != u32_Tasks)
  {
    mu32_Tasks = u32_Tasks;
    notifyObservers(static_cast<EDataField>(EField::Tasks));
  }
}


uint32_t SurveillanceData::getTasks(void) const
{
  return mu32_Tasks;
}




void SurveillanceData::setFreeHeapSize(uint32_t u32_FreeHeapSize, uint32_t u32_MinimumEverFreeHeapSize)
{
  bool b_DataChanged = false;

  std::lock_guard<std::mutex> lock(m_DataMutex);

  if(mu32_FreeHeapSize != u32_FreeHeapSize)
  {
    mu32_FreeHeapSize = u32_FreeHeapSize;
    b_DataChanged = true;
  }

  if(mu32_MinimumEverFreeHeapSize != u32_MinimumEverFreeHeapSize)
  {
    mu32_MinimumEverFreeHeapSize = u32_MinimumEverFreeHeapSize;
    b_DataChanged = true;
  }

  if(b_DataChanged)
  {
    notifyObservers(static_cast<EDataField>(EField::FreeHeapSize));
  }
}


uint32_t SurveillanceData::getFreeHeapSize(void) const
{
  return mu32_FreeHeapSize;
}



uint32_t SurveillanceData::getMinimumEverFreeHeapSize(void) const
{
  return mu32_MinimumEverFreeHeapSize;
}

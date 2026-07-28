#include "surveillance_data.h"


SurveillanceData::SurveillanceData()
  : mu32_Uptime(0)
{
}



void SurveillanceData::setUptime(uint32_t u32_Uptime)
{
  bool b_Changed = false;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);

    if(mu32_Uptime != u32_Uptime)
    {
      mu32_Uptime = u32_Uptime;
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::Uptime));
  }
}



uint32_t SurveillanceData::getUptime(void) const
{
  return mu32_Uptime;
}


void SurveillanceData::setTasks(uint32_t u32_Tasks)
{
  bool b_Changed = false;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);

    if(mu32_Tasks != u32_Tasks)
    {
      mu32_Tasks = u32_Tasks;
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::Tasks));
  }
}


uint32_t SurveillanceData::getTasks(void) const
{
  return mu32_Tasks;
}




void SurveillanceData::setFreeHeapSize(uint32_t u32_FreeHeapTotal, uint32_t u32_MinimumEverFreeHeap, uint32_t u32_FreeHeapInternal, uint32_t u32_FreePSRAM)
{
  bool b_DataChanged = false;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);

    if(mu32_FreeHeapSizeTotal != u32_FreeHeapTotal)
    {
      mu32_FreeHeapSizeTotal = u32_FreeHeapTotal;
      b_DataChanged = true;
    }

    if(mu32_MinimumEverFreeHeapSize != u32_MinimumEverFreeHeap)
    {
      mu32_MinimumEverFreeHeapSize = u32_MinimumEverFreeHeap;
      b_DataChanged = true;
    }

    if(mu32_FreeHeapSizeInternal != u32_FreeHeapInternal)
    {
      mu32_FreeHeapSizeInternal = u32_FreeHeapInternal;
      b_DataChanged = true;
    }

    if(mu32_FreePSRAM != u32_FreePSRAM)
    {
      mu32_FreePSRAM = u32_FreePSRAM;
      b_DataChanged = true;
    }
  }

  if(b_DataChanged)
  {
    notifyObservers(static_cast<EDataField>(EField::FreeHeapSize));
  }
}




uint32_t SurveillanceData::getFreeHeapSizeTotal(void) const
{
  return mu32_FreeHeapSizeTotal;
}



uint32_t SurveillanceData::getMinimumEverFreeHeapSize(void) const
{
  return mu32_MinimumEverFreeHeapSize;
}


uint32_t SurveillanceData::getFreeHeapSizeInternal(void) const
{
  return mu32_FreeHeapSizeInternal;
}


uint32_t SurveillanceData::getFreePsramSize(void) const
{
  return mu32_FreePSRAM;
}

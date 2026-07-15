#include "system_data.h"


SystemData::SystemData()
  : mf32_SocPercent(0.0f)
  , mf32_ChargingCurrentA(0.0f)
  , mu16_RemainingTimeMin(0)
{
}


void SystemData::setSocPercent(float f32_SocPercent)
{
  bool b_Changed = false;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    if (mf32_SocPercent != f32_SocPercent)
    {
      mf32_SocPercent = f32_SocPercent;
      b_Changed = true;
    }
  }

  if (b_Changed)
    notifyObservers(static_cast<EDataField>(EField::SocPercent));
}


float SystemData::getSocPercent(void) const
{
  return mf32_SocPercent;
}


void SystemData::setChargingCurrentA(float f32_ChargingCurrentA)
{
  bool b_Changed = false;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    // Use a small epsilon to avoid spurious change notifications for float
    if (f32_ChargingCurrentA < mf32_ChargingCurrentA - 0.05f || f32_ChargingCurrentA > mf32_ChargingCurrentA + 0.05f)
    {
      mf32_ChargingCurrentA = f32_ChargingCurrentA;
      b_Changed = true;
    }
  }

  if (b_Changed)
    notifyObservers(static_cast<EDataField>(EField::ChargingCurrent));
}


float SystemData::getChargingCurrentA(void) const
{
  return mf32_ChargingCurrentA;
}


void SystemData::setRemainingTimeMin(uint16_t u16_RemainingTimeMin)
{
  bool b_Changed = false;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    if (mu16_RemainingTimeMin != u16_RemainingTimeMin)
    {
      mu16_RemainingTimeMin = u16_RemainingTimeMin;
      b_Changed = true;
    }
  }

  if (b_Changed)
    notifyObservers(static_cast<EDataField>(EField::RemainingTime));
}


uint16_t SystemData::getRemainingTimeMin(void) const
{
  return mu16_RemainingTimeMin;
}

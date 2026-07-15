#include "system_data.h"


SystemData::SystemData()
  : mf32_SocPercent(0.0f)
  , mf32_ChargingCurrentA(0.0f)
  , mu16_DurationMin(0)
  , mf32_PowerConsumptionKWh(0.0f)
  , mf32_ChargingSpeedKW(0.0f)
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


void SystemData::setDurationMin(uint16_t u16_DurationMin)
{
  bool b_Changed = false;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    if (mu16_DurationMin != u16_DurationMin)
    {
      mu16_DurationMin = u16_DurationMin;
      b_Changed = true;
    }
  }

  if (b_Changed)
    notifyObservers(static_cast<EDataField>(EField::DurationMin));
}


uint16_t SystemData::getDurationMin(void) const
{
  return mu16_DurationMin;
}


void SystemData::setPowerConsumptionKWh(float f32_PowerConsumptionKWh)
{
  bool b_Changed = false;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    if (f32_PowerConsumptionKWh < mf32_PowerConsumptionKWh - 0.01f || f32_PowerConsumptionKWh > mf32_PowerConsumptionKWh + 0.01f)
    {
      mf32_PowerConsumptionKWh = f32_PowerConsumptionKWh;
      b_Changed = true;
    }
  }

  if (b_Changed)
    notifyObservers(static_cast<EDataField>(EField::PowerConsumptionKWh));
}


float SystemData::getPowerConsumptionKWh(void) const
{
  return mf32_PowerConsumptionKWh;
}


void SystemData::setChargingSpeedKW(float f32_ChargingSpeedKW)
{
  bool b_Changed = false;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    if (f32_ChargingSpeedKW < mf32_ChargingSpeedKW - 0.05f || f32_ChargingSpeedKW > mf32_ChargingSpeedKW + 0.05f)
    {
      mf32_ChargingSpeedKW = f32_ChargingSpeedKW;
      b_Changed = true;
    }
  }

  if (b_Changed)
    notifyObservers(static_cast<EDataField>(EField::ChargingSpeedKW));
}


float SystemData::getChargingSpeedKW(void) const
{
  return mf32_ChargingSpeedKW;
}

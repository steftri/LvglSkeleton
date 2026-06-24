#include "data_container.h"

DataContainer::DataContainer(void)
{
    // Constructor implementation (if needed)
}

WifiData &DataContainer::getWifiData(void)
{
  return m_WifiData;
}


MqttData &DataContainer::getMqttData(void)
{
  return m_MqttData;
}


SurveillanceData &DataContainer::getSurveillanceData(void)
{
  return m_SurveillanceData;
}
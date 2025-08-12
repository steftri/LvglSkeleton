#include "data.h"

Data::Data(void)
{
    // Constructor implementation (if needed)
}

WifiData *Data::getWifiData(void)
{
    return &m_WifiData;
}

#include "wifi_data.h"
#include <cstring>


WifiData::WifiData()
  : mu8_NumberOfAvaliableNetworks(0)
{
    // Initialize the available networks to empty strings
    for (uint8_t i = 0; i < MAX_WIFI_NETWORKS; ++i)
    {
        mac_AvailableNetworks[i][0] = '\0';
    }
    
    // Initialize the selected network to empty strings
    m_SelectedNetwork.ac_SSID[0] = '\0';
    m_SelectedNetwork.ac_Password[0] = '\0';

    // Initialize IP address to zero
    memset(mau8_IPAddress, 0, sizeof(mau8_IPAddress));
}



void WifiData::setState(EState e_State)
{
    me_State = e_State;
}


WifiData::EState WifiData::getState(void) const
{
    return me_State;
}


void WifiData::setAvaliableNetworks(const char **ppc_Networks, const uint8_t u8_NetworkCount)
{
    mu8_NumberOfAvaliableNetworks = (u8_NetworkCount > MAX_WIFI_NETWORKS) ? MAX_WIFI_NETWORKS : u8_NetworkCount;
    for (uint8_t i = 0; i < mu8_NumberOfAvaliableNetworks; ++i)
    {
        strncpy(mac_AvailableNetworks[i], ppc_Networks[i], MAX_SSID_LENGTH);
        mac_AvailableNetworks[i][MAX_SSID_LENGTH] = '\0'; // Ensure null-termination
    }
}

uint8_t WifiData::getAvailableNetworkCount(void) const
{
    return mu8_NumberOfAvaliableNetworks;
}

void WifiData::getAvailableNetwork(char *pc_SsidBuffer, const size_t bufferSize, const uint8_t u8_Index) const
{
    if(bufferSize == 0 || pc_SsidBuffer == nullptr)
    {
        return; // Invalid buffer
    }
    if (u8_Index < mu8_NumberOfAvaliableNetworks)
    {
        strncpy(pc_SsidBuffer, mac_AvailableNetworks[u8_Index], bufferSize - 1);
        pc_SsidBuffer[bufferSize - 1] = '\0'; // Ensure null-termination
    }
    else
    {
        pc_SsidBuffer[0] = '\0'; // Return an empty string if index is invalid
    }
}

void WifiData::setSelectedNetwork(uint8_t u8_Index)
{
    if (u8_Index < mu8_NumberOfAvaliableNetworks)
    {
        strncpy(m_SelectedNetwork.ac_SSID, mac_AvailableNetworks[u8_Index], MAX_SSID_LENGTH);
        m_SelectedNetwork.ac_SSID[MAX_SSID_LENGTH] = '\0'; // Ensure null-termination
    }
}

void WifiData::setNetworkPassword(const char *pc_Password)
{
    strncpy(m_SelectedNetwork.ac_Password, pc_Password, MAX_WPA2_PASSWORD_LENGTH);
    m_SelectedNetwork.ac_Password[MAX_WPA2_PASSWORD_LENGTH] = '\0'; // Ensure null-termination
}

void WifiData::getSelectedNetwork(char *pc_SSID, char *pc_Password) const
{
  if(pc_SSID != nullptr)
  {
    strncpy(pc_SSID, m_SelectedNetwork.ac_SSID, MAX_SSID_LENGTH);
    pc_SSID[MAX_SSID_LENGTH] = '\0'; // Ensure null-termination
  }
  if(pc_Password != nullptr)
  {
    strncpy(pc_Password, m_SelectedNetwork.ac_Password, MAX_WPA2_PASSWORD_LENGTH);
    pc_Password[MAX_WPA2_PASSWORD_LENGTH] = '\0'; // Ensure null-termination
  }
}

void WifiData::setIPAddress(const uint8_t u8_IPAddr1, const uint8_t u8_IPAddr2, const uint8_t u8_IPAddr3, const uint8_t u8_IPAddr4)
{
    mau8_IPAddress[0] = u8_IPAddr1;
    mau8_IPAddress[1] = u8_IPAddr2;
    mau8_IPAddress[2] = u8_IPAddr3;
    mau8_IPAddress[3] = u8_IPAddr4;
}

void WifiData::getIPAddress(uint8_t *pu8_IPAddress) const
{
    memcpy(pu8_IPAddress, mau8_IPAddress, sizeof(mau8_IPAddress));
}

#include "wifi_data.h"
#include <cstring>


WifiData::WifiData()
  : mb_Enabled(false) // Wi-Fi is initially disabled
  , mu8_NumberOfAvaliableNetworks(0)
  , me_State(EState::Disconnected)
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
  memset(mac_IPAddress, 0, sizeof(mac_IPAddress));
}


void WifiData::setEnable(bool b_Enable)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);
    
  if(mb_Enabled != b_Enable)
  {
    mb_Enabled = b_Enable;
    notifyObservers(static_cast<EDataField>(EField::EnableState));
  }
}


bool WifiData::isEnabled() const
{
  return mb_Enabled;
}



void WifiData::setState(EState e_State)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  if(me_State != e_State)
  {
    me_State = e_State;
    notifyObservers(static_cast<EDataField>(EField::ConnectionState));
  }
}



WifiData::EState WifiData::getState(void) const
{
  return me_State;
}



void WifiData::setAvaliableNetworks(const char **ppc_Networks, uint8_t u8_NetworkCount)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  mu8_NumberOfAvaliableNetworks = (u8_NetworkCount > MAX_WIFI_NETWORKS) ? MAX_WIFI_NETWORKS : u8_NetworkCount;
  for (uint8_t i = 0; i < mu8_NumberOfAvaliableNetworks; ++i)
  {
    strncpy(mac_AvailableNetworks[i], ppc_Networks[i], MAX_SSID_LENGTH);
    mac_AvailableNetworks[i][MAX_SSID_LENGTH] = '\0'; // Ensure null-termination
  }

  notifyObservers(static_cast<EDataField>(EField::AvailableNetworks));
}



uint8_t WifiData::getAvailableNetworkCount(void) const
{
  return mu8_NumberOfAvaliableNetworks;
}



void WifiData::getAvailableNetwork(char *pc_SsidBuffer, const size_t bufferSize, const uint8_t u8_Index) 
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

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



void WifiData::setSelectedNetwork(const char *pc_SSID, const char *pc_Password)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  if((pc_SSID == nullptr) || (pc_SSID[0] == '\0'))
  {
    return; // Invalid SSID
  }

  strncpy(m_SelectedNetwork.ac_SSID, pc_SSID, MAX_SSID_LENGTH);
  m_SelectedNetwork.ac_SSID[MAX_SSID_LENGTH] = '\0'; // Ensure null-termination

  if((pc_Password != nullptr) && (pc_Password[0] != '\0'))
  {
    strncpy(m_SelectedNetwork.ac_Password, pc_Password, MAX_WPA2_PASSWORD_LENGTH);
    m_SelectedNetwork.ac_Password[MAX_WPA2_PASSWORD_LENGTH] = '\0'; // Ensure null-termination
  }
  else
  {
    m_SelectedNetwork.ac_Password[0] = '\0'; 
  }

  notifyObservers(static_cast<EDataField>(EField::SelectedNetwork));
}


void WifiData::setSelectedNetworkSSID(const char *pc_SSID)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  if((pc_SSID == nullptr) || (pc_SSID[0] == '\0'))
  {
    return; // Invalid SSID
  }
  
  if(strncmp(pc_SSID, m_SelectedNetwork.ac_SSID, MAX_SSID_LENGTH) != 0)
  {
    strncpy(m_SelectedNetwork.ac_SSID, pc_SSID, MAX_SSID_LENGTH);
    m_SelectedNetwork.ac_SSID[MAX_SSID_LENGTH] = '\0'; // Ensure null-termination

    m_SelectedNetwork.ac_Password[0] = '\0'; // Clear the password when changing the SSID

    notifyObservers(static_cast<EDataField>(EField::SelectedNetwork));
  }
}


void WifiData::setSelectedNetworkPassword(const char *pc_Password)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  strncpy(m_SelectedNetwork.ac_Password, pc_Password, MAX_WPA2_PASSWORD_LENGTH);
  m_SelectedNetwork.ac_Password[MAX_WPA2_PASSWORD_LENGTH] = '\0'; // Ensure null-termination
 
  notifyObservers(static_cast<EDataField>(EField::SelectedNetwork));
}



void WifiData::getSelectedNetwork(char *pc_SSID, const size_t ssidBufferSize, char *pc_Password, const size_t passwordBufferSize)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  if((pc_SSID != nullptr) && (ssidBufferSize > 0))
  {
    strncpy(pc_SSID, m_SelectedNetwork.ac_SSID, ssidBufferSize - 1);
    pc_SSID[ssidBufferSize - 1] = '\0'; // Ensure null-termination
  }
  if((pc_Password != nullptr) && (passwordBufferSize > 0))
  {
    strncpy(pc_Password, m_SelectedNetwork.ac_Password, passwordBufferSize - 1);
    pc_Password[passwordBufferSize - 1] = '\0'; // Ensure null-termination
  }
}



void WifiData::setIPAddress(const char *pc_IPAddress)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  strncpy(mac_IPAddress, pc_IPAddress, MAX_IP_ADDRESS_LENGTH);
  mac_IPAddress[MAX_IP_ADDRESS_LENGTH] = '\0'; // Ensure null-termination

  notifyObservers(static_cast<EDataField>(EField::IPAddress));
}



void WifiData::getIPAddress(char *pc_IPAddress, const size_t bufferSize)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  if(bufferSize == 0 || pc_IPAddress == nullptr)
  {
    return; // Invalid buffer
  }

  strncpy(pc_IPAddress, mac_IPAddress, bufferSize - 1);
  pc_IPAddress[bufferSize - 1] = '\0'; // Ensure null-termination
}

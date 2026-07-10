#include "wifi_data.h"
#include <cstring>
#include <ctime>


WifiData::WifiData()
  : mb_Enabled(false) // Wi-Fi is initially disabled
  , me_State(EState::Disconnected)
  , mu8_NumberOfAvaliableNetworks(0)
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
  bool b_Changed = false;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    
    if(mb_Enabled != b_Enable)
    {
      mb_Enabled = b_Enable;
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::EnableState));
  }
}


bool WifiData::isEnabled() const
{
  return mb_Enabled;
}



void WifiData::setState(EState e_State, uint8_t u8_ErrorCode, const char *pc_ErrorMessage)
{
  bool b_StateChanged = false;
  bool b_ErrorChanged = false;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    if(me_State != e_State)
    {
      me_State = e_State;
      b_StateChanged = true;
    }
    if(e_State == EState::Error)
    {
      mu8_LastErrorCode = u8_ErrorCode;
      if(pc_ErrorMessage != nullptr)
      {
        strncpy(mac_LastErrorMessage, pc_ErrorMessage, sizeof(mac_LastErrorMessage) - 1);
        mac_LastErrorMessage[sizeof(mac_LastErrorMessage) - 1] = '\0'; // Ensure null-termination
      }
      else
      {
        mac_LastErrorMessage[0] = '\0'; // Clear the last error message if none provided
      }
      b_ErrorChanged = true;
    }
  }

  if(b_StateChanged)
  {
    notifyObservers(static_cast<EDataField>(EField::ConnectionState));
    if(b_ErrorChanged)
    {
      notifyObservers(static_cast<EDataField>(EField::LastError));
    }
  }
}



WifiData::EState WifiData::getState(void) const
{
  return me_State;
}



uint8_t WifiData::getLastErrorCode(void) const
{
  return mu8_LastErrorCode;
}



const char *WifiData::getLastErrorMessage(void) const
{
  return mac_LastErrorMessage;
}



void WifiData::setAvaliableNetworks(const char **ppc_Networks, uint8_t u8_NetworkCount)
{
  bool b_Changed = false;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);

    if(u8_NetworkCount > MAX_WIFI_NETWORKS)
    {
      u8_NetworkCount = MAX_WIFI_NETWORKS; // Limit to maximum allowed networks
    }

    if(mu8_NumberOfAvaliableNetworks != u8_NetworkCount)
    {
      mu8_NumberOfAvaliableNetworks = u8_NetworkCount;
      b_Changed = true;
    }

    for (uint8_t i = 0; i < mu8_NumberOfAvaliableNetworks; ++i)
    {
      if(strncmp(mac_AvailableNetworks[i], ppc_Networks[i], MAX_SSID_LENGTH) != 0)
      {
        strncpy(mac_AvailableNetworks[i], ppc_Networks[i], MAX_SSID_LENGTH);
        mac_AvailableNetworks[i][MAX_SSID_LENGTH] = '\0'; // Ensure null-termination
        b_Changed = true;
      }
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::AvailableNetworks));
  }
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
  bool b_Changed = false;

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
      b_Changed = true;
    }

    if((pc_Password != nullptr) && (pc_Password[0] != '\0'))
    {
      if(strncmp(pc_Password, m_SelectedNetwork.ac_Password, MAX_WPA2_PASSWORD_LENGTH) != 0)
      {
        strncpy(m_SelectedNetwork.ac_Password, pc_Password, MAX_WPA2_PASSWORD_LENGTH);
        m_SelectedNetwork.ac_Password[MAX_WPA2_PASSWORD_LENGTH] = '\0'; // Ensure null-termination
        b_Changed = true;
      }
    }
    else
    {
      if(m_SelectedNetwork.ac_Password[0] != '\0')
      {
        m_SelectedNetwork.ac_Password[0] = '\0'; // Clear the password
        b_Changed = true;
      }
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::SelectedNetwork));
  }
}


void WifiData::setSelectedNetworkSSID(const char *pc_SSID)
{
  bool b_Changed = false;

  if((pc_SSID == nullptr) || (pc_SSID[0] == '\0'))
  {
    return; // Invalid SSID
  }

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    
    if(strncmp(pc_SSID, m_SelectedNetwork.ac_SSID, MAX_SSID_LENGTH) != 0)
    {
      strncpy(m_SelectedNetwork.ac_SSID, pc_SSID, MAX_SSID_LENGTH);
      m_SelectedNetwork.ac_SSID[MAX_SSID_LENGTH] = '\0'; // Ensure null-termination

      m_SelectedNetwork.ac_Password[0] = '\0'; // Clear the password when changing the SSID
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::SelectedNetwork));
  }
}


void WifiData::setSelectedNetworkPassword(const char *pc_Password)
{
  bool b_Changed = false;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);

    strncpy(m_SelectedNetwork.ac_Password, pc_Password, MAX_WPA2_PASSWORD_LENGTH);
    m_SelectedNetwork.ac_Password[MAX_WPA2_PASSWORD_LENGTH] = '\0'; // Ensure null-termination
    b_Changed = true;
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::SelectedNetwork));
  }
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
  bool b_Changed = false;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);

    if(strncmp(mac_IPAddress, pc_IPAddress, MAX_IP_ADDRESS_LENGTH) != 0)
    {
      strncpy(mac_IPAddress, pc_IPAddress, MAX_IP_ADDRESS_LENGTH);
      mac_IPAddress[MAX_IP_ADDRESS_LENGTH] = '\0'; // Ensure null-termination
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::IPAddress));
  }
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



void WifiData::setLocalTime(const struct tm &r_Timeinfo)
{
  bool b_Changed = false;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);

    if(memcmp(&m_TimeInfo, &r_Timeinfo, sizeof(struct tm)) != 0)
    {
      m_TimeInfo = r_Timeinfo;
      m_UtcTimeOffset = mktime(&m_TimeInfo) - time(nullptr); // Calculate the UTC time offset
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::Time));
  }
}



time_t WifiData::getTime()
{
  std::lock_guard<std::mutex> lock(m_DataMutex);
  return time(nullptr) + m_UtcTimeOffset; // Return the current time adjusted by the UTC offset
}



void WifiData::getLocalTime(struct tm *p_Timeinfo)
{
  if(p_Timeinfo == nullptr)
  {
    return;
  }

  std::lock_guard<std::mutex> lock(m_DataMutex);
  time_t currentTime = time(nullptr) + m_UtcTimeOffset;
  memcpy(p_Timeinfo, localtime(&currentTime), sizeof(struct tm)); // Convert to local time and copy to the provided structure
}

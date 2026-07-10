#include <string.h>

#include <FreeRTOS.h>
#include <task.h>

#include "wifi_hal.h"


static const uint8_t NUMBER_OF_NETWORKS = 3;


static const TickType_t CONNECTION_TIME_TICKS = 2000 / portTICK_PERIOD_MS;
static const TickType_t IP_FETCH_TIME_TICKS = 1000 / portTICK_PERIOD_MS;
static const TickType_t NETWORK_SCAN_TIME_TICKS = 1000 / portTICK_PERIOD_MS;

static const char *NETWORK_LIST[] = 
{
  "Kugelblitz",
  "Hyperraum",
  "Hyperraum-Transit"
};



WifiHal::WifiHal(WifiActionInterface &r_ActionListener)
 : mr_ActionListener(r_ActionListener)
 , mb_Enabled(false)
 , mb_Connected(false)
 , mb_NetworksFound(false)
{
}

void WifiHal::setup()
{
  mb_Connected = false;
  mb_NetworksFound = false;
}

void WifiHal::enable()
{
  mb_Enabled = true;
}

void WifiHal::disable()
{
  mb_Enabled = false;
  mb_Connected = false;
}


void WifiHal::setHostname(const char *pc_Hostname)
{
  (void)pc_Hostname;
}


void WifiHal::scanNetworks()
{
  vTaskDelay(NETWORK_SCAN_TIME_TICKS);
  mb_NetworksFound = true;
  mr_ActionListener.onWifiNetworksUpdated();
}

uint8_t WifiHal::getAvailableNetworkCount() const
{
  return NUMBER_OF_NETWORKS; 
}

void WifiHal::getAvailableNetworkSSID(char *pc_buffer, size_t bufferSize, uint8_t u8_Index) const
{
  if((pc_buffer == nullptr) || (u8_Index >= NUMBER_OF_NETWORKS))
    return;

  strncpy(pc_buffer, NETWORK_LIST[u8_Index], bufferSize);
  pc_buffer[bufferSize - 1] = 0;
}


void WifiHal::getAvailableNetworkSignalStrength(int32_t* ps32_signalStrength, uint8_t u8_index) const
{
  // Implement logic to get the signal strength of the available network at the specified index
}

void WifiHal::connect(const char* ssid, const char* password)
{
  mr_ActionListener.onWifiConnecting();
  vTaskDelay(CONNECTION_TIME_TICKS);

  if(strcmp(ssid, "Hyperraum-Transit") == 0 && strcmp(password, "B-AR 3045") == 0)
  {
    mb_Connected = true;
    mr_ActionListener.onWifiConnected();
    vTaskDelay(IP_FETCH_TIME_TICKS);
    mr_ActionListener.onWifiGotIP();
  }
  else
  {
    mb_Connected = false;
    mr_ActionListener.onWifiConnectionFailed(static_cast<uint8_t>(EWifiConnectionError::WrongPassword));
  }
}

void WifiHal::disconnect()
{
  mb_Connected = false;
  mr_ActionListener.onWifiDisconnected();
}

bool WifiHal::isConnected() const
{
  return mb_Connected; // Placeholder
}

void WifiHal::getIPAddress(char* buffer, size_t bufferSize) const
{
  strncpy(buffer, "192.168.178.33", bufferSize);
  buffer[bufferSize - 1] = 0;
}


int WifiHal::getSignalStrength() const
{
  // Implement logic to get the signal strength of the current Wi-Fi connection
  return 0; // Placeholder
}


const char *WifiHal::disconnectReasonToString(uint8_t u8_Reason) const
{
  switch(u8_Reason)
  {
    case static_cast<uint8_t>(EWifiConnectionError::None):
      return "No error";
    case static_cast<uint8_t>(EWifiConnectionError::WrongPassword):
      return "Wrong password";
    case static_cast<uint8_t>(EWifiConnectionError::NetworkNotFound):
      return "Network not found";
    case static_cast<uint8_t>(EWifiConnectionError::Timeout):
      return "Connection timeout";
    default:
      return "Unknown error";
  }
}  


void WifiHal::configTime(int32_t s32_GmtOffset, int32_t s32_DstOffset, const char *pc_NtpServer)
{
  (void)s32_GmtOffset;
  (void)s32_DstOffset;
  (void)pc_NtpServer;
}


int32_t WifiHal::getLocalTime(struct tm *p_Timeinfo, uint32_t u32_TimeoutMs) const
{
  (void)p_Timeinfo;
  (void)u32_TimeoutMs;
  return 0; // Placeholder
}

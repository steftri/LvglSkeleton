#include <Arduino.h>

#include "wifi_task.h"


static const size_t MAX_NETWORK_COUNT = 16; // Maximum number of networks to handle in the buffer
static const size_t MAX_SSID_LENGTH = 32; // Maximum length of SSID
static const size_t MAX_IP_ADDRESS_LENGTH = 16; // Maximum length of IP address string


WifiTask *WifiTask::mp_thisInstance = nullptr; // Initialize static instance pointer


WifiTask::WifiTask(WifiData &wifiData)
  : mp_TaskHandle(nullptr)
  , m_WifiHal(*this) // Pass the WifiActionInterface reference to the WifiHal
  , m_WifiData(wifiData) // Initialize the reference to the Wi-Fi data
{
  mp_thisInstance = this;
}



void WifiTask::begin(void)
{
  Serial.println("Creating WifiTask");

  mp_TaskHandle = xTaskCreateStatic(
     task,                     // Task function
     "Wifi",                   // Task name
     WIFI_TASK_STACK_SIZE,     // Stack size
     nullptr,                  // Parameters
     1,                        // Priority
     m_TaskStack,              // Task handle
     &m_TaskBuffer             // Static task buffer
  );
}



void WifiTask::enable()
{
  Serial.println("Enabling Wi-Fi");
  m_WifiHal.enable(); // Enable the Wi-Fi hardware
}

void WifiTask::disable()
{
  Serial.println("Disabling Wi-Fi");
  m_WifiHal.disable(); // Disable the Wi-Fi hardware
}




void WifiTask::task(void *pvParameters)
{
  if(mp_thisInstance == nullptr)
  {
    Serial.println("Error: WifiTask instance not set");
    vTaskDelete(nullptr); // Delete the task if instance is not set
    return;
  }

  Serial.println("Wifi Task started");
  mp_thisInstance->setup(); // Call setup once at the beginning

  while (true)
  {
    mp_thisInstance->loop(); // Call loop for periodic updates
  }

}



void WifiTask::setup(void)
{
  Serial.println("WifiTask running.");

  m_WifiHal.setup(); // Initialize the Wi-Fi hardware and start scanning for networks
  m_WifiHal.scanNetworks(); // Start scanning for Wi-Fi networks    
}



void WifiTask::loop(void)
{
  Serial.printf("Free Wifi stack: %u/%u (Usage: %u%%)\n",
                uxTaskGetStackHighWaterMark(NULL), WIFI_TASK_STACK_SIZE,
                ((WIFI_TASK_STACK_SIZE - uxTaskGetStackHighWaterMark(NULL)) * 100) / WIFI_TASK_STACK_SIZE); // NULL = aktueller Task

  // continuously scan networks in the background, 
  // results will be processed in the event handler (WifiHal::onEvent) when the scan is complete
  m_WifiHal.scanNetworks(); // Start scanning for Wi-Fi networks         

  vTaskDelay(pdMS_TO_TICKS(10000)); // Alle 10 Sekunden aktualisieren  
}



// WifiActionInterface implementation
void WifiTask::onWifiNetworksUpdated()
{
  char ac_SsidBuffer[MAX_NETWORK_COUNT][MAX_SSID_LENGTH+1];
  uint8_t u8_NetworkCount = m_WifiHal.getAvailableNetworkCount();

  Serial.printf("Number of Wi-Fi networks found: %u\n", u8_NetworkCount);
  if(u8_NetworkCount > MAX_NETWORK_COUNT)
  {
    Serial.printf("Warning: Number of networks exceeds buffer size (%u). Only the first %u will be processed.\n", u8_NetworkCount, MAX_NETWORK_COUNT);
    u8_NetworkCount = MAX_NETWORK_COUNT; // Limit to the maximum buffer size
  }
  
  // You can also retrieve details about each network if needed
  for (uint8_t i = 0; i < u8_NetworkCount; ++i) 
  {
    int32_t signalStrength; // Variable to hold signal strength
    m_WifiHal.getAvailableNetworkSSID(ac_SsidBuffer[i], sizeof(ac_SsidBuffer[i]), i);
    m_WifiHal.getAvailableNetworkSignalStrength(&signalStrength, i); // You can also get signal strength if needed
    Serial.printf("Network %u: SSID: %s, Signal Strength: %d\n", i, ac_SsidBuffer[i], signalStrength);
  }

  const char *apc_Networks[MAX_NETWORK_COUNT];
  for (uint8_t i = 0; i < u8_NetworkCount; ++i)
  {
    apc_Networks[i] = ac_SsidBuffer[i];
  }
  m_WifiData.setAvaliableNetworks(apc_Networks, u8_NetworkCount); // Update the Wi-Fi data with the available networks
}


void WifiTask::onWifiConnected()
{
  Serial.println("Wi-Fi connected");
  
  m_WifiData.setState(WifiData::EState::Connected); // Update the Wi-Fi connection state in the data
}


void WifiTask::onWifiDisconnected()
{
  Serial.println("Wi-Fi disconnected");
  
  m_WifiData.setState(WifiData::EState::Enabled); // wifi state goes back to enabled but not connected
}


void WifiTask::onWifiGotIP()
{
  char ac_IPAddress[MAX_IP_ADDRESS_LENGTH+1]; // Buffer to hold IP address as string
  m_WifiHal.getIPAddress(ac_IPAddress, sizeof(ac_IPAddress)); // Get the IP address as a string
  Serial.printf("Wi-Fi got IP address: %s\n", ac_IPAddress);

  m_WifiData.setIPAddress(ac_IPAddress); // Update the Wi-Fi data with the obtained IP address
}


void WifiTask::onWifiConnectionFailed(EWifiConnectionError error)
{
  Serial.printf("Wi-Fi connection failed with error: %d\n", static_cast<int>(error));
  // Notify the view to update the Wi-Fi connection status and show an error message if needed
}



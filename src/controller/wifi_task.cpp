#include <Arduino.h>

#include "wifi_task.h"


static const size_t MAX_NETWORK_COUNT = 16; // Maximum number of networks to handle in the buffer
static const size_t MAX_SSID_LENGTH = 32; // Maximum length of SSID
static const size_t MAX_WPA2_PASSWORD_LENGTH = 63; // Maximum length of WPA2 password
static const size_t MAX_IP_ADDRESS_LENGTH = 16; // Maximum length of IP address string


WifiTask *WifiTask::mp_thisInstance = nullptr; // Initialize static instance pointer


enum class ENotificationBits : uint32_t
{
  Enable = (1UL << 0),
  Disable = (1UL << 1),
  Connect = (1UL << 2),
  Disconnect = (1UL << 3)
};



WifiTask::WifiTask(WifiSettings &wifiSettings, WifiData &wifiData)
  : mp_TaskHandle(nullptr)
  , m_WifiHal(*this) // Pass the WifiActionInterface reference to the WifiHal
  , m_WifiSettings(wifiSettings) // Initialize the reference to the Wi-Fi settings
  , m_WifiData(wifiData) // Initialize the reference to the Wi-Fi data
{
  mp_thisInstance = this;
}



void WifiTask::begin(void)
{
  Serial.println("Creating WifiTask");

  mp_TaskHandle = xTaskCreateStaticPinnedToCore(
     task,                     // Task function
     "Wifi",                   // Task name
     WIFI_TASK_STACK_SIZE,     // Stack size
     nullptr,                  // Parameters
     1,                        // Priority
     m_TaskStack,              // Task handle
     &m_TaskBuffer,            // Static task buffer
     0                         // Core 0 
  );
}



void WifiTask::enable()
{
  xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::Enable), eSetBits);
}

void WifiTask::disable()
{
  xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::Disable), eSetBits);
}


void WifiTask::connect()
{
  xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::Connect), eSetBits);
}


void WifiTask::disconnect()
{
  xTaskNotify(mp_TaskHandle, static_cast<uint32_t>(ENotificationBits::Disconnect), eSetBits);
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

  if(m_WifiSettings.getEnableState())
  {
    Serial.println("Wi-Fi is enabled in settings, enabling Wi-Fi hardware...");
    m_WifiData.setEnable(true); 
    m_WifiHal.enable(); // Enable the Wi-Fi hardware if it is enabled in the settings
    
    if(m_WifiSettings.getNetworkCount() > 0) // Check if there are any stored networks in the settings
    {
      // try to connect to the previously selected network if Wi-Fi was enabled
      const char *pc_SSID;
      const char *pc_Password;

      m_WifiSettings.getNetwork(&pc_SSID, &pc_Password, 0);
      m_WifiData.setSelectedNetwork(pc_SSID, pc_Password); 

      Serial.printf("Attempting to connect to previously selected Wi-Fi network \"%s\"...\n", pc_SSID);
      m_WifiHal.connect(pc_SSID, pc_Password); 
    }
  }
}



void WifiTask::loop(void)
{
  static uint32_t lastUpdateTime = 0;
  uint32_t currentTime = millis();
  uint32_t u32_NotifiedValue = 0;

  xTaskNotifyWait(0, 0xffff, &u32_NotifiedValue, pdMS_TO_TICKS(1000)); // Wait for notifications with a timeout
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::Enable))
  {
    actionEnable();
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::Disable))
  {
    actionDisable();
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::Connect))
  {
    actionConnect();
  }
  if (u32_NotifiedValue & static_cast<uint32_t>(ENotificationBits::Disconnect))
  {
    actionDisconnect();
  }

  if (currentTime - lastUpdateTime >= 60*1000UL) // Update every 60 seconds
  {
    lastUpdateTime = currentTime;
    Serial.printf("  Free WifiTask stack: %u/%u (Usage: %u%%)\n",
                  uxTaskGetStackHighWaterMark(nullptr), WIFI_TASK_STACK_SIZE,
                  ((WIFI_TASK_STACK_SIZE - uxTaskGetStackHighWaterMark(nullptr)) * 100) / WIFI_TASK_STACK_SIZE); // nullptr = aktueller Task

    if(m_WifiData.isEnabled()) // enabled, but not connected -> scan for networks to update the list in the view
    {
      m_WifiHal.scanNetworks(); // Periodically scan for Wi-Fi networks to update the list in the view
    }
  }
}


void WifiTask::actionEnable()
{
  Serial.println("Enabling Wi-Fi");
  m_WifiData.setEnable(true); 
  m_WifiSettings.setEnableState(true); // Store the enabled state in the settings for persistence
  m_WifiHal.enable(); // Enable the Wi-Fi hardware
}

void WifiTask::actionDisable()
{
  Serial.println("Disabling Wi-Fi");
  m_WifiHal.disable(); // Disable the Wi-Fi hardware
  m_WifiData.setEnable(false); // Update the Wi-Fi connection state in the data
  m_WifiSettings.setEnableState(false); // Store the disabled state in the settings for persistence
}


void WifiTask::actionConnect()
{
  // Implement logic to connect to the selected Wi-Fi network using m_WifiHal
  char ac_SSID[MAX_SSID_LENGTH + 1];
  char ac_Password[MAX_WPA2_PASSWORD_LENGTH + 1];
  m_WifiData.getSelectedNetwork(ac_SSID, sizeof(ac_SSID), ac_Password, sizeof(ac_Password)); // Get the selected network's SSID and password from the data

  Serial.printf("Connecting to Wi-Fi network \"%s\" with password \"%s\"\n", ac_SSID, ac_Password);
  m_WifiHal.connect(ac_SSID, ac_Password); // Connect to the Wi-Fi network using the HAL
}


void WifiTask::actionDisconnect()
{
  Serial.println("Disconnecting from Wi-Fi");
  m_WifiHal.disconnect(); // Disconnect from the Wi-Fi network using the HAL
  m_WifiData.setState(WifiData::EState::Disconnected);
}











// WifiActionInterface implementation
void WifiTask::onWifiNetworksUpdated()
{
  char ac_SsidBuffer[MAX_NETWORK_COUNT][MAX_SSID_LENGTH+1];
  uint8_t u8_NetworkCount = m_WifiHal.getAvailableNetworkCount();
  uint8_t u8_UniqueNetworkCount = 0;

  Serial.printf("Number of Wi-Fi networks found: %u\n", u8_NetworkCount);
  if(u8_NetworkCount > MAX_NETWORK_COUNT)
  {
    Serial.printf("Warning: Number of networks exceeds buffer size (%u). Only the first %u will be processed.\n", u8_NetworkCount, MAX_NETWORK_COUNT);
    u8_NetworkCount = MAX_NETWORK_COUNT; // Limit to the maximum buffer size
  }
  
  u8_UniqueNetworkCount = 0;
  for (uint8_t i = 0; i < u8_NetworkCount; i++) 
  {
    //int32_t signalStrength; // Variable to hold signal strength
    m_WifiHal.getAvailableNetworkSSID(ac_SsidBuffer[u8_UniqueNetworkCount], sizeof(ac_SsidBuffer[u8_UniqueNetworkCount]), i);

    // check if the SSID is already in the list to avoid duplicates (some networks may appear multiple times with different signal strengths)
    bool b_isDuplicate = false;
    for (uint8_t j = 0; j < u8_UniqueNetworkCount; j++) 
    {
      if (strcmp(ac_SsidBuffer[u8_UniqueNetworkCount], ac_SsidBuffer[j]) == 0) 
      {
        b_isDuplicate = true;
        break;
      }
    }
    if (!b_isDuplicate) 
    {
      u8_UniqueNetworkCount++; // Increment index only for unique SSIDs
    }
  }

  const char *apc_Networks[MAX_NETWORK_COUNT];
  for (uint8_t i = 0; i < u8_UniqueNetworkCount; ++i)
  {
    apc_Networks[i] = ac_SsidBuffer[i];
  }
  m_WifiData.setAvaliableNetworks(apc_Networks, u8_UniqueNetworkCount); // Update the Wi-Fi data with the available networks
}



void WifiTask::onWifiConnected()
{
  char ac_SSID[MAX_SSID_LENGTH + 1];
  char ac_Password[MAX_WPA2_PASSWORD_LENGTH + 1];

  Serial.println("Wi-Fi connected");

  m_WifiData.setState(WifiData::EState::Connected); // Update the Wi-Fi connection state in the data
  m_WifiData.getSelectedNetwork(ac_SSID, sizeof(ac_SSID), ac_Password, sizeof(ac_Password)); // Get the selected network's SSID and password from the data
  Serial.printf("Connected to Wi-Fi network \"%s\" with password \"%s\"\n", ac_SSID, ac_Password);
  m_WifiSettings.setNetwork(ac_SSID, ac_Password); // Store the last connected SSID in the settings for future reference
}


void WifiTask::onWifiDisconnected()
{
  Serial.println("Wi-Fi disconnected");
  
  m_WifiData.setState(WifiData::EState::Disconnected); // wifi state goes back to not connected
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



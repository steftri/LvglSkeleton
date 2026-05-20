#include <Arduino.h>
#include <WiFi.h>

#include "wifi_hal.h"


WifiHal::WifiHal()
{
}


void WifiHal::setup()
{
  Serial.println("WifiHal setup called");

  WiFi.disconnect(); // Disconnect from any previous Wi-Fi connections and erase credentials

  WiFi.mode(WIFI_STA); // Optional
  WiFi.setHostname(APPLICATION_NAME);
  WiFi.onEvent(WifiHal::onEvent);

  WiFi.begin();
}



void WifiHal::scanNetworks()
{
  WifiHal::b_ScanComplete = false;
  WiFi.scanNetworks(true); // Start an asynchronous Wi-Fi scan
}

uint8_t WifiHal::getAvailableNetworkCount() const
{
  int scanResult = WiFi.scanComplete();

  if (scanResult == WIFI_SCAN_RUNNING) 
  {
    return 0; // Scan is still running, no results available yet
  }
  return static_cast<uint8_t>(scanResult); // Return the number of networks found
}

void WifiHal::getAvailableNetworkSSID(char* buffer, size_t bufferSize, uint8_t index) const
{
  if(bufferSize < 1) 
    return;
  strncpy(buffer, WiFi.SSID(index).c_str(), bufferSize - 1); // Copy SSID to buffer, ensuring null-termination
  buffer[bufferSize - 1] = '\0'; // Ensure null-termination
}

void WifiHal::getAvailableNetworkSignalStrength(int* signalStrength, uint8_t index) const
{
  // Implement logic to get the signal strength of the available network at the specified index
}

void WifiHal::connect(const char* ssid, const char* password)
{
  // Implement Wi-Fi connection logic here
} 

void WifiHal::disconnect()
{
  // Implement Wi-Fi disconnection logic here
}

bool WifiHal::isConnected() const
{
  // Implement logic to check if Wi-Fi is connected
  return false; // Placeholder
}

void WifiHal::getIPAddress(char* buffer, size_t bufferSize) const
{
  // Implement logic to get the IP address of the device
}

int WifiHal::getSignalStrength() const
{
  // Implement logic to get the signal strength of the current Wi-Fi connection
  return 0; // Placeholder
}


bool WifiHal::b_ScanComplete = false;

void WifiHal::onEvent(WiFiEvent_t event) 
{
  Serial.printf("[WiFi-event] event: %d\n", event);

  switch (event) 
  {
    case ARDUINO_EVENT_WIFI_READY:               Serial.println("WiFi interface ready"); break;
    case ARDUINO_EVENT_WIFI_SCAN_DONE:           
      Serial.println("Completed scan for access points"); 
      b_ScanComplete = true;
      break;
    case ARDUINO_EVENT_WIFI_STA_START:           Serial.println("WiFi client started"); break;
    case ARDUINO_EVENT_WIFI_STA_STOP:            Serial.println("WiFi clients stopped"); break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:       Serial.println("Connected to access point"); break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:    Serial.println("Disconnected from WiFi access point"); break;
    case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE: Serial.println("Authentication mode of access point has changed"); break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.print("Obtained IP address: ");
      Serial.println(WiFi.localIP());
      break;
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:        Serial.println("Lost IP address and IP address is reset to 0"); break;
    case ARDUINO_EVENT_WPS_ER_SUCCESS:          Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode"); break;
    case ARDUINO_EVENT_WPS_ER_FAILED:           Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode"); break;
    case ARDUINO_EVENT_WPS_ER_TIMEOUT:          Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode"); break;
    case ARDUINO_EVENT_WPS_ER_PIN:              Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode"); break;
    case ARDUINO_EVENT_WIFI_AP_START:           Serial.println("WiFi access point started"); break;
    case ARDUINO_EVENT_WIFI_AP_STOP:            Serial.println("WiFi access point  stopped"); break;
    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:    Serial.println("Client connected"); break;
    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED: Serial.println("Client disconnected"); break;
    case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:   Serial.println("Assigned IP address to client"); break;
    case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:  Serial.println("Received probe request"); break;
    case ARDUINO_EVENT_WIFI_AP_GOT_IP6:         Serial.println("AP IPv6 is preferred"); break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP6:        Serial.println("STA IPv6 is preferred"); break;
    case ARDUINO_EVENT_ETH_GOT_IP6:             Serial.println("Ethernet IPv6 is preferred"); break;
    case ARDUINO_EVENT_ETH_START:               Serial.println("Ethernet started"); break;
    case ARDUINO_EVENT_ETH_STOP:                Serial.println("Ethernet stopped"); break;
    case ARDUINO_EVENT_ETH_CONNECTED:           Serial.println("Ethernet connected"); break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:        Serial.println("Ethernet disconnected"); break;
    case ARDUINO_EVENT_ETH_GOT_IP:              Serial.println("Obtained IP address"); break;
    default:                                    break;
  }
}

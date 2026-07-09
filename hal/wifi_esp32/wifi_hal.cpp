#include <Arduino.h>
#include <WiFi.h>

#include "wifi_hal.h"


WifiHal *WifiHal::mp_thisInstance = nullptr; // Initialize static instance pointer








WifiHal::WifiHal(WifiActionInterface &actionListener)
  : m_actionListener(actionListener)
{
  mp_thisInstance = this; // Set the static instance pointer to this instance
}


void WifiHal::setup()
{
  Serial.println("WifiHal setup called");

  WiFi.useStaticBuffers(true);
  WiFi.disconnect(true); // Disconnect and erase credentials
  WiFi.mode(WIFI_STA); // Optional
  WiFi.setHostname(APPLICATION_NAME);

  WiFi.onEvent(WifiHal::onEvent);
}



void WifiHal::enable()
{
  Serial.println("HAL: Enabling Wi-Fi");
  WiFi.scanNetworks(true); // Start scanning for Wi-Fi networks in the background
}


void WifiHal::disable()
{
  Serial.println("HAL: Disabling Wi-Fi");
  WiFi.disconnect(true); // Disconnect and erase credentials
}


void WifiHal::setHostname(const char *pc_Hostname)
{
  if(pc_Hostname != nullptr)
  {
    WiFi.setHostname(pc_Hostname);
  }
}



void WifiHal::scanNetworks()
{
  Serial.println("HAL: Scanning for Wi-Fi networks");
  WiFi.scanNetworks(true); // Start an asynchronous Wi-Fi scan
}

uint8_t WifiHal::getAvailableNetworkCount() const
{
  int16_t s16_scanResult = WiFi.scanComplete();

  if (s16_scanResult == WIFI_SCAN_RUNNING) 
  {
    return 0; // Scan is still running, no results available yet
  }
  return static_cast<uint8_t>(s16_scanResult); // Return the number of networks found
}

void WifiHal::getAvailableNetworkSSID(char *pc_buffer, size_t bufferSize, uint8_t u8_index) const
{
  if(bufferSize < 1) 
    return;
  strncpy(pc_buffer, WiFi.SSID(u8_index).c_str(), bufferSize - 1); // Copy SSID to buffer, ensuring null-termination
  pc_buffer[bufferSize - 1] = '\0'; // Ensure null-termination
}

void WifiHal::getAvailableNetworkSignalStrength(int32_t *ps32_signalStrength, uint8_t u8_index) const
{
  if(ps32_signalStrength == nullptr) 
    return;
  *ps32_signalStrength = WiFi.RSSI(u8_index); // Get the signal strength (RSSI) of the network at the specified index
}

void WifiHal::connect(const char* pc_Ssid, const char* pc_Password)
{
  // Implement Wi-Fi connection logic here
  WiFi.begin(pc_Ssid, pc_Password); // Start Wi-Fi connection with the provided SSID and password
} 

void WifiHal::disconnect()
{
  // Implement Wi-Fi disconnection logic here
  WiFi.disconnect(true); // Disconnect and erase credentials
}

bool WifiHal::isConnected() const
{
  // Implement logic to check if Wi-Fi is connected
  return WiFi.status() == WL_CONNECTED;
}

void WifiHal::getIPAddress(char* pc_buffer, size_t bufferSize) const
{
  if(pc_buffer == nullptr || bufferSize < 1) 
    return;
  strncpy(pc_buffer, WiFi.localIP().toString().c_str(), bufferSize - 1); // Copy IP address to buffer, ensuring null-termination
  pc_buffer[bufferSize - 1] = '\0'; // Ensure null-termination
}

int WifiHal::getSignalStrength() const
{
  // Implement logic to get the signal strength of the current Wi-Fi connection
  return WiFi.RSSI(); // Return the signal strength (RSSI) of the current Wi-Fi connection
}


void WifiHal::onEvent(arduino_event_id_t event, arduino_event_info_t info)
{
  if(mp_thisInstance == nullptr) 
    return; // No instance to handle the event

  switch (event) 
  {
    case ARDUINO_EVENT_WIFI_READY:               
      Serial.println("WiFi interface ready"); 
      break;
    case ARDUINO_EVENT_WIFI_SCAN_DONE:           
      Serial.println("Completed scan for access points"); 
      mp_thisInstance->m_actionListener.onWifiNetworksUpdated();
      break;
    case ARDUINO_EVENT_WIFI_STA_START:           
      Serial.println("WiFi client started"); 
      break;
    case ARDUINO_EVENT_WIFI_STA_STOP:            
      Serial.println("WiFi clients stopped"); 
      break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:       
      Serial.println("Connected to access point"); 
      mp_thisInstance->m_actionListener.onWifiConnected();
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("Disconnected from WiFi access point");
      if(info.wifi_sta_disconnected.reason == WIFI_REASON_ASSOC_LEAVE)
      {
        // this is no error, but a normal disconnect, e.g. when calling WiFi.disconnect()
        mp_thisInstance->m_actionListener.onWifiDisconnected();
      }
      else
      {
        mp_thisInstance->m_actionListener.onWifiConnectionFailed(info.wifi_sta_disconnected.reason);
      }
      break;
    case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE: 
      Serial.println("Authentication mode of access point has changed"); 
      break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.println("Got IP address");
      mp_thisInstance->m_actionListener.onWifiGotIP();
      break;
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:        
      Serial.println("Lost IP address and IP address is reset to 0"); 
      break;
    case ARDUINO_EVENT_WPS_ER_SUCCESS:          
      Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode"); 
      break;
    case ARDUINO_EVENT_WPS_ER_FAILED:           
      Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode"); 
      break;
    case ARDUINO_EVENT_WPS_ER_TIMEOUT:          
      Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode"); 
      break;
    case ARDUINO_EVENT_WPS_ER_PIN:              
      Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode"); 
      break;
    case ARDUINO_EVENT_WIFI_AP_START:           
      Serial.println("WiFi access point started"); 
      break;
    case ARDUINO_EVENT_WIFI_AP_STOP:            
      Serial.println("WiFi access point  stopped"); 
      break;
    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:    
      Serial.println("Client connected"); 
      break;
    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED: 
      Serial.println("Client disconnected"); 
      break;
    case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:   
      Serial.println("Assigned IP address to client"); 
      break;
    case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:  
      Serial.println("Received probe request"); 
      break;
    case ARDUINO_EVENT_WIFI_AP_GOT_IP6:         
      Serial.println("AP IPv6 is preferred"); 
      break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP6:        
      Serial.println("STA IPv6 is preferred"); 
      break;
    case ARDUINO_EVENT_ETH_GOT_IP6:             
      Serial.println("Ethernet IPv6 is preferred"); 
      break;
    case ARDUINO_EVENT_ETH_START:               
      Serial.println("Ethernet started"); 
      break;
    case ARDUINO_EVENT_ETH_STOP:                
      Serial.println("Ethernet stopped"); 
      break;
    case ARDUINO_EVENT_ETH_CONNECTED:           
      Serial.println("Ethernet connected"); 
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:        
      Serial.println("Ethernet disconnected"); 
      break;
    case ARDUINO_EVENT_ETH_GOT_IP:              
      Serial.println("Obtained IP address"); 
      break;
    default:                                    
      break;
  }
}


const char *WifiHal::disconnectReasonToString(uint8_t u8_Reason) const
{
  switch (u8_Reason)
  {
    case WIFI_REASON_UNSPECIFIED:              return "Unspecified reason";
    case WIFI_REASON_AUTH_EXPIRE:              return "Authentication expired";
    case WIFI_REASON_AUTH_LEAVE:               return "Authentication leave";
    case WIFI_REASON_ASSOC_EXPIRE:             return "Association expired";
    case WIFI_REASON_ASSOC_TOOMANY:            return "Association too many";
    case WIFI_REASON_NOT_AUTHED:               return "Not authenticated";
    case WIFI_REASON_NOT_ASSOCED:              return "Not associated";
    case WIFI_REASON_ASSOC_LEAVE:              return "Association leave";
    case WIFI_REASON_ASSOC_NOT_AUTHED:         return "Association not authenticated";
    case WIFI_REASON_DISASSOC_PWRCAP_BAD:      return "Disassociation due to bad power capability";
    case WIFI_REASON_DISASSOC_SUPCHAN_BAD:     return "Disassociation due to unsupported channel";
    case WIFI_REASON_BSS_TRANSITION_DISASSOC:  return "BSS transition disassociation";
    case WIFI_REASON_IE_INVALID:               return "Invalid IE";
    case WIFI_REASON_MIC_FAILURE:              return "MIC failure";
    case WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT:   return "4-way handshake timeout";
    case WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT: return "Group key update timeout";
    case WIFI_REASON_IE_IN_4WAY_DIFFERS:       return "IE in 4-way differs";
    case WIFI_REASON_GROUP_CIPHER_INVALID:     return "Group cipher invalid";
    case WIFI_REASON_PAIRWISE_CIPHER_INVALID:  return "Pairwise cipher invalid";
    case WIFI_REASON_AKMP_INVALID:             return "AKMP invalid";
    case WIFI_REASON_UNSUPP_RSN_IE_VERSION:    return "Unsupported RSN IE version";
    case WIFI_REASON_INVALID_RSN_IE_CAP:       return "Invalid RSN IE capabilities";
    case WIFI_REASON_802_1X_AUTH_FAILED:       return "802.1X authentication failed";
    case WIFI_REASON_CIPHER_SUITE_REJECTED:    return "Cipher suite rejected";
    case WIFI_REASON_INVALID_PMKID:            return "Invalid PMKID";
    case WIFI_REASON_BEACON_TIMEOUT:           return "Beacon timeout";
    case WIFI_REASON_NO_AP_FOUND:              return "No AP found";
    case WIFI_REASON_AUTH_FAIL:                return "Authentication failed";
    case WIFI_REASON_ASSOC_FAIL:               return "Association failed";
    case WIFI_REASON_HANDSHAKE_TIMEOUT:        return "Handshake timeout";
    case WIFI_REASON_CONNECTION_FAIL:          return "Connection failed";
    case WIFI_REASON_AP_TSF_RESET:             return "AP TSF reset";
    case WIFI_REASON_ROAMING:                  return "Roaming";
    default:                                   return "Unknown reason";
  }
}

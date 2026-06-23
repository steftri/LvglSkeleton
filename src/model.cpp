#include <Arduino.h>

#include "model.h"
#include "os.h"


Model::Model()
  : m_Settings()
  , m_Data()
  , m_Task(m_Settings, m_Data)
{
}


void Model::setup(void)
{  
  // Load settings immediately on setup to ensure they are available before any tasks start

  SettingsContainer::ERc result = m_Settings.load();
  if (result != SettingsContainer::ERc::Ok) 
  {
    // Handle error if needed, e.g., log or set default values
    if (result == SettingsContainer::ERc::NotFoundError) 
    {
      Os::log("Settings not found, using defaults.");
    } 
    else if (result == SettingsContainer::ERc::VersionMismatchError) 
    {
      Os::log("Settings version mismatch, using defaults.");
    } 
    else if (result == SettingsContainer::ERc::VerificationError) 
    {
      Os::log("Settings verification failed, using defaults.");
    }
  }

  Serial.println("Known Wi-Fi networks loaded from settings:");
  for (uint8_t i = 0; i < m_Settings.getWifiSettings().getNetworkCount(); ++i)
  {
    const char *pc_SSID;
    const char *pc_Password;
    m_Settings.getWifiSettings().getNetwork(&pc_SSID, &pc_Password, i);
    if (strlen(pc_SSID) > 0)
    {
      Serial.printf("  %u: %s (%s)\n", i, pc_SSID, pc_Password);
    }
  }
}


void Model::begin(void)
{
  m_Task.begin(); // Start the model task
}



SettingsContainer &Model::getSettings(void)
{
  return m_Settings;
}

DataContainer &Model::getData(void)
{
  return m_Data;
}

ModelTask &Model::getTask(void)
{
  return m_Task;
}


void Model::getWifiPassword(char *pc_Password, size_t passwordBufferSize, const char *pc_SSID)
{
  char ac_SelectedSSID[WifiData::MAX_SSID_LENGTH + 1];

  if(pc_Password == nullptr || passwordBufferSize == 0)
  {
    return; // Invalid parameters, do nothing
  }

  if(pc_SSID == nullptr || strlen(pc_SSID) == 0)
  {
    pc_Password[0] = '\0'; // If SSID is null or empty, return an empty password
    return;
  }

  // if the requested SSID matches the selected SSID and the password is available, return this; 
  m_Data.getWifiData().getSelectedNetwork(ac_SelectedSSID, sizeof(ac_SelectedSSID), pc_Password, passwordBufferSize);
  if(strcmp(ac_SelectedSSID, pc_SSID) == 0 && strlen(pc_Password) > 0)
  {
    Serial.printf("Returning password for selected SSID %s: %s\n", pc_SSID, pc_Password);
    return;
  }
  
  // otherwise, copy the password from the settings (if it exists)
  strncpy(pc_Password, m_Settings.getWifiSettings().getNetworkPassword(pc_SSID), passwordBufferSize - 1);
  pc_Password[passwordBufferSize - 1] = '\0'; // Ensure null-termination
  Serial.printf("Returning password from settings for SSID %s: %s\n", pc_SSID, pc_Password);
}

#include "model.h"
#include "os.h"


Model::Model()
  : m_Settings()
  , m_Data()
  , m_Task()
{
}


void Model::setup(void)
{  
  // Load settings from persistent storage
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
}


void Model::begin(void)
{
  m_Task.begin(); // Start the model task
}


void Model::loop(void)
{
  // Arduino context; the model's loop can be used for periodic updates if needed,
  // but for now, we can keep it empty.
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
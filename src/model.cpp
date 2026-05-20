#include "model.h"
#include "os.h"


void Model::setup(void)
{
  // Initialize the model settings
  m_Settings.setup();
  
  // Load settings from persistent storage
  Settings::ERc result = m_Settings.load();
  if (result != Settings::ERc::Ok) 
  {
    // Handle error if needed, e.g., log or set default values
    if (result == Settings::ERc::NotFoundError) 
    {
      Os::log("Settings not found, using defaults.");
    } 
    else if (result == Settings::ERc::VersionMismatchError) 
    {
      Os::log("Settings version mismatch, using defaults.");
    } 
    else if (result == Settings::ERc::VerificationError) 
    {
      Os::log("Settings verification failed, using defaults.");
    }
  }
}


void Model::begin(void)
{
  // Start any threads or background tasks related to the model if needed
}


void Model::loop(void)
{
  // Arduino context; the model's loop can be used for periodic updates if needed,
  // but for now, we can keep it empty.
}


Settings *Model::getSettings(void)
{
  return &m_Settings;
}

Data *Model::getData(void)
{
  return &m_Data;
}

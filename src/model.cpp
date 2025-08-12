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
    m_Settings.save();
  }
}


void Model::loop(void)
{
  // Update the model state here
  // This could involve processing data, handling events, etc.
  // For example, you might update variables or trigger actions based on certain conditions.
}


Settings *Model::getSettings(void)
{
  return &m_Settings;
}


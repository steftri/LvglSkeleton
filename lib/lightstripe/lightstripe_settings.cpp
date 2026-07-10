#include "lightstripe_settings.h"


/**
 * @brief Constructor for the LightstripeSettings class.
 * 
 * This constructor initializes the LightstripeSettings object by calling the init() method.
 */
LightstripeSettings::LightstripeSettings(void)
{
  init();
}


void LightstripeSettings::init(void)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  mb_EnableState = true; // Wi-Fi station mode is initially enabled
}


/**
 * @brief Sets the Wi-Fi enable state.
 * 
 * This function sets the Wi-Fi enable state. If the state changes,
 * it notifies the observers about the change.
 * 
 * @param b_Enable The new enable state.
 */
void LightstripeSettings::setEnable(bool b_Enable)
{
  bool b_Changed = false;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    
    if(mb_EnableState != b_Enable)
    {
      mb_EnableState = b_Enable;
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::EnableState));
  }
}


/**
 * @brief Retrieves the Wi-Fi enable state.
 * 
 * This function retrieves the current enable state of the Wi-Fi station mode.
 * 
 * @return The current enable state.
 */
bool LightstripeSettings::getEnable(void) const
{
  return mb_EnableState;
}





uint16_t LightstripeSettings::serialize(uint8_t *pu8_Buffer, const uint16_t u16_BufferSize)
{
  uint16_t u16_BufferPos = 0;

  if((!pu8_Buffer) || (u16_BufferSize<LIGHTSTRIPE_SETTINGS_SIZE))
    return 0;

  std::lock_guard<std::mutex> lock(m_DataMutex);

  pu8_Buffer[u16_BufferPos++] = mb_EnableState ? 1 : 0;

  return u16_BufferPos;
}



void LightstripeSettings::unserialize(const uint8_t *pu8_Buffer, const uint16_t u16_Size)
{
  uint16_t u16_BufferPos = 0;

  std::lock_guard<std::mutex> lock(m_DataMutex);

  if((!pu8_Buffer) || (u16_Size<LIGHTSTRIPE_SETTINGS_SIZE))
    return;

  mb_EnableState = pu8_Buffer[u16_BufferPos++] ? true : false;
}

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

  for(uint8_t i = 0; i < MAX_INSTANCES; ++i)
  {
    m_Insance[i].mb_EnableState = false;
    m_Insance[i].mu16_StartPixelIndex = 0;
    m_Insance[i].mu16_NumPixels = 0;
    m_Insance[i].me_ValueTarget = EValueTarget::Brightness;
    m_Insance[i].me_WaveForm = EWaveForm::None;
    m_Insance[i].me_ColorMode = EColorMode::RGB;
    m_Insance[i].mf32_WaveInterval = 1.0f;
    m_Insance[i].mf32_WaveMaxSpeed = 1.0f;
    m_Insance[i].mf32_HueMin = 0.0f;
    m_Insance[i].mf32_HueMax = 360.0f;
    m_Insance[i].mu8_RedMin = 0;
    m_Insance[i].mu8_GreenMin = 0;
    m_Insance[i].mu8_BlueMin = 0;
    m_Insance[i].mu8_RedMax = 255;
    m_Insance[i].mu8_GreenMax = 255;
    m_Insance[i].mu8_BlueMax = 255;
  }
  m_Insance[0].mb_EnableState = true;
  m_Insance[0].mu16_NumPixels = 1;
}


void LightstripeSettings::setEnable(uint8_t u8_Instance, bool b_Enable)
{
  bool b_Changed = false;

  if(u8_Instance >= MAX_INSTANCES)
  {
    return;
  }

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    
    if(m_Insance[u8_Instance].mb_EnableState != b_Enable)
    {
      m_Insance[u8_Instance].mb_EnableState = b_Enable;
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::EnableState));
  }
}



bool LightstripeSettings::getEnable(uint8_t u8_Instance) const
{
  return m_Insance[u8_Instance].mb_EnableState;
}



void LightstripeSettings::setPixelRange(uint8_t u8_Instance, uint16_t u16_StartPixelIndex, uint16_t u16_NumPixels)
{
  bool b_Changed = false;

  if(u8_Instance >= MAX_INSTANCES)
  {
    return;
  }

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    
    if((m_Insance[u8_Instance].mu16_StartPixelIndex != u16_StartPixelIndex) || (m_Insance[u8_Instance].mu16_NumPixels != u16_NumPixels))
    {
      m_Insance[u8_Instance].mu16_StartPixelIndex = u16_StartPixelIndex;
      m_Insance[u8_Instance].mu16_NumPixels = u16_NumPixels;
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::ColorSettings));
  }
}


uint16_t LightstripeSettings::getStartPixelIndex(uint8_t u8_Instance) const
{
  if(u8_Instance >= MAX_INSTANCES)
  {
    return 0;
  }

  return m_Insance[u8_Instance].mu16_StartPixelIndex;
}


uint16_t LightstripeSettings::getNumPixels(uint8_t u8_Instance) const
{
  if(u8_Instance >= MAX_INSTANCES)
  {
    return 0;
  }

  return m_Insance[u8_Instance].mu16_NumPixels;
}

void LightstripeSettings::setValueTarget(uint8_t u8_Instance, EValueTarget e_Target)
{
  bool b_Changed = false;

  if(u8_Instance >= MAX_INSTANCES)
  {
    return;
  }

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    
    if(m_Insance[u8_Instance].me_ValueTarget != e_Target)
    {
      m_Insance[u8_Instance].me_ValueTarget = e_Target;
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::ColorSettings));
  }
}

LightstripeSettings::EValueTarget LightstripeSettings::getValueTarget(uint8_t u8_Instance) const
{
  if(u8_Instance >= MAX_INSTANCES)
  {
    return EValueTarget::Brightness;
  }

  return m_Insance[u8_Instance].me_ValueTarget;
}

void LightstripeSettings::setWaveForm(uint8_t u8_Instance, EWaveForm e_WaveForm)
{
  bool b_Changed = false;

  if(u8_Instance >= MAX_INSTANCES)
  {
    return;
  }

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    
    if(m_Insance[u8_Instance].me_WaveForm != e_WaveForm)
    {
      m_Insance[u8_Instance].me_WaveForm = e_WaveForm;
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::ColorSettings));
  }
}

LightstripeSettings::EWaveForm LightstripeSettings::getWaveForm(uint8_t u8_Instance) const
{
  if(u8_Instance >= MAX_INSTANCES)
  {
    return EWaveForm::None;
  }

  return m_Insance[u8_Instance].me_WaveForm;
}


void LightstripeSettings::setColorMode(uint8_t u8_Instance, EColorMode e_ColorMode)
{
  bool b_Changed = false;

  if(u8_Instance >= MAX_INSTANCES)
  {
    return;
  }

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    
    if(m_Insance[u8_Instance].me_ColorMode != e_ColorMode)
    {
      m_Insance[u8_Instance].me_ColorMode = e_ColorMode;
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::ColorSettings));
  }
}

LightstripeSettings::EColorMode LightstripeSettings::getColorMode(uint8_t u8_Instance) const
{
  if(u8_Instance >= MAX_INSTANCES)
  {
    return EColorMode::RGB;
  }

  return m_Insance[u8_Instance].me_ColorMode;
}



void LightstripeSettings::setWaveMaxSpeed(uint8_t u8_Instance, float f32_Speed)
{
  bool b_Changed = false;

  if(u8_Instance >= MAX_INSTANCES)
  {
    return;
  }

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    
    if(m_Insance[u8_Instance].mf32_WaveMaxSpeed != f32_Speed)
    {
      m_Insance[u8_Instance].mf32_WaveMaxSpeed = f32_Speed;
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::ColorSettings));
  }
}
float LightstripeSettings::getWaveMaxSpeed(uint8_t u8_Instance) const
{
  if(u8_Instance >= MAX_INSTANCES)
  {
    return 0.0f;
  }

  return m_Insance[u8_Instance].mf32_WaveMaxSpeed;
}

void LightstripeSettings::setWaveInterval(uint8_t u8_Instance, float f32_Pixels)
{
  bool b_Changed = false;

  if(u8_Instance >= MAX_INSTANCES)
  {
    return;
  }

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    
    if(m_Insance[u8_Instance].mf32_WaveInterval != f32_Pixels)
    {
      m_Insance[u8_Instance].mf32_WaveInterval = f32_Pixels;
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::ColorSettings));
  }
}

float LightstripeSettings::getWaveInterval(uint8_t u8_Instance) const
{
  if(u8_Instance >= MAX_INSTANCES)
  {
    return 0.0f;
  }

  return m_Insance[u8_Instance].mf32_WaveInterval;
}

void LightstripeSettings::setMinRgbColor(uint8_t u8_Instance, uint32_t u32_Color)
{
  bool b_Changed = false;

  if(u8_Instance >= MAX_INSTANCES)
  {
    return;
  }

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    
    uint8_t u8_Red = (u32_Color >> 16) & 0xFF;
    uint8_t u8_Green = (u32_Color >> 8) & 0xFF;
    uint8_t u8_Blue = u32_Color & 0xFF;

    if((m_Insance[u8_Instance].mu8_RedMin != u8_Red) || (m_Insance[u8_Instance].mu8_GreenMin != u8_Green) || (m_Insance[u8_Instance].mu8_BlueMin != u8_Blue))
    {
      m_Insance[u8_Instance].mu8_RedMin = u8_Red;
      m_Insance[u8_Instance].mu8_GreenMin = u8_Green;
      m_Insance[u8_Instance].mu8_BlueMin = u8_Blue;
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::ColorSettings));
  }
}

uint32_t LightstripeSettings::getMinRgbColor(uint8_t u8_Instance)
{
  if(u8_Instance >= MAX_INSTANCES)
  {
    return 0;
  }

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);

    uint32_t u32_Color = (static_cast<uint32_t>(m_Insance[u8_Instance].mu8_RedMin) << 16) |
                         (static_cast<uint32_t>(m_Insance[u8_Instance].mu8_GreenMin) << 8) |
                         static_cast<uint32_t>(m_Insance[u8_Instance].mu8_BlueMin);

    return u32_Color;
  }
}


void LightstripeSettings::setMaxRgbColor(uint8_t u8_Instance, uint32_t u32_Color)
{
  bool b_Changed = false;

  if(u8_Instance >= MAX_INSTANCES)
  {
    return;
  }

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    
    uint8_t u8_Red = (u32_Color >> 16) & 0xFF;
    uint8_t u8_Green = (u32_Color >> 8) & 0xFF;
    uint8_t u8_Blue = u32_Color & 0xFF;

    if((m_Insance[u8_Instance].mu8_RedMax != u8_Red) || (m_Insance[u8_Instance].mu8_GreenMax != u8_Green) || (m_Insance[u8_Instance].mu8_BlueMax != u8_Blue))
    {
      m_Insance[u8_Instance].mu8_RedMax = u8_Red;
      m_Insance[u8_Instance].mu8_GreenMax = u8_Green;
      m_Insance[u8_Instance].mu8_BlueMax = u8_Blue;
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::ColorSettings));
  }
}


uint32_t LightstripeSettings::getMaxRgbColor(uint8_t u8_Instance)
{
  if(u8_Instance >= MAX_INSTANCES)
  {
    return 0;
  }

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);

    uint32_t u32_Color = (static_cast<uint32_t>(m_Insance[u8_Instance].mu8_RedMax) << 16) |
                         (static_cast<uint32_t>(m_Insance[u8_Instance].mu8_GreenMax) << 8) |
                         static_cast<uint32_t>(m_Insance[u8_Instance].mu8_BlueMax);

    return u32_Color;
  }
}

void LightstripeSettings::setMinHue(uint8_t u8_Instance, float f32_Hue)
{
  bool b_Changed = false;

  if(u8_Instance >= MAX_INSTANCES)
  {
    return;
  }

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);

    if(m_Insance[u8_Instance].mf32_HueMin != f32_Hue)
    {
      m_Insance[u8_Instance].mf32_HueMin = f32_Hue;
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::ColorSettings));
  }
}

float LightstripeSettings::getMinHue(uint8_t u8_Instance) const
{
  if(u8_Instance >= MAX_INSTANCES)
  {
    return 0;
  }

  return m_Insance[u8_Instance].mf32_HueMin;
}

void LightstripeSettings::setMaxHue(uint8_t u8_Instance, float f32_Hue)
{
  bool b_Changed = false;

  if(u8_Instance >= MAX_INSTANCES)
  {
    return;
  }

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);

    if(m_Insance[u8_Instance].mf32_HueMax != f32_Hue)
    {
      m_Insance[u8_Instance].mf32_HueMax = f32_Hue;
      b_Changed = true;
    }
  }

  if(b_Changed)
  {
    notifyObservers(static_cast<EDataField>(EField::ColorSettings));
  }
}

float LightstripeSettings::getMaxHue(uint8_t u8_Instance) const
{
  if(u8_Instance >= MAX_INSTANCES)
  {
    return 0;
  }

  return m_Insance[u8_Instance].mf32_HueMax;
}





uint16_t LightstripeSettings::serialize(uint8_t *pu8_Buffer, const uint16_t u16_BufferSize)
{
  uint16_t u16_BufferPos = 0;

  if((!pu8_Buffer) || (u16_BufferSize<LIGHTSTRIPE_SETTINGS_SIZE))
    return 0;

  std::lock_guard<std::mutex> lock(m_DataMutex);

  for(uint8_t i = 0; i < MAX_INSTANCES; ++i)
  {
    pu8_Buffer[u16_BufferPos++] = m_Insance[i].mb_EnableState ? 1 : 0;

    pu8_Buffer[u16_BufferPos++] = static_cast<uint8_t>(m_Insance[i].mu16_StartPixelIndex & 0xFF);
    pu8_Buffer[u16_BufferPos++] = static_cast<uint8_t>((m_Insance[i].mu16_StartPixelIndex >> 8) & 0xFF);
    pu8_Buffer[u16_BufferPos++] = static_cast<uint8_t>(m_Insance[i].mu16_NumPixels & 0xFF);
    pu8_Buffer[u16_BufferPos++] = static_cast<uint8_t>((m_Insance[i].mu16_NumPixels >> 8) & 0xFF);

    pu8_Buffer[u16_BufferPos++] = static_cast<uint8_t>(m_Insance[i].me_ValueTarget);
    pu8_Buffer[u16_BufferPos++] = static_cast<uint8_t>(m_Insance[i].me_WaveForm);
    pu8_Buffer[u16_BufferPos++] = static_cast<uint8_t>(m_Insance[i].me_ColorMode);

    uint8_t* pu8_WaveInterval = reinterpret_cast<uint8_t*>(&m_Insance[i].mf32_WaveInterval);
    uint8_t* pu8_WaveMaxSpeed = reinterpret_cast<uint8_t*>(&m_Insance[i].mf32_WaveMaxSpeed);
    uint8_t* pu8_HueMin = reinterpret_cast<uint8_t*>(&m_Insance[i].mf32_HueMin);
    uint8_t* pu8_HueMax = reinterpret_cast<uint8_t*>(&m_Insance[i].mf32_HueMax);

    for (int j = 0; j < 4; ++j) 
      pu8_Buffer[u16_BufferPos++] = pu8_WaveInterval[j];
    for (int j = 0; j < 4; ++j)
      pu8_Buffer[u16_BufferPos++] = pu8_WaveMaxSpeed[j];
    for (int j = 0; j < 4; ++j)
      pu8_Buffer[u16_BufferPos++] = pu8_HueMin[j];
    for (int j = 0; j < 4; ++j)
      pu8_Buffer[u16_BufferPos++] = pu8_HueMax[j];

    pu8_Buffer[u16_BufferPos++] = m_Insance[i].mu8_RedMin;
    pu8_Buffer[u16_BufferPos++] = m_Insance[i].mu8_GreenMin;
    pu8_Buffer[u16_BufferPos++] = m_Insance[i].mu8_BlueMin;
    pu8_Buffer[u16_BufferPos++] = m_Insance[i].mu8_RedMax;
    pu8_Buffer[u16_BufferPos++] = m_Insance[i].mu8_GreenMax;
    pu8_Buffer[u16_BufferPos++] = m_Insance[i].mu8_BlueMax;
  }

  return u16_BufferPos;
}



void LightstripeSettings::unserialize(const uint8_t *pu8_Buffer, const uint16_t u16_Size)
{
  uint16_t u16_BufferPos = 0;

  std::lock_guard<std::mutex> lock(m_DataMutex);

  if((!pu8_Buffer) || (u16_Size<LIGHTSTRIPE_SETTINGS_SIZE))
    return;

  for(uint8_t i = 0; i < MAX_INSTANCES; ++i)
  {
    m_Insance[i].mb_EnableState = pu8_Buffer[u16_BufferPos++] ? true : false;

    m_Insance[i].mu16_StartPixelIndex = static_cast<uint16_t>(pu8_Buffer[u16_BufferPos++]);
    m_Insance[i].mu16_StartPixelIndex |= static_cast<uint16_t>(pu8_Buffer[u16_BufferPos++]) << 8;
    m_Insance[i].mu16_NumPixels = static_cast<uint16_t>(pu8_Buffer[u16_BufferPos++]);
    m_Insance[i].mu16_NumPixels |= static_cast<uint16_t>(pu8_Buffer[u16_BufferPos++]) << 8;

    m_Insance[i].me_ValueTarget = static_cast<EValueTarget>(pu8_Buffer[u16_BufferPos++]);
    m_Insance[i].me_WaveForm = static_cast<EWaveForm>(pu8_Buffer[u16_BufferPos++]);
    m_Insance[i].me_ColorMode = static_cast<EColorMode>(pu8_Buffer[u16_BufferPos++]);

    uint8_t* pu8_WaveInterval = reinterpret_cast<uint8_t*>(&m_Insance[i].mf32_WaveInterval);
    for (int j = 0; j < 4; ++j) 
      pu8_WaveInterval[j] = pu8_Buffer[u16_BufferPos++];

    uint8_t* pu8_WaveMaxSpeed = reinterpret_cast<uint8_t*>(&m_Insance[i].mf32_WaveMaxSpeed);
    for (int j = 0; j < 4; ++j)
      pu8_WaveMaxSpeed[j] = pu8_Buffer[u16_BufferPos++];
  
    uint8_t* pu8_HueMin = reinterpret_cast<uint8_t*>(&m_Insance[i].mf32_HueMin);
    for (int j = 0; j < 4; ++j)
      pu8_HueMin[j] = pu8_Buffer[u16_BufferPos++];
  
    uint8_t* pu8_HueMax = reinterpret_cast<uint8_t*>(&m_Insance[i].mf32_HueMax);
    for (int j = 0; j < 4; ++j)
      pu8_HueMax[j] = pu8_Buffer[u16_BufferPos++];

    m_Insance[i].mu8_RedMin = pu8_Buffer[u16_BufferPos++];
    m_Insance[i].mu8_GreenMin = pu8_Buffer[u16_BufferPos++];
    m_Insance[i].mu8_BlueMin = pu8_Buffer[u16_BufferPos++];
    m_Insance[i].mu8_RedMax = pu8_Buffer[u16_BufferPos++];
    m_Insance[i].mu8_GreenMax = pu8_Buffer[u16_BufferPos++];
    m_Insance[i].mu8_BlueMax = pu8_Buffer[u16_BufferPos++]; 
  }
}

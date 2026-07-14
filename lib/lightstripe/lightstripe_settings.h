#ifndef LIGHTSTRIPE_SETTINGS_H
#define LIGHTSTRIPE_SETTINGS_H

#include <inttypes.h>

#include "data.h"

class LightstripeSettings : public Data
{
public:
  static const uint8_t MAX_INSTANCES = 4;

  enum class EValueTarget
  {
    Brightness = 0,
    Speed
  };

  enum class EWaveForm
  {
    None = 0,
    Sine,
    Sawtooth
  };

  enum class EColorMode
  {
    RGB = 0,
    HSV
  };
  
  static const uint16_t LIGHTSTRIPE_SETTINGS_SIZE = MAX_INSTANCES * 30UL; 
  enum class EField : uint8_t 
  { 
    EnableState = 0,
    ColorSettings
  }; 

private:
  
  struct 
  {
    bool mb_EnableState; 

    uint16_t mu16_StartPixelIndex;
    uint16_t mu16_NumPixels;

    EValueTarget me_ValueTarget;
    EWaveForm me_WaveForm;
    EColorMode me_ColorMode;
    float mf32_WaveInterval;
    float mf32_WaveMaxSpeed;
    float mf32_HueMin;
    float mf32_HueMax;
    uint8_t mu8_RedMin;
    uint8_t mu8_GreenMin;
    uint8_t mu8_BlueMin;
    uint8_t mu8_RedMax;
    uint8_t mu8_GreenMax;
    uint8_t mu8_BlueMax;
  } m_Insance[MAX_INSTANCES];

public:
  LightstripeSettings(void);

  void init(void);
  void setEnable(uint8_t u8_Instance, bool b_Enable);
  bool getEnable(uint8_t u8_Instance) const;

  void setPixelRange(uint8_t u8_Instance, uint16_t u16_StartPixelIndex, uint16_t u16_NumPixels);
  uint16_t getStartPixelIndex(uint8_t u8_Instance) const;
  uint16_t getNumPixels(uint8_t u8_Instance) const;

  void setValueTarget(uint8_t u8_Instance, EValueTarget e_Target);
  EValueTarget getValueTarget(uint8_t u8_Instance) const;

  void setWaveForm(uint8_t u8_Instance, EWaveForm e_WaveForm);
  EWaveForm getWaveForm(uint8_t u8_Instance) const;

  void setColorMode(uint8_t u8_Instance, EColorMode e_ColorMode);
  EColorMode getColorMode(uint8_t u8_Instance) const;

  void setWaveMaxSpeed(uint8_t u8_Instance, float f32_Speed);
  float getWaveMaxSpeed(uint8_t u8_Instance) const;
  void setWaveInterval(uint8_t u8_Instance, float f32_Pixels);
  float getWaveInterval(uint8_t u8_Instance) const;

  void setMinRgbColor(uint8_t u8_Instance, uint32_t u32_Color);
  uint32_t getMinRgbColor(uint8_t u8_Instance);
  void setMaxRgbColor(uint8_t u8_Instance, uint32_t u32_Color);
  uint32_t getMaxRgbColor(uint8_t u8_Instance);

  void setMinHue(uint8_t u8_Instance, float f32_Hue);
  float getMinHue(uint8_t u8_Instance) const;
  void setMaxHue(uint8_t u8_Instance, float f32_Hue);
  float getMaxHue(uint8_t u8_Instance) const;

  uint16_t serialize(uint8_t *pu8_Buffer, const uint16_t u16_BufferSize);
  void unserialize(const uint8_t *pu8_Buffer, const uint16_t u16_Size);
};

#endif

#ifndef LIGHTSTRIPE_H
#define LIGHTSTRIPE_H

#include <stdint.h>

#include "lightstripe_hal.h"


class Lightstripe
{
public:
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

private:
  LightstripeHal m_LightstripeHal; // Instance of the Lightstripe HAL
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

  float mf32_Value;
  float mf32_CurrentPhase;

public:
  Lightstripe(uint8_t u8_Pin, uint16_t u16_NumPixels);
  ~Lightstripe() = default;

  void setup(void);
  void loop(uint32_t u32_CurrentTime); 

  void enable(void);
  void disable(void);

  void setValueTarget(EValueTarget e_Target);
  void setValue(float f_Value);

  void setWaveForm(EWaveForm e_WaveForm);
  void setWaveInterval(float f32_Pixels);
  void setWaveMaxSpeed(float f32_Speed);

  void setColorMode(EColorMode e_ColorMode);
  void setMinRgbColor(uint32_t u32_Color);
  void setMaxRgbColor(uint32_t u32_Color);
  void setMinHue(float f32_Hue);
  void setMaxHue(float f32_Hue);

private:
  uint32_t convertHueToRgb(float f32_Hue, float f32_Saturation = 1.0f, float f32_Value = 1.0f);  
  uint8_t colorToLedValue(float f32_BrightnessInFraction);
};


#endif // LIGHTSTRIPE_H
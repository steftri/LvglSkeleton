#include <math.h>

#include "lightstripe.h"



Lightstripe::Lightstripe(uint8_t u8_Pin, uint16_t u16_NumPixels)
  : m_LightstripeHal(u8_Pin, u16_NumPixels)
  , mu16_NumPixels(u16_NumPixels)
  , me_ValueTarget(EValueTarget::Brightness)
  , me_WaveForm(EWaveForm::None)
  , me_ColorMode(EColorMode::RGB)
  , mf32_HueMin(0.0f)
  , mf32_HueMax(360.0f)
  , mu8_RedMin(0)
  , mu8_GreenMin(0)
  , mu8_BlueMin(0)
  , mu8_RedMax(255)
  , mu8_GreenMax(255)
  , mu8_BlueMax(255)
  , mf32_Value(0.0f)
  , mf32_CurrentPhase(0.0f)
  , mu32_LastTime(0)
{
}



void Lightstripe::setup(void)
{
  m_LightstripeHal.setup();
}





void Lightstripe::enable(void)
{
  m_LightstripeHal.enable();
}


void Lightstripe::disable(void)
{
  m_LightstripeHal.disable();
}




void Lightstripe::setValueTarget(EValueTarget e_Target)
{
  me_ValueTarget = e_Target;
}


void Lightstripe::setValue(float f_Value)
{
  mf32_Value = f_Value;
}


void Lightstripe::setWaveForm(EWaveForm e_WaveForm)
{
  me_WaveForm = e_WaveForm;
}


void Lightstripe::setWaveInterval(float f32_Pixels)
{
  if(f32_Pixels < 2.0f)
  {
    f32_Pixels = 2.0f;
  }
  mf32_WaveInterval = f32_Pixels;
}


void Lightstripe::setWaveMaxSpeed(float f32_Speed)
{
  mf32_WaveMaxSpeed = f32_Speed;
}


void Lightstripe::setColorMode(EColorMode e_ColorMode)
{
  me_ColorMode = e_ColorMode;
}


void Lightstripe::setMinRgbColor(uint32_t u32_Color)
{
  mu8_RedMin = (u32_Color >> 16) & 0xFF;
  mu8_GreenMin = (u32_Color >> 8) & 0xFF;
  mu8_BlueMin = u32_Color & 0xFF;
}


void Lightstripe::setMaxRgbColor(uint32_t u32_Color)
{
  mu8_RedMax = (u32_Color >> 16) & 0xFF;
  mu8_GreenMax = (u32_Color >> 8) & 0xFF;
  mu8_BlueMax = u32_Color & 0xFF;
}


void Lightstripe::setMinHue(float f32_Hue)
{
  if(f32_Hue < 0.0f)
  {
    f32_Hue = 0.0f;
  }
  else if(f32_Hue > 360.0f)
  {
    f32_Hue = 360.0f;
  }
  mf32_HueMin = f32_Hue;
}

void Lightstripe::setMaxHue(float f32_Hue)
{
  if(f32_Hue < 0.0f)
  {
    f32_Hue = 0.0f;
  }
  else if(f32_Hue > 360.0f)
  {
    f32_Hue = 360.0f;
  }
  mf32_HueMax = f32_Hue;
}



void Lightstripe::loop(uint32_t u32_CurrentTime)
{
  float f32_WaveformValue = 1.0f;
  float f32_WaveSpeed;
  float f32_Brightness;

  // first, in case the value target is speed, we need to adjust the wave speed
  if(me_ValueTarget == EValueTarget::Speed)
  {
    f32_WaveSpeed = mf32_WaveMaxSpeed * mf32_Value;
    f32_Brightness = 1.0f; 
  }
  else
  {
    f32_WaveSpeed = mf32_WaveMaxSpeed;
    f32_Brightness = mf32_Value;
  }

  // Accumulate phase based on delta time — avoids jumps when speed changes.
  // Skip accumulation when speed is near zero (wave stands still) to avoid division by zero.
  uint32_t u32_DeltaTime = u32_CurrentTime - mu32_LastTime;
  mu32_LastTime = u32_CurrentTime;
  mf32_CurrentPhase += f32_WaveSpeed * static_cast<float>(u32_DeltaTime) / 1000.0f;
  mf32_CurrentPhase -= floorf(mf32_CurrentPhase); // Normalize to [0,1) — works for any speed magnitude and direction

  for(uint16_t u16_PixelIndex = 0; u16_PixelIndex < mu16_NumPixels; u16_PixelIndex++)
  {
    // Apply wave form if any
    if(me_WaveForm != EWaveForm::None)
    {
      float f32_Phase = (static_cast<float>(u16_PixelIndex) / mf32_WaveInterval) + mf32_CurrentPhase;
      switch(me_WaveForm)
      {
        case EWaveForm::Sine:
          f32_WaveformValue = (sinf(f32_Phase * 2.0f * 3.14159265f) + 1.0f) / 2.0f; // Normalize sine wave to [0,1]
          break;
        case EWaveForm::Sawtooth:
          f32_WaveformValue = fmod(f32_Phase, 1.0f); // Sawtooth wave
          break;
        default:
          break;
      }
    }

    // second step: apply color mode
    uint32_t u32_Color = 0;
    if(me_ColorMode == EColorMode::RGB)
    {
      // Interpolate between min and max RGB colors based on the value
      float f32_PixelBrightness = f32_Brightness * f32_WaveformValue;

      uint8_t u8_Red = colorToLedValue((mu8_RedMin + (mu8_RedMax - mu8_RedMin) * f32_PixelBrightness) / 255.0f);
      uint8_t u8_Green = colorToLedValue((mu8_GreenMin + (mu8_GreenMax - mu8_GreenMin) * f32_PixelBrightness) / 255.0f);
      uint8_t u8_Blue = colorToLedValue((mu8_BlueMin + (mu8_BlueMax - mu8_BlueMin) * f32_PixelBrightness) / 255.0f);
      u32_Color = (u8_Red << 16) | (u8_Green << 8) | u8_Blue;
    }
    else 
    {
      float f32_Hue;

      if( me_WaveForm == EWaveForm::None )
      {
        // Interpolate between min and max hue based on the value
        f32_Hue = mf32_HueMin + (mf32_HueMax - mf32_HueMin) * mf32_Value;
      }
      else
      {
        // Interpolate between min and max hue based on the waveform value
        f32_Hue = mf32_HueMin + (mf32_HueMax - mf32_HueMin) * f32_WaveformValue;
      }
      u32_Color = convertHueToRgb(f32_Hue, 1.0f, f32_Brightness);
    }
  
    m_LightstripeHal.setPixelColor(u16_PixelIndex, u32_Color);
  }
  m_LightstripeHal.show(); // Call the HAL loop to update the light stripe hardware
}



uint32_t Lightstripe::convertHueToRgb(float f32_Hue, float f32_Saturation, float f32_Value)
{
  float f32_C = f32_Value * f32_Saturation; // Chroma
  float f32_X = f32_C * (1.0f - fabs(fmod(f32_Hue / 60.0f, 2) - 1.0f));
  float f32_M = f32_Value - f32_C;
  float f32_R = 0.0f;
  float f32_G = 0.0f;
  float f32_B = 0.0f;

  if(f32_Hue < 60.0f) { f32_R = f32_C; f32_G = f32_X; f32_B = 0.0f; }
  else if(f32_Hue < 120.0f) { f32_R = f32_X; f32_G = f32_C; f32_B = 0.0f; }
  else if(f32_Hue < 180.0f) { f32_R = 0.0f; f32_G = f32_C; f32_B = f32_X; }
  else if(f32_Hue < 240.0f) { f32_R = 0.0f; f32_G = f32_X; f32_B = f32_C; }
  else if(f32_Hue < 300.0f) { f32_R = f32_X; f32_G = 0.0f; f32_B = f32_C; }
  else { f32_R = f32_C; f32_G = 0.0f; f32_B = f32_X; }

  uint8_t u8_Red = colorToLedValue(f32_R + f32_M);
  uint8_t u8_Green = colorToLedValue(f32_G + f32_M);
  uint8_t u8_Blue = colorToLedValue(f32_B + f32_M);
  
  return (u8_Red << 16) | (u8_Green << 8) | u8_Blue;
}


uint8_t Lightstripe::colorToLedValue(float f32_BrightnessInFraction)
{
  // Apply a simple gamma correction for perceived brightness
  float f32_Gamma = 2.2f; // Typical gamma value for LEDs
  float f32_Corrected = pow(f32_BrightnessInFraction, f32_Gamma);
  return static_cast<uint8_t>(f32_Corrected * 255.0f);
} 

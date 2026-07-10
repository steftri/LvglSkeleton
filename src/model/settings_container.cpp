#include "crc32.h"

#include "settings_container.h"



const uint32_t EEPROM_SETTINGS_MAGIC   = 0x1acffc1d;
const uint16_t EEPROM_SETTINGS_VERSION = 1;


SettingsContainer::SettingsContainer(void)  
  : mb_Valid(false)
{
}





SettingsContainer::ERc SettingsContainer::save(void)
{ 
  uint8_t au8_VersionBuffer[2];

  uint8_t au8_SystemSettingBuffer[SystemSettings::SYSTEM_SETTINGS_SIZE];
  uint16_t u16_SystemSettingsSize = 0;

  uint8_t au8_WifiSettingBuffer[WifiSettings::WIFI_SETTINGS_SIZE];
  uint16_t u16_WifiSettingSize = 0; 

  uint8_t au8_MqttSettingBuffer[MqttSettings::MQTT_SETTINGS_SIZE];
  uint16_t u16_MqttSettingSize = 0; 
  
  CRC32 crc;
  uint32_t u32_crc;
  
  m_DataStorage.open(DataStorage::EMode::Write);
  m_DataStorage.write(static_cast<uint8_t>(EEPROM_SETTINGS_MAGIC>>24));
  m_DataStorage.write(static_cast<uint8_t>(EEPROM_SETTINGS_MAGIC>>16));
  m_DataStorage.write(static_cast<uint8_t>(EEPROM_SETTINGS_MAGIC>>8));
  m_DataStorage.write(static_cast<uint8_t>(EEPROM_SETTINGS_MAGIC));

  au8_VersionBuffer[0] = static_cast<uint8_t>(EEPROM_SETTINGS_VERSION>>8);
  au8_VersionBuffer[1] = static_cast<uint8_t>(EEPROM_SETTINGS_VERSION);
  m_DataStorage.write(au8_VersionBuffer, 2);  
  crc.add(au8_VersionBuffer, 2);

  // serialize and store System settings
  u16_SystemSettingsSize = m_SystemSettings.serialize(au8_SystemSettingBuffer, sizeof(au8_SystemSettingBuffer));
  m_DataStorage.write(au8_SystemSettingBuffer, u16_SystemSettingsSize);
  crc.add(au8_SystemSettingBuffer, u16_SystemSettingsSize);
  
  // serialize and store WIFI settings 
  u16_WifiSettingSize = m_WifiSettings.serialize(au8_WifiSettingBuffer, sizeof(au8_WifiSettingBuffer));
  m_DataStorage.write(au8_WifiSettingBuffer, u16_WifiSettingSize);  
  crc.add(au8_WifiSettingBuffer, u16_WifiSettingSize);

  // serialize and store MQTT settings 
  u16_MqttSettingSize = m_MqttSettings.serialize(au8_MqttSettingBuffer, sizeof(au8_MqttSettingBuffer));
  m_DataStorage.write(au8_MqttSettingBuffer, u16_MqttSettingSize);  
  crc.add(au8_MqttSettingBuffer, u16_MqttSettingSize);

  // TODO: serialize and store Worker settings

  u32_crc = crc.getResult();
  m_DataStorage.write(static_cast<uint8_t>(u32_crc>>24));
  m_DataStorage.write(static_cast<uint8_t>(u32_crc>>16));
  m_DataStorage.write(static_cast<uint8_t>(u32_crc>>8));
  m_DataStorage.write(static_cast<uint8_t>(u32_crc));  
  
  m_DataStorage.close();

  mb_Valid = true;
  return ERc::Ok;     
}



SettingsContainer::ERc SettingsContainer::load(void)
{
  uint32_t u32_Magic;
  uint8_t au8_VersionBuffer[2];
  uint16_t u16_Version;

  uint8_t au8_SystemSettingsBuffer[SystemSettings::SYSTEM_SETTINGS_SIZE];
  
  uint8_t au8_WifiSettingBuffer[WifiSettings::WIFI_SETTINGS_SIZE];

  uint8_t au8_MqttSettingBuffer[MqttSettings::MQTT_SETTINGS_SIZE];

  CRC32 crc;
  uint32_t u32_crc;

  mb_Valid = false;

  m_DataStorage.open(DataStorage::EMode::Read);

  u32_Magic  = static_cast<uint32_t>(m_DataStorage.read())<<24;
  u32_Magic |= static_cast<uint32_t>(m_DataStorage.read())<<16;
  u32_Magic |= static_cast<uint32_t>(m_DataStorage.read())<<8;
  u32_Magic |= static_cast<uint32_t>(m_DataStorage.read()); 

  if(u32_Magic != EEPROM_SETTINGS_MAGIC)
  {
    m_DataStorage.close();
    return ERc::NotFoundError;     
  }

  for(uint16_t i=0; i<2; i++)
    au8_VersionBuffer[i] = m_DataStorage.read();
  crc.add(au8_VersionBuffer, 2);    

  u16_Version = (static_cast<uint16_t>(au8_VersionBuffer[0])<<8) | static_cast<uint16_t>(au8_VersionBuffer[1]);
  if(u16_Version!=EEPROM_SETTINGS_VERSION)
  {
    m_DataStorage.close();
    return ERc::VersionMismatchError;
  }

  // load System settings to flat buffer
  m_DataStorage.read(au8_SystemSettingsBuffer, SystemSettings::SYSTEM_SETTINGS_SIZE);
  crc.add(au8_SystemSettingsBuffer, SystemSettings::SYSTEM_SETTINGS_SIZE);

  // load WIFI settings to flat buffer
  m_DataStorage.read(au8_WifiSettingBuffer, WifiSettings::WIFI_SETTINGS_SIZE);
  crc.add(au8_WifiSettingBuffer, WifiSettings::WIFI_SETTINGS_SIZE);  

  // load MQTT settings to flat buffer
  m_DataStorage.read(au8_MqttSettingBuffer, MqttSettings::MQTT_SETTINGS_SIZE);
  crc.add(au8_MqttSettingBuffer, MqttSettings::MQTT_SETTINGS_SIZE);  

  // TODO: load Worker settings to flat buffer

  u32_crc  = static_cast<uint32_t>(m_DataStorage.read())<<24;
  u32_crc |= static_cast<uint32_t>(m_DataStorage.read())<<16;
  u32_crc |= static_cast<uint32_t>(m_DataStorage.read())<<8;
  u32_crc |= static_cast<uint32_t>(m_DataStorage.read()); 

  m_DataStorage.close();

  if(u32_crc != crc.getResult())
  {
    return ERc::VerificationError;
  }

  // unserialize System settings
  m_SystemSettings.unserialize(au8_SystemSettingsBuffer, sizeof(au8_SystemSettingsBuffer));

  // unserialize WIFI settings
  m_WifiSettings.unserialize(au8_WifiSettingBuffer, sizeof(au8_WifiSettingBuffer));

  // unserialize MQTT settings
  m_MqttSettings.unserialize(au8_MqttSettingBuffer, sizeof(au8_MqttSettingBuffer));

  // TODO: unserialize Worker settings

  mb_Valid = true;
  return ERc::Ok;
}



void SettingsContainer::clear(void)
{
  m_SystemSettings.init();
  m_WifiSettings.init();
  m_MqttSettings.init();
  m_WorkerSettings.init();
  save();
}


bool SettingsContainer::isValid(void)
{
  return mb_Valid;
}


SystemSettings &SettingsContainer::getSystemSettings(void)
{
  return m_SystemSettings;
}


WifiSettings &SettingsContainer::getWifiSettings(void)
{
  return m_WifiSettings;
}


MqttSettings &SettingsContainer::getMqttSettings(void)
{
  return m_MqttSettings;
}


WorkerSettings &SettingsContainer::getWorkerSettings(void)
{
  return m_WorkerSettings;
}

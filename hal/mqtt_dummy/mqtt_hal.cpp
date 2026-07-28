#include "mqtt_hal.h"


MqttHal::MqttHal(MqttActionInterface &actionListener)
 : mr_ActionListener(actionListener)
{
}


void MqttHal::setup()
{
}


MqttHal::ERc MqttHal::connect(const char *pc_BrokerAddress, uint16_t u16_BrokerPort)
{
  mr_ActionListener.onConnected();
  return MqttHal::ERc::Ok; // Dummy implementation, always return Ok
} 


void MqttHal::disconnect()
{
  mr_ActionListener.onDisconnected();
}



void MqttHal::setLastWill(const char *pc_Topic, const uint8_t *pu8_MessageBuffer, const size_t MessageSize, uint8_t u8_QoS, bool b_Retain)
{
}



void MqttHal::publish(const char *pc_Topic, const uint8_t *pu8_MessageBuffer, const size_t MessageSize, uint8_t u8_QoS, bool b_Retain)
{
}



void MqttHal::subscribe(const char *pc_Topic)
{
}


void MqttHal::poll()
{ 
}


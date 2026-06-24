#include "mqtt_data.h"
#include <cstring>


MqttData::MqttData()
  : me_State(EState::Disconnected)
  , mu32_SentMessageCount(0)
  , mu32_ReceivedMessageCount(0)
{
}



void MqttData::incrementSentMessageCount()
{
  if(mu32_SentMessageCount < UINT32_MAX) // Prevent overflow
    mu32_SentMessageCount++;
}



void MqttData::incrementReceivedMessageCount()
{
  if(mu32_ReceivedMessageCount < UINT32_MAX) // Prevent overflow
    mu32_ReceivedMessageCount++;
}



uint32_t MqttData::getSentMessageCount() const
{
  return mu32_SentMessageCount;
}



uint32_t MqttData::getReceivedMessageCount() const
{
  return mu32_ReceivedMessageCount;
}



void MqttData::setState(EState e_State)
{
  std::lock_guard<std::mutex> lock(m_DataMutex);

  if(me_State != e_State)
  {
    me_State = e_State;
    notifyObservers(static_cast<EDataField>(EField::ConnectionState));
  }
}



MqttData::EState MqttData::getState(void) const
{
  return me_State;
}


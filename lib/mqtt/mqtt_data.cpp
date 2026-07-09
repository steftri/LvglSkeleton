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
  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    mu32_SentMessageCount++;
  }
  notifyObservers(static_cast<EDataField>(EField::MessageCount));
}



void MqttData::incrementReceivedMessageCount()
{
  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    mu32_ReceivedMessageCount++;
  }
  notifyObservers(static_cast<EDataField>(EField::MessageCount));
}



uint32_t MqttData::getSentMessageCount() const
{
  return mu32_SentMessageCount;
}



uint32_t MqttData::getReceivedMessageCount() const
{
  return mu32_ReceivedMessageCount;
}



void MqttData::setState(EState e_State, int32_t s32_ErrorCode, const char *pc_ErrorMessage)
{
  bool b_StateChanged = false;
  bool b_ErrorChanged = false;

  {
    std::lock_guard<std::mutex> lock(m_DataMutex);
    if(me_State != e_State)
    {
      me_State = e_State;
      b_StateChanged = true;
    }
    if(e_State == EState::Error)
    {
      ms32_LastErrorCode = s32_ErrorCode;
      if(pc_ErrorMessage != nullptr)
      {
        strncpy(mac_LastErrorMessage, pc_ErrorMessage, sizeof(mac_LastErrorMessage) - 1);
        mac_LastErrorMessage[sizeof(mac_LastErrorMessage) - 1] = '\0'; // Ensure null-termination
      }
      else
      {
        mac_LastErrorMessage[0] = '\0'; // Clear the last error message if none provided
      }
      b_ErrorChanged = true;
    }
  }

  if(b_StateChanged)
  {
    notifyObservers(static_cast<EDataField>(EField::ConnectionState));
    if(b_ErrorChanged)
    {
      notifyObservers(static_cast<EDataField>(EField::LastError));
    }
  }
}



MqttData::EState MqttData::getState(void) const
{
  return me_State;
}

int32_t MqttData::getLastErrorCode(void) const
{
  return ms32_LastErrorCode;
}

const char *MqttData::getLastErrorMessage(void) const
{
  return mac_LastErrorMessage;
}


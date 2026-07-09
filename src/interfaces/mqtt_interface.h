#ifndef MQTT_INTERFACE_H
#define MQTT_INTERFACE_H

class Mqtt
{
public:
  Mqtt() = delete;

  enum class EMessageType
  {
    NBIRTH,
    NDEATH,
    NDATA,
    NCMD,
    DBIRTH,
    DDEATH,
    DDATA,
    DCMD
  };

  static const char *MessageTypeToString(EMessageType e_MessageType)
  {
    switch(e_MessageType)
    {
      case EMessageType::NBIRTH: return "NBIRTH";
      case EMessageType::NDEATH: return "NDEATH";
      case EMessageType::NDATA: return "NDATA";
      case EMessageType::NCMD: return "NCMD";
      case EMessageType::DBIRTH: return "DBIRTH";
      case EMessageType::DDEATH: return "DDEATH";
      case EMessageType::DDATA: return "DDATA";
      case EMessageType::DCMD: return "DCMD";
      default: return "";
    }
  }
};

#endif // MQTT_INTERFACE_H
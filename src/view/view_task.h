#ifndef VIEW_TASK_H
#define VIEW_TASK_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "ui.h"

#include "model/data_container.h"
#include "model/settings_container.h"

#include "data_observer.h"


static const size_t VIEW_TASK_STACK_SIZE = 9*1024; // Stack size for the task
static const size_t VIEW_TASK_MESSAGE_QUEUE_SIZE = 10; // Size of the message queue for inter-task communication

static constexpr uint32_t UITASK_NOTIFY_NETWORKS_UPDATED = (1UL << 0); // Notification bit for available networks update




class ViewTask : public DataObserverInterface
{
private:
  TaskHandle_t mp_TaskHandle;

  bool mb_BlinkState;

  static void task(void *pvParameters);  
  static ViewTask *mp_thisInstance; // Static instance pointer for task access

  typedef struct
  {
    const char *pc_Title;
    const char *pc_Text;
  } SMessage;

  SMessage ma_MessageQueueStorage[VIEW_TASK_MESSAGE_QUEUE_SIZE];
  static StaticQueue_t m_xStaticQueue;
  QueueHandle_t m_xQueueHandle;

  Ui m_ui;

public:
  ViewTask();

  void begin();

  void showMessageBox(const char *pc_Title, const char *pc_Message);

private:
  void setup();
  void loop();

  // DataObserver implementation
  void onDataChanged(Data &r_Data, EDataField e_Field) override;

  void onWIFIDataChanged(EDataField e_Field);
  void onMQTTDataChanged(EDataField e_Field);
  void onSurveillanceDataChanged(EDataField e_Field);
  void onChargingDataChanged(EDataField e_Field);

  // Thread-internal methods to update the UI based on notifications
  void onUpdateEnableState();
  void onUpdateWIFIConnectionState();
  void onUpdateSettingsNetworkList();
  void onUpdateSettingsIPAddress();
  void onUpdateInfoSurveillanceStats();
  void onUpdateInfoLVGLStats();
  void onUpdateInfoMQTTConnectionState();
  void onUpdateInfoMQTTStats();
  void onShowMessageBox(const char *pc_Title, const char *pc_Message);

  void updateStateIndicators();

};



#endif // VIEW_TASK_H
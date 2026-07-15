#ifndef WORKER_TASK_H
#define WORKER_TASK_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "data.h"

#include "lightstripe_settings.h"
#include "lightstripe_data.h"
#include "system_data.h"

#include "lightstripe.h"


static const size_t WORKER_TASK_STACK_SIZE = 4096; // Stack size for the task


class WorkerTask : public DataObserverInterface
{
private:
  TaskHandle_t mp_TaskHandle;

  static void task(void *pvParameters);  
  static WorkerTask *mp_thisInstance; // Static instance pointer for task access

  LightstripeSettings &m_Settings;
  LightstripeData &m_Data;
  SystemData &m_SystemData;

  Lightstripe m_Lightstripe; // Instance of the Lightstripe HAL

  // Simulation state
  float    mf32_SimSoc;            ///< Simulated state of charge (0–100 %)
  float    mf32_SimCurrentA;       ///< Simulated charging current in A
  uint16_t mu16_SimDurationMin;    ///< Simulated charging duration in min
  float    mf32_SimPowerKWh;       ///< Simulated power consumption in kWh
  float    mf32_SimChargingSpeedKW; ///< Simulated charging speed in kW

public:
  WorkerTask(LightstripeSettings &settings, LightstripeData &data, SystemData &systemData);
  void begin(void);

  // DataObserver implementation
  void onDataChanged(Data &r_Data, EDataField e_Field) override;
  void onSettingsChanged(EDataField e_Field);
  void onDataChanged(EDataField e_Field);

private:  
  void setup();
  void loop();
  void updateSimulation(uint32_t u32_CurrentTimeMs);
};


#endif // WORKER_TASK_H
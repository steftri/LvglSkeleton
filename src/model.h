#ifndef MODEL_H
#define MODEL_H

#include <stddef.h>

#include "model/settings_container.h"
#include "model/data_container.h"

#include "model/model_task.h"


class Model
{
  SettingsContainer m_Settings; // Holds the persistant settings
  DataContainer m_Data; // Holds the runtime data, e.g., Wi-Fi data

  ModelTask m_Task; // Task for handling model operations

public:
  // Constructor
  Model(void);

  // Destructor
  ~Model() = default;

  // Initialize the model
  void setup(void);

  // Start the model's threads (if any)
  void begin(void);

  // Update the model state
  void loop(void);

  SettingsContainer &getSettings(void);
  DataContainer &getData(void);

  ModelTask &getTask(void);

  void getWifiPassword(char *pc_Password, size_t passwordBufferSize, const char *pc_SSID);
};

#endif // MODEL_H
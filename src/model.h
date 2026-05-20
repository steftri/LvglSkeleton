#ifndef MODEL_H
#define MODEL_H

#include "model/settings.h"
#include "model/data.h"


class Model
{
  Settings m_Settings; // Holds the persistant settings
  Data m_Data; // Holds the runtime data, e.g., Wi-Fi data

public:
  // Constructor
  Model(void) = default;

  // Destructor
  ~Model() = default;

  // Initialize the model
  void setup(void);

  // Start the model's threads (if any)
  void begin(void);

  // Update the model state
  void loop(void);

  Settings *getSettings(void);
  Data *getData(void);
};

#endif // MODEL_H
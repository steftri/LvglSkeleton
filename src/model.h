#ifndef MODEL_H
#define MODEL_H

#include "model/settings.h"


class Model
{
  Settings m_Settings; // Holds the persistant settings

public:
  // Constructor
  Model(void) = default;

  // Destructor
  ~Model() = default;

  // Initialize the model
  void setup(void);

  // Update the model state
  void loop(void);

  Settings *getSettings(void);
};

#endif // MODEL_H
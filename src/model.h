#ifndef MODEL_H
#define MODEL_H

class Model
{
public:
  // Constructor
  Model(void) = default;

  // Destructor
  ~Model() = default;

  // Initialize the model
  void setup(void);

  // Update the model state
  void loop(void);
};

#endif // MODEL_H
#ifndef DATA_OBSERVER_INTERFACE_H
#define DATA_OBSERVER_INTERFACE_H

#include <inttypes.h>

class Data;

/**
 * @brief Strongly-typed field identifier.
 *
 * Each concrete Data subclass should define its own enum class with
 * underlying type EDataField to identify individual fields, e.g.:
 * @code
 *   enum class Field : EDataField { SPEED = 0, RPM = 1 };
 *   notifyObservers(static_cast<EDataField>(Field::SPEED));
 * @endcode
 * Use EDataField::ALL_FIELDS to signal that all fields changed.
 */
enum class EDataField : uint8_t
{
  AllData = 0xFF
};


/**
 * @brief Interface for observers that want to be notified when a Data object changes.
 *
 * Inherit from this class and implement onDataChanged() to receive notifications.
 * Register with a Data-derived object via Data::registerObserver().
 */
class DataObserverInterface
{
public:

  virtual ~DataObserverInterface() = default;

  /**
   * @brief Called by the observed Data object whenever its contents change.
   * @param r_Data   Reference to the Data object that triggered the notification.
   * @param e_Field  Identifier of the changed field, or EDataField::AllData.
   */
  virtual void onDataChanged(Data &r_Data, EDataField e_Field) = 0;
};


#endif // DATA_OBSERVER_INTERFACE_H

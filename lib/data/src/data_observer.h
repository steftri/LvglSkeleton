#ifndef DATA_OBSERVER_INTERFACE_H
#define DATA_OBSERVER_INTERFACE_H


class Data;


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
   * @param r_Data Reference to the Data object that triggered the notification.
   */
  virtual void onDataChanged(Data &r_Data) = 0;
};


#endif // DATA_OBSERVER_INTERFACE_H

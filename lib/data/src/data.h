#ifndef DATA_H
#define DATA_H

#include <inttypes.h>
#include <vector>
#include <mutex>

#include "data_observer.h"


/**
 * @brief Thread-safe base class for data containers with observer support.
 *
 * Derive from this class to create concrete data containers. Call
 * notifyObservers() inside setter methods whenever the stored data changes.
 * Observers register/unregister at runtime and are called outside the internal
 * lock to prevent deadlocks.
 */
class Data
{
public:
  Data(void);
  virtual ~Data() = default;

  void init(void);

  /**
   * @brief Register an observer to be notified on data changes.
   * @param p_Observer Non-owning pointer to the observer. Must not be nullptr.
   */
  void registerObserver(DataObserverInterface *p_Observer);

  /**
   * @brief Unregister a previously registered observer.
   * @param p_Observer Pointer that was passed to registerObserver().
   */
  void unregisterObserver(DataObserverInterface *p_Observer);

protected:
  /**
   * @brief Notify all registered observers that data has changed.
   *
   * Thread-safe: the observer list is copied under the lock, then each
   * observer is called outside the lock so that observer code may itself
   * call registerObserver() / unregisterObserver() without deadlocking.
   */
  void notifyObservers(void);

private:
  std::vector<DataObserverInterface *> m_Observers;
  std::mutex                           m_ObserverMutex;
};


#endif // DATA_H

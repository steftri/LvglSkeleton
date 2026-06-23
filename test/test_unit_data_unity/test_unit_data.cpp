#include <unity.h>

#ifdef ARDUINO
#include <Arduino.h>   // needed for service delay
#endif

#include "data.h"


// ---------------------------------------------------------------------------
// Concrete Data subclass used in tests
// ---------------------------------------------------------------------------
class ConcreteData : public Data
{
public:
  enum class EField : uint8_t { Value = 0 };

  int getValue(void) const { return m_Value; }

  void setValue(int i_Value)
  {
    m_Value = i_Value;
    notifyObservers(static_cast<EDataField>(EField::Value));
  }

  void notifyObserversPublic(void) { notifyObservers(); }

private:
  int m_Value{0};
};


// ---------------------------------------------------------------------------
// Concrete Observer used in tests
// ---------------------------------------------------------------------------
class TestObserver : public DataObserverInterface
{
public:
  int        notifyCount{0};
  Data      *p_lastSender{nullptr};
  EDataField e_lastField{EDataField::AllData};

  void onDataChanged(Data &r_Data, EDataField e_Field) override
  {
    ++notifyCount;
    p_lastSender = &r_Data;
    e_lastField  = e_Field;
  }
};


// ---------------------------------------------------------------------------
// setUp / tearDown (required by Unity)
// ---------------------------------------------------------------------------
void setUp(void)
{
}

void tearDown(void)
{
}


// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

void no_notification_without_observer(void)
{
  ConcreteData data;
  data.setValue(42);
  // No assertion needed – just must not crash
  TEST_PASS();
}


void observer_is_notified_on_change(void)
{
  ConcreteData data;
  TestObserver obs;

  data.registerObserver(&obs);
  data.setValue(7);

  TEST_ASSERT_EQUAL_INT(1, obs.notifyCount);
}


void observer_receives_correct_sender(void)
{
  ConcreteData data;
  TestObserver obs;

  data.registerObserver(&obs);
  data.setValue(3);

  TEST_ASSERT_EQUAL_PTR(&data, obs.p_lastSender);
}


void multiple_observers_are_all_notified(void)
{
  ConcreteData data;
  TestObserver obs1;
  TestObserver obs2;

  data.registerObserver(&obs1);
  data.registerObserver(&obs2);
  data.setValue(99);

  TEST_ASSERT_EQUAL_INT(1, obs1.notifyCount);
  TEST_ASSERT_EQUAL_INT(1, obs2.notifyCount);
}


void multiple_changes_accumulate_notifications(void)
{
  ConcreteData data;
  TestObserver obs;

  data.registerObserver(&obs);
  data.setValue(1);
  data.setValue(2);
  data.setValue(3);

  TEST_ASSERT_EQUAL_INT(3, obs.notifyCount);
}


void unregistered_observer_is_no_longer_notified(void)
{
  ConcreteData data;
  TestObserver obs;

  data.registerObserver(&obs);
  data.setValue(1);
  data.unregisterObserver(&obs);
  data.setValue(2);

  TEST_ASSERT_EQUAL_INT(1, obs.notifyCount);
}


void observer_can_be_reregistered(void)
{
  ConcreteData data;
  TestObserver obs;

  data.registerObserver(&obs);
  data.setValue(1);
  data.unregisterObserver(&obs);
  data.registerObserver(&obs);
  data.setValue(2);

  TEST_ASSERT_EQUAL_INT(2, obs.notifyCount);
}


void data_value_is_correct_after_set(void)
{
  ConcreteData data;
  data.setValue(123);
  TEST_ASSERT_EQUAL_INT(123, data.getValue());
}


void observer_receives_correct_field(void)
{
  ConcreteData data;
  TestObserver obs;

  data.registerObserver(&obs);
  data.setValue(5);

  TEST_ASSERT_EQUAL_INT(
    static_cast<uint8_t>(ConcreteData::EField::Value),
    static_cast<uint8_t>(obs.e_lastField)
  );
}


void observer_receives_all_fields_when_no_field_given(void)
{
  ConcreteData data;
  TestObserver obs;

  data.registerObserver(&obs);
  data.notifyObserversPublic();

  TEST_ASSERT_EQUAL_INT(
    static_cast<uint8_t>(EDataField::ALL_FIELDS),
    static_cast<uint8_t>(obs.e_lastField)
  );
}


// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------
void setup()
{
#ifdef ARDUINO
  delay(2000); // service delay
#endif

  UNITY_BEGIN();  

  RUN_TEST(no_notification_without_observer);
  RUN_TEST(observer_is_notified_on_change);
  RUN_TEST(observer_receives_correct_sender);
  RUN_TEST(multiple_observers_are_all_notified);
  RUN_TEST(multiple_changes_accumulate_notifications);
  RUN_TEST(unregistered_observer_is_no_longer_notified);
  RUN_TEST(observer_can_be_reregistered);
  RUN_TEST(data_value_is_correct_after_set);
  RUN_TEST(observer_receives_correct_field);
  RUN_TEST(observer_receives_all_fields_when_no_field_given);

  UNITY_END();
}

void loop() {}

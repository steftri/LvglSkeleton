#include <gtest/gtest.h>

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
// Test fixture
// ---------------------------------------------------------------------------
class DataUnitTest : public ::testing::Test
{
protected:
  ConcreteData data;
  TestObserver obs1;
  TestObserver obs2;

  void SetUp() override {}
  void TearDown() override {}
};


// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

TEST_F(DataUnitTest, NoNotificationWithoutObserver)
{
  data.setValue(42);
  // No crash, nothing to assert – just exercise the code path
}

TEST_F(DataUnitTest, ObserverIsNotifiedOnChange)
{
  data.registerObserver(&obs1);
  data.setValue(7);
  EXPECT_EQ(obs1.notifyCount, 1);
}

TEST_F(DataUnitTest, ObserverReceivesCorrectSender)
{
  data.registerObserver(&obs1);
  data.setValue(3);
  EXPECT_EQ(obs1.p_lastSender, &data);
}

TEST_F(DataUnitTest, MultipleObserversAreAllNotified)
{
  data.registerObserver(&obs1);
  data.registerObserver(&obs2);
  data.setValue(99);
  EXPECT_EQ(obs1.notifyCount, 1);
  EXPECT_EQ(obs2.notifyCount, 1);
}

TEST_F(DataUnitTest, MultipleChangesAccumulateNotifications)
{
  data.registerObserver(&obs1);
  data.setValue(1);
  data.setValue(2);
  data.setValue(3);
  EXPECT_EQ(obs1.notifyCount, 3);
}

TEST_F(DataUnitTest, UnregisteredObserverIsNoLongerNotified)
{
  data.registerObserver(&obs1);
  data.setValue(1);
  data.unregisterObserver(&obs1);
  data.setValue(2);
  EXPECT_EQ(obs1.notifyCount, 1);
}

TEST_F(DataUnitTest, ObserverCanBeReRegistered)
{
  data.registerObserver(&obs1);
  data.setValue(1);
  data.unregisterObserver(&obs1);
  data.registerObserver(&obs1);
  data.setValue(2);
  EXPECT_EQ(obs1.notifyCount, 2);
}

TEST_F(DataUnitTest, DataValueIsCorrectAfterSet)
{
  data.setValue(123);
  EXPECT_EQ(data.getValue(), 123);
}

TEST_F(DataUnitTest, ObserverReceivesCorrectField)
{
  data.registerObserver(&obs1);
  data.setValue(5);
  EXPECT_EQ(obs1.e_lastField, static_cast<EDataField>(ConcreteData::EField::Value));
}

TEST_F(DataUnitTest, ObserverReceivesAllFieldsWhenNoFieldGiven)
{
  data.registerObserver(&obs1);
  data.notifyObserversPublic();
  EXPECT_EQ(obs1.e_lastField, EDataField::AllData);
}


int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

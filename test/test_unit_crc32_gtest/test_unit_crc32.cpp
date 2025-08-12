#include <gtest/gtest.h>

#include "crc32.h"



class Crc32UnitTest : public ::testing::Test 
{
protected:
  CRC32 myCrc32Library;
    
  void SetUp() override 
  {
    myCrc32Library.init();
  }

  void TearDown() override 
  {
  }
};



TEST_F(Crc32UnitTest, DefaultValueTest) 
{
  EXPECT_EQ(myCrc32Library.get(), 0u);
}



TEST_F(Crc32UnitTest, SetValueTest) 
{
  myCrc32Library.init();
  myCrc32Library.add(42);
  EXPECT_EQ(myCrc32Library.get(), 42u);
}



int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

#include <gtest/gtest.h>

#include "data.h"



class DataUnitTest : public ::testing::Test 
{
protected:    
  void SetUp() override 
  {
  }

  void TearDown() override 
  {
  }
};



TEST_F(DataUnitTest, DefaultValueTest) 
{
  Data myDataLibrary;
//  EXPECT_EQ(myDataLibrary.getResult(), 0u);
}




int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

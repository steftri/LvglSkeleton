#include <gtest/gtest.h>

#include "crc32.h"



class Crc32UnitTest : public ::testing::Test 
{
protected:    
  void SetUp() override 
  {
  }

  void TearDown() override 
  {
  }
};



TEST_F(Crc32UnitTest, DefaultValueTest) 
{
  CRC32 myCrc32Library;
  EXPECT_EQ(myCrc32Library.getResult(), 0u);
}



TEST_F(Crc32UnitTest, AddSingleValue42) 
{
  CRC32 myCrc32Library;
  myCrc32Library.add(42);
  EXPECT_EQ(myCrc32Library.getResult(), 0x09B9265B);
}

TEST_F(Crc32UnitTest, InitValue) 
{
  CRC32 myCrc32Library;
  myCrc32Library.add(42);
  myCrc32Library.init();
  EXPECT_EQ(myCrc32Library.getResult(), 0u);
}

TEST_F(Crc32UnitTest, AddBuffer) 
{
  CRC32 myCrc32Library;
  myCrc32Library.init();
  myCrc32Library.add(reinterpret_cast<const uint8_t*>("123456789"), 9);
  EXPECT_EQ(myCrc32Library.getResult(), 0xCBF43926);
}


int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

#include <unity.h>

#ifdef ARDUINO  
#include <Arduino.h>   // needed for service delay
#endif

#include "crc32.h"







void setUp(void) 
{
  // set stuff up here
}

void tearDown(void) 
{
  // clean stuff up here
}



void default_value_test(void) 
{
  // check default value
  CRC32 myCrc32Library;
  TEST_ASSERT_EQUAL_UINT32(0, myCrc32Library.getResult());
}


void add_single_value_test(void) 
{
  // check set value
  CRC32 myCrc32Library;
  myCrc32Library.init();
  myCrc32Library.add(42);
  TEST_ASSERT_EQUAL_UINT32(0x09B9265B, myCrc32Library.getResult());
}


void init_value_test(void) 
{
  // check init
  CRC32 myCrc32Library;
  myCrc32Library.add(42);
  myCrc32Library.init();
  TEST_ASSERT_EQUAL_UINT32(0, myCrc32Library.getResult());
}


void add_buffer_test(void) 
{
  // check set value
  CRC32 myCrc32Library;
  myCrc32Library.init();
  myCrc32Library.add(reinterpret_cast<const uint8_t*>("123456789"), 9);
  TEST_ASSERT_EQUAL_UINT32(0xCBF43926, myCrc32Library.getResult());
}


void setup()
{
#ifdef ARDUINO  
    delay(2000); // service delay
#endif    
    UNITY_BEGIN();

    RUN_TEST(default_value_test);
    RUN_TEST(add_single_value_test);
    RUN_TEST(init_value_test);
    RUN_TEST(add_buffer_test);

    UNITY_END(); // stop unit testing
}



void loop()
{
}


#ifndef ARDUINO
// only needed if unit test is running in an native environment
int main(int argc, char *argv[])
{
  setup();
  loop();  // only execute loop once. We want to finish the test.
  return 0;
}
#endif

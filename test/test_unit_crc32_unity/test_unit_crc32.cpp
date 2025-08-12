#include <unity.h>

#ifdef ARDUINO  
#include <Arduino.h>   // needed for service delay
#endif

#include "crc32.h"



CRC32 myCrc32Library;



void setUp(void) 
{
  // set stuff up here
  (void)myCrc32Library.init();
}

void tearDown(void) 
{
  // clean stuff up here
}



void default_value_test(void) 
{
  // check default value
  TEST_ASSERT_EQUAL_UINT32(0, myCrc32Library.get());
}


void set_value_test(void) 
{
  // check set value
  myCrc32Library.init();
  myCrc32Library.add(47);
  TEST_ASSERT_EQUAL_UINT32(47, myCrc32Library.get());
}




void setup()
{
#ifdef ARDUINO  
    delay(2000); // service delay
#endif    
    UNITY_BEGIN();

    RUN_TEST(default_value_test);
    RUN_TEST(set_value_test);

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

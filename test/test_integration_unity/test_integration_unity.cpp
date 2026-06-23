#include <unity.h>

#ifdef ARDUINO  
#include <Arduino.h>   // needed for service delay
#endif



void setUp(void) 
{
  // set stuff up here
}

void tearDown(void) 
{
  // clean stuff up here
}



void test_case_1(void) 
{
  TEST_PASS();
}



void setup()
{
#ifdef ARDUINO  
    delay(2000); // service delay
#endif    
    UNITY_BEGIN();

    RUN_TEST(test_case_1);

    UNITY_END(); // stop unit testing
}



void loop()
{
  // nothing to do here, since unit test is finished
}


#ifndef ARDUINO
// only needed if unit test is running in an native environment
int main(int argc, char *argv[])
{
  setup();
  loop();
  return 0;
}
#endif
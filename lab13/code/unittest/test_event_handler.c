#include "CppUTest/TestHarness.h"

#include "stdio.h"
#include "hal_gpio.h"
#include "event_handler.h"
#include "timer.h"

TEST_GROUP(event_handler)
{
    void setup()
    {
        printf("\n==================================================\n");
    }

    void teardown()
    {
        printf("\n==================================================\n");
    }
};


/* test_no_timeout_event:
   start the timer with a duration of 2, call eh_get_event() only once
   and then check that the event TIME_OUT is not generated */

/// STUDENTS: To be programmed
TEST(event_handler,test_no_timeout_event)
{
    event_t event;
    
    timer_init();
    timer_start((uint16_t)2);
    event = eh_get_event();
    
    printf("event = %d\n", event);
    CHECK_TEXT(event != TIME_OUT, "WRONG WRONG");
}
/// END: To be programmed


/* test_timeout_event:
   start the timer with a duration so that after once calling eh_get_event()
   the TIME_OUT event is generated and then check it */

/// STUDENTS: To be programmed
TEST(event_handler,test_timeout_event)
{
    event_t event;
    
    timer_init();
    timer_start((uint16_t)1);
    event = eh_get_event();
    
    printf("event = %d\n", event);
    CHECK_TEXT(event == TIME_OUT, "no TIME_OUT generated");
}


/// END: To be programmed


/* test_edge_detection:
   initialize the GPIOB->IDR to zero.
   implements three calls to eh_get_event() 
   after each call the return value is checked
   1. no event after first call
   2. correct event occurs if pin GPIOB.0 is set to one before second call
   3. no event if pin GPIOB.0 is reset to zero before third call */

/// STUDENTS: To be programmed
TEST(event_handler,test_edge_detection)
{
    event_t event;

    GPIOB->IDR = 0x0000; 
    event = eh_get_event();
    printf("event = %d\n", event);   
    CHECK_TEXT(event == NO_EVENT, "not NO_EVENT");
    
    GPIOB->IDR = 0x0001; 
    event = eh_get_event();
    printf("event = %d\n", event);   
    CHECK_TEXT(event == EV_CAR_W, "not EV_CAR_W");
    
    GPIOB->IDR = 0x0000; 
    event = eh_get_event();
    printf("event = %d\n", event);   
    CHECK_TEXT(event == NO_EVENT, "not NO_EVENT");
}



/// END: To be programmed

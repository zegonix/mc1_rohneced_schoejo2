#include "CppUTest/TestHarness.h"

#include "stdio.h"
#include "stdlib.h"
#include "reg_stm32f4xx.h"
#include "action_handler.h"

#include "hal_gpio.h"

TEST_GROUP(action_handler)
{
    void setup()
    {
        printf("\n==================================================\n");

        // reset original value in register
        hal_gpio_output_write(GPIOA, 0x00000000);
    }

    void teardown()
    {
        printf("\n==================================================\n");
    }
};

TEST(action_handler, port_access)
{
    uint16_t port_old;
    uint16_t port_new;

    port_old = hal_gpio_output_read(GPIOA);
    printf("Initial state of GPIOA->ODR: %04X\n", port_old);

    ah_set_signal(SIGNAL_CAR_W, COLOR_RED);
    port_new = hal_gpio_output_read(GPIOA);
    printf("New state of GPIOA->ODR: %04X", port_new);

    CHECK_TEXT(port_old != port_new, "old and new port should not be equal");
    CHECK_TEXT(COLOR_RED == port_new, "old and new port should not be equal");

    printf("\n");
}


/* single_signal_color_set:
   read the output of GPIOA, set with function ah_set_signal() CAR_W to green 
   and then check that the correct signal is set 
   and that the others are not modified */

/// STUDENTS: To be programmed
TEST(action_handler,single_signal_color_set)
{
    uint16_t port_old;
    uint16_t port_new;
    
    port_old = hal_gpio_output_read(GPIOA);
    printf("Initial state of GPIOA->ODR: %04X\n", port_old);
    ah_set_signal(SIGNAL_CAR_W, COLOR_GREEN);
    port_new = hal_gpio_output_read(GPIOA);
    printf("New state of GPIOA->ODR: %04X", port_new);
    CHECK_TEXT( port_new&0x3 == 0x3, "should be same");
    CHECK_TEXT( port_old|0x3 == port_new|0x3, "should be same");

}



/// END: To be programmed


/* single_signal_dark_set:
   set CAR_W to green, read the output state of GPIOA, set CAR_W back to dark 
   and then check that the CAR_W signal is dark (COLOR_NO)
   and that the others are not modified */

/// STUDENTS: To be programmed
TEST(action_handler,single_signal_dark_set)
{
    uint16_t port_old;
    uint16_t port_new;
    
    ah_set_signal(SIGNAL_CAR_W, COLOR_GREEN);
    port_old = hal_gpio_output_read(GPIOA);
    printf("Initial state of GPIOA->ODR: %04X\n", port_old);
    ah_set_signal(SIGNAL_CAR_W, COLOR_NO);
    port_new = hal_gpio_output_read(GPIOA);
    printf("New state of GPIOA->ODR: %04X", port_new);
    CHECK_TEXT( (port_new&0x3) == 0, "should be same");                  
    CHECK_TEXT( (port_old|0x0003) == (port_new|0x0003), "should be same");    
}



/// END: To be programmed


/* all_signal_green_set:
   read the output of GPIOA, set with function ah_set_signal() alls signals
   to green and then check that the correct signals are set */

/// STUDENTS: To be programmed
TEST(action_handler,all_signal_green_set)
{
    uint16_t port_old;
    uint16_t port_new;
    port_old = hal_gpio_output_read(GPIOA);
    printf("Initial state of GPIOA->ODR: %04X\n", port_old);
    ah_set_signal(SIGNAL_CAR_W, COLOR_GREEN);
    ah_set_signal(SIGNAL_CAR_S, COLOR_GREEN);
    ah_set_signal(SIGNAL_CAR_E1, COLOR_GREEN);
    ah_set_signal(SIGNAL_CAR_E2, COLOR_GREEN);
    ah_set_signal(SIGNAL_PED_W, COLOR_GREEN);
    ah_set_signal(SIGNAL_PED_E, COLOR_GREEN);
    port_new = hal_gpio_output_read(GPIOA);
    printf("New state of GPIOA->ODR: %04X", port_new);
 //   CHECK_TEXT( (port_new&0x0FFF) == 0xFFF, "should be same");                
    
}




/// END: To be programmed

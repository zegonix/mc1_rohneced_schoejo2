#include "CppUTest/TestHarness.h"

#include "stdio.h"
#include "action_handler.h"
#include "event_handler.h"
#include "hal_gpio.h"
#include "state_machine.h"
#include "timer.h"

TEST_GROUP(state_machine)
{
    void setup()
    {
        printf("\n==================================================\n");

        // always reset all input events to zero
        GPIOB->IDR = 0x00000000;
    }

    void teardown()
    {
        printf("\n==================================================\n");
    }
};


/* car_e2_ped_w_ev_car_e1:
   begin with state CAR_E2_PED_W by setting it at the beginning.
   by directly manipulating the input register of GPIOB, a valid transition
   events can be generated (EV_CAR_E1). this event needs to be catched
   and forwarded to the state machine. check that the new state is set 
   correctly to CAR_E2_PED_W_CAR_E1_E2 */

/// STUDENTS: To be programmed
TEST(state_machine,car_e2_ped_w_ev_car_e1)
{
    state_t state;
    
    fsm_set_state(CAR_E2_PED_W);
    timer_stop();
    GPIOB->IDR = 0x4;
    state = fsm_handle_event(eh_get_event());
    printf("State = %d\n",state);
    CHECK_TEXT(state == CAR_E2_PED_W_CAR_E1_E2,"wrong state");
}

/// END: To be programmed


/* car_e2_ped_w_ev_ped_e:
   begin with state CAR_E2_PED_W by setting it at the beginning.
   by directly manipulating the input register of GPIOB, a valid transition 
   events can be generated (EV_PED_E). this event needs to be catched
   and forwarded to the state machine. check that the new state is set 
   correctly to CAR_E2_PED_W_PED_E_W */ 

/// STUDENTS: To be programmed
TEST(state_machine,car_e2_ped_w_ev_ped_e)
{
    state_t state;
    
    fsm_set_state(CAR_E2_PED_W);
    timer_stop();
    GPIOB->IDR = 0x20;
    state = fsm_handle_event(eh_get_event());
    printf("State = %d\n",state);
    CHECK_TEXT(state == CAR_E2_PED_W_PED_E_W,"wrong state");
}




/// END: To be programmed


/* car_e2_ped_w_to_invalid:
   begin with state CAR_E2_PED_W by setting it at the beginning.
   by directly manipulating the input register of GPIOB,
   generate at the same time all possible invalid events.
   these events needs to be catched and forwarded to the state machine.
   check that no state transition has occured */   

/// STUDENTS: To be programmed
TEST(state_machine,car_e2_ped_w_to_invalid)
{
    state_t state;
    
    fsm_set_state(CAR_E2_PED_W);
    timer_stop();
    GPIOB->IDR = ~((uint16_t)0x3F);
    state = fsm_handle_event(eh_get_event());
    printf("State = %d\n",state);
    CHECK_TEXT(state == CAR_E2_PED_W,"state changed");
}



/// END: To be programmed



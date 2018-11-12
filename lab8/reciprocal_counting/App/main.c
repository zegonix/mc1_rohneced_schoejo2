/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : MC1 lab 3 Frequenzmessung
 * -- Description :
 * --   #Reciprocal Counting 4.1 (Input GPIO Port A Pin 8):
 * --    This program counts with an internal fast clock using
 * --    timer 1. Timer 1 is setup in input capture mode.
 * --    The external fequency on PA 8 triggers on rising edge a
 * --    capture. Timer 1 fires a capture interrupt.
 * --    Now the capture value can be read and the frequency can
 * --    be calculated and displayed on lcd.
 * --   #Frequency multiplier 4.2 (Output GPIO Port A Pin 5):
 * --    With an interrupt the capture
 * --    value of timer 1 is loaded into timer 8 autoreload register.
 * --    By multiplying (dividing) the prescaler from timer 1 with the
 * --    hexswitch value and setting them as prescaler of timer 8, the
 * --    timer 8 now generates a multiplied frequency.
 * --    Timer 8 is set in output mode channel 1N, so the multiplied
 * --    frequency can be shown on the oscilloscope at gpio PA5.
 * --
 * -- $Id: main.c 945 2016-10-17 13:14:05Z feur $
 * --------------------------------------------------------------- */

/* user includes */
#include <stdint.h>
#include "reg_ctboard.h"
#include "display_freq.h"
#include "hal_timer.h"
#include "timers_freq.h"

/* user defines */
#define PRESCALER              140u           // prescaler timer 1
#define COUNTER_CLOCK (uint32_t)(((uint32_t)84000000) / PRESCALER)
#define TIMER_WAIT_FOR_LCD     0x9ffff


/* ------------------------------------------------------------------
 * Main
 * --------------------------------------------------------------- */

int main(void)
{
    /* local vars */
    /// STUDENTS: To be programmed




    /// END: To be programmed
  
    uint32_t wait = 0;

    init_measure_timer(PRESCALER);

    /* init timer 8, task 4.2 */
    init_frequency_multiplier();
    /* start counter of timer 1, task 4.1 */
    hal_timer_start(TIM1);
    /* start counter of timer 8, task 4.2 */
    hal_timer_start(TIM8);


    while (1) {
        /* main loop */
      
        /// STUDENTS: To be programmed




        /// END: To be programmed
      
        /*wait a little due to lcd */
        for (wait = 0; wait < TIMER_WAIT_FOR_LCD; wait++) ;
    }
}



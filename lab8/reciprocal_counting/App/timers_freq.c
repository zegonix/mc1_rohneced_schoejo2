/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : MC lab 3 - Frequenzmessung
 * -- Description : Implementation of module timers frequency.
 * --               Init timer 1 with an external capture channel on
 * --               GPIO PA8. At initialization the timer sets a prescaler
 * --               for the internal clock source (84 MHz).
 * --               Init timer 8 in output compare mode.
 * --               Outputpin is GPIO PA5.
 * --               At run time the prescaler of timer 8 can be set
 * --               depending on the hex switch. This allows setting
 * --               the timer speed. The multiplier factor (hex switch value)
 * --               is displayed on 7seg.
 * --               
 * --
 * -- $Revision: 946 $
 * -- $Id: timers_freq.c 945 2018-10-25 08:42:05Z gruj $
 * --------------------------------------------------------------- */

#include <stdint.h>
#include "reg_ctboard.h"
#include "timers_freq.h"
#include "hal_timer.h"
#include "hal_gpio.h"
#include "hal_rcc.h"
#include "hal_ct_seg7.h"


/* module-wide global variables */
static volatile uint8_t multiplier;
static volatile uint16_t prescaler_tim1;
/* memory for capture value */
static volatile uint32_t capture_value = 0;

/* prototypes for internal functions */
/// STUDENTS: To be programmed




/// END: To be programmed


/*
 * See header file
 */
void init_measure_timer(uint16_t prescaler)
{
    hal_gpio_output_t gpio;
    hal_timer_base_init_t timer_base;
    hal_timer_input_init_t timer_input;
    hal_timer_clock_init_t timer_clock;
    
    /* store parameter to module-wide global variable */
    prescaler_tim1 = prescaler;

    /* enable peripheral clock for timer 1 and gpio */
    TIM1_ENABLE();
    GPIOA_ENABLE();

  
    /* select alternate function mode to use gpio port for timer purpose.
     * gpio PA8 is directly mapped to timer 1 CH1.
     * althought PA8 is initialized as output pin, timer 1 can use 
     * PA8 either on CH1 as an output pin (for output compare mode)
     * or as an input pin (for input capture mode).
     */
    gpio.pins = (0x1 << 8u);
    gpio.pupd = HAL_GPIO_PUPD_DOWN;
    gpio.out_speed = HAL_GPIO_OUT_SPEED_100MHZ;
    gpio.out_type = HAL_GPIO_OUT_TYPE_PP;
    hal_gpio_init_alternate(GPIOA, HAL_GPIO_AF_TIM1, gpio);
  
  
    /* enable counter reset at capture
     */
    /* select internal clock source (CK_INT = 84MHz) as counter source */
    timer_clock.source = HAL_TIMER_CLKSRC_INT;
    /* select the filtered timer input 1 (TI1FP1) as the trigger input source 
     * for the trigger input (TRGI) */
    timer_clock.trg_source = HAL_TIMER_TRGSRC_TI1FP;
    /* trigger input (TRGI) resets the counter of timer 1 */
    timer_clock.slave_mode = HAL_TIMER_SLAVE_RESET;
    hal_timer_init_clock(TIM1, timer_clock);
    
    
    /* configure and enable the input capture channel 1 (IC1)
     */
    /* select the direct pin mapping, so CH1 is mapped to timer input 1 (TI1) */
    timer_input.input = HAL_TIMER_IC_SEL_DIRECT;
    /* no sampling filter ist needed on TI1 */
    timer_input.filter.mode = HAL_TIMER_FILTER_OFF;
    /* select TI1FP1 as input source for caption, such that the rising edge of TI1
     * triggers a caption */
    timer_input.filter.polarity = HAL_TIMER_POLARITY_HIGH;
    /* no prescaler is needed */
    timer_input.filter.prescaler = HAL_TIMER_PSC_OFF;
    /* enable capture mode on specified channel */
    timer_input.input_state = ENABLE;
    hal_timer_init_input(TIM1, HAL_TIMER_CH1, timer_input);
    

    /* init timer 1 as 16 bit upcounter 
     *    -> use hal_timer_init_base()
     * enable interrupt
     *    -> use hal_timer_irq_set()
     */

    /// STUDENTS: To be programmed

		timer_base.prescaler = 1;
		timer_base.mode = HAL_TIMER_MODE_UP;
		timer_base.run_mode = HAL_TIMER_RUN_CONTINOUS;
		timer_base.count = 0;
		hal_timer_init_base(TIM1, timer_base);
		hal_timer_irq_set(TIM1, 

    /// END: To be programmed
}


/*
 * See header file
 */
void init_frequency_multiplier(void)
{
    hal_gpio_output_t gpio;
    hal_timer_base_init_t timer_base;
    hal_timer_output_init_t timer_output;

    /* enable peripheral clock for timer and gpio */
    GPIOA_ENABLE();
    TIM8_ENABLE();

  
    /* select alternate function mode to use gpio for timer purpose.
     * gpio PA5 is directly mapped to timer 8 CH1N 
     * (CH1N can be used only for output purpose)
     * init PA5 as an output pin for timer 8 (output compare mode)
     */
    gpio.pins = (0x1 << 5u);
    gpio.pupd = HAL_GPIO_PUPD_DOWN;
    gpio.out_speed = HAL_GPIO_OUT_SPEED_100MHZ;
    gpio.out_type = HAL_GPIO_OUT_TYPE_PP;
    hal_gpio_init_alternate(GPIOA, HAL_GPIO_AF_TIM8, gpio);

  
    /* Init timer 8 as 16 bit downcounter
     *    -> use hal_timer_init_base()
     * configure timer 8 in output compare mode
     *    -> use hal_timer_init_output()
     */
     
    /// STUDENTS: To be programmed




    /// END: To be programmed

    /* Disable master mode on timer 8 */
    TIM8->SMCR &= ~(0x7 << 0u);
}

/*
 * See header file
 */
uint32_t get_capture_value(void)
{
    return capture_value;
}


/*
 * See header file
 */
void set_prescaler_freq_mul(void)
{
    /// STUDENTS: To be programmed




    /// END: To be programmed
}


/**
 *  \brief  Interrupt service routine for timer 1.
 *          Read out the new capture value and save.
 *          Load capture value into reload reg of timer 8.
 */
extern void TIM1_CC_IRQHandler(void)
{
    /// STUDENTS: To be programmed




    /// END: To be programmed
}


/* ------------------- internal functions ------------------- */
/// STUDENTS: To be programmed




/// END: To be programmed

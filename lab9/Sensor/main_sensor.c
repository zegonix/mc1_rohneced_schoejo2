/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : MC1 lab - energy efficient sensors
 * -- Description : This code controls the accelerometer LSM6DSL
 * --               and shows its values on the LCD display
 * --
 * -- $Id: main.c 1623 2018-10-17 09:00:42Z angv $
 * --------------------------------------------------------------- */

/* includes ---------------------------------------------------------------- */
#include <stdint.h>
#include "reg_ctboard.h"
#include "display_acc.h"
#include "hal_timer.h"
#include "accelerometer.h"
#include "hal_acc.h"

/* -- defines----------------------------------------------------------------*/
#define EXTI_PR_PR8                   ((uint32_t)0x00000100) // !< Pending bit for line 8
#define MIN_NR_OF_SAMPLES             100
#define NR_OF_AXES                    3

/* -- local variables declarations -------------------------------------------*/
/* buffer for acceleration */
static int16_t acc_buffer[MIN_NR_OF_SAMPLES * NR_OF_AXES];
static uint16_t sample_nr;             // Indicates how many sample(x,y,z value)
                                       // are currently in the buffer
static volatile uint8_t data_ready;    // indicates that an interrupt occured

/* -- local function declarations -------------------------------------------*/
//static void enter_sleep(void);
static void calculate_acc_average(int16_t *buffer, int16_t *result,
                                  uint16_t buffer_length);

static void calc_average_and_write_to_lcd(int16_t *acc_buffer,
                                          int16_t *acceleration,
                                          uint16_t sample_nr);


int main(void)
{
    /* --------
     * init
     * --------*/

    /* local vars */
    int16_t acceleration[3];

    /* init Sensor */
    accelerometer_init();

//    /* set clock */
//    power_set_clock();

//    /* Enable debugging while in sleep and stop mode */
//    DBG->CR |= (0x1 << 0u);
//    DBG->CR |= (0x1 << 1u);

    /* ------------------------------------------------------------------
     * MAIN LOOP
     * --------------------------------------------------------------- */
    while (1) {

                /// STUDENTS: To be programmed
                if (data_ready > 0) {
                    /* read acceleration and write how many data is collected
                       through FIFO into sample_nr. */
                    sample_nr = accelerometer_read_acceleration(acc_buffer);
                    calc_average_and_write_to_lcd(acc_buffer, acceleration,
                                                  sample_nr);
                    data_ready = 0;
                }
                /// END: To be programmed

//        enter_sleep();
    }
}

/**
 *  \brief  calculate the average of a buffer filled with 3 axis values
 *  \param  buffer: buffer which contain 3-axis values
 *  \param  result: array with 3 positions on
 *                  which the result will be stored
 *  \param  buffer_length: length of the input buffer
 */
static void calculate_acc_average(int16_t *buffer, int16_t *result,
                                  uint16_t buffer_length)
{
    int32_t x_sum = 0;
    int32_t y_sum = 0;
    int32_t z_sum = 0;
    uint16_t i;

    for (i = 0; i < buffer_length; i++) {
        x_sum += buffer[i * 3];
        y_sum += buffer[i * 3 + 1];
        z_sum += buffer[i * 3 + 2];
    }

    result[0] = (int16_t)(x_sum / buffer_length);
    result[1] = (int16_t)(y_sum / buffer_length);
    result[2] = (int16_t)(z_sum / buffer_length);
    return;
}

/**
 *  \brief  calling 2 functions for calculating an average value and write that
            to the lcd
 *   \param acc_buffer: buffer of acceleration data for calculating average
 *   \param acceleration: mean acceleration data. array size: 3
 *   \param sample_nr: Indicates how many samples are currently in the buffer
 */
static void calc_average_and_write_to_lcd(int16_t *acc_buffer,
                                          int16_t *acceleration,
                                          uint16_t sample_nr)
{
    /* calculate average values from acc_buffer*/
    calculate_acc_average(acc_buffer, acceleration, sample_nr);

    /* write acceleration to LCD */
    write_acc_to_lcd(acceleration);
}

///**
// *  \brief  set the uc in sleep mode
// */
//static void enter_sleep(void)
//{
//    SCB->SCR |= (0x1 << 2u);
//    __asm volatile ("wfi");
//    power_set_clock();
//}

/**
 *  \brief  Handles Extern interupt lines 5 to 9
 *          The interrupt indicates that a measurement of
 *          the accelerometer is complete.
 */
extern void EXTI9_5_IRQHandler(void)
{
    EXTI->PR |= EXTI_PR_PR8;
    data_ready = 1;
    return;
}


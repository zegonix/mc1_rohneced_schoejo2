/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : MC1 lab 4 MDA
 * -- Description : This code controls the accelerometer LSM6DSL
 * --               and shows its values on the LCD display
 * --
 * -- $Id: main.c 1583 2018-10-01 09:48:52Z angv $
 * ------------------------------------------------------------------------- */

/* includes ---------------------------------------------------------------- */
#include <stdint.h>
#include "display_acc.h"
#include "hal_timer.h"
#include "accelerometer.h"

/* -- defines----------------------------------------------------------------*/
/* Pending bit for line 9 */
#define EXTI_PR_PR9         ((uint32_t)0x00000200)

#define SAMPLES_FOR_AVERAGE 200

/* -- static variables declarations -----------------------------------------*/
static int16_t acc_buffer[SAMPLES_FOR_AVERAGE * 3]; // minimum 3 times the buffer size
static uint8_t data_ready = 0;  // Indicates if the FIFO buffer is full

/* -- static function declarations -------------------------------------------*/
static void calculate_acc_average(int16_t *buffer, int16_t *result,
                                  uint16_t buffer_length);

/* ------------------------------------------------------------------
 * Main
 * --------------------------------------------------------------- */
int main(void)
{
    int16_t acceleration[3];
    uint16_t sample_nr = 0;
    uint8_t i;

    accelerometer_init();

    //display default value
    acceleration[0] = 0;
    acceleration[1] = 0;
    acceleration[2] = 1000;
    write_acc_to_lcd(acceleration);
    
    while (1) {
        /* main loop */
        if (data_ready > 0) {
            /* read current acceleration */
            accelerometer_read_acceleration(acceleration);

            /* Save Acceleration to buffer */
            for (i = 0; i < 3; i++) {
                acc_buffer[sample_nr * 3 + i] = acceleration[i];
            }
            sample_nr += 1;
            data_ready = 0;
        }
        /* Check if enough samples are collected, calculate
           average and display the values	*/
        if (sample_nr > SAMPLES_FOR_AVERAGE) {
            /* calculate average values from acc_buffer,
               result saved in variable -> acceleration */
            calculate_acc_average(acc_buffer, acceleration, sample_nr);
            write_acc_to_lcd(acceleration);
            sample_nr = 0;
        }
    }
}


/* internal functions definitions ------------------------------------------ */

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
    /// STUDENTS: To be programmed




    /// END: To be programmed    
    return;
}


/**
 *  \brief  Handles Extern interupt lines 5 to 9
 *          The interrupt indicates that the FIFO buffer
 *          is filled to the watermark
 */
extern void EXTI9_5_IRQHandler(void)
{
    /* clear pending bit */
    EXTI->PR |= EXTI_PR_PR9;

    /* read Acceleration */
    data_ready = 1;

    return;
}


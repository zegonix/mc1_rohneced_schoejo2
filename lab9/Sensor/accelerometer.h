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
 * -- Description : This modul handles the commands of the Accelerometer
 *                  of the Type LSM6DSL. The access to the peripherals is
 *                  executed trough the file hal_acc.h.
 * -- $Revision: 1623 $
 * -- $Id: accelerometer.h 1623 2018-10-17 09:00:42Z angv $
 * --------------------------------------------------------------- */

#ifndef _ACCELEROMETER
#define _ACCELEROMETER

#include <stdint.h>

/* public function declarations -------------------------------------------- */

/* Defines */

/**
 *  \brief  Setup SPI, GPIOs and DMA for the Accelerometer LSM6DSL,all registers
 *          are initialized and a measurement is started
 *          The sensor is initialized with the following settings
 *          (values in lsm6dsl_reg.h):
 *          Measurement:                Acceleration
 *          Range:                      +- 4 G
 *          Measure Freq(ODR):          104 Hz
 *          Interupt 1:                 Acceleration data ready
 */
void accelerometer_init(void);


/**
 *  \brief  Reads the Acceleration Data of the LSM6DSL.
 *          Determines automatically (depending on switch_position) if dma or
 *          continous mode is necessary. can be used for both.
 *  \param  int16_t *acceleration: array with space for the whole fifo
 *  \param  transmission_mode switch_position: enum-element of the chosen mode
 *  \return number of values read by the function and stored in the array
 */
uint16_t accelerometer_read_acceleration(int16_t *acceleration);

/* end header ----------------------------------------------------- */
#endif

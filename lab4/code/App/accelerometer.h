/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : MC lab 4 - DMA
 * -- Description : This module handles the commands of the Accelerometer
 *                  of the Type LSM6DSL. The access to the peripherals
 *                  is executed through the file hal_acc.c
 *                  The addresses for register initialitation are written
 *                  in the file lsm6dsl_reg.h
 * -- $Revision: 1577 $
 * -- $Id: accelerometer.h 1577 2018-09-28 09:23:20Z angv $
 * --------------------------------------------------------------- */

#ifndef _ACCELEROMETER
#define _ACCELEROMETER

#include <stdint.h>

/* public function declarations -------------------------------------------- */

/**
 *  \brief  Setup SPI, GPIOs and DMA for the Accelerometer LSM6DSL,
 *          initializing all registers and starting a measurement.
 *          The sensor is initialized with the following settings:
 *          Measurement:         Acceleration
 *          Range:               +/- 4G
 *          Measure Freq(ODR):   833 Hz
 *          Interupt 1:          Acceleration data ready
 */
void accelerometer_init(void);


/**
 *  \brief  Reads the id of accelerometer with SPI
 */
uint8_t accelerometer_read_acc_id(void);

/**
 *  \brief  Reads the current Acceleration Data of the LSM6DSL (Reg 0x28-2D)
 *  \param  *acceleration: array on which the acceleration Data will be stored.
 */
void accelerometer_read_acceleration(int16_t *acceleration);


/* end header ----------------------------------------------------- */
#endif

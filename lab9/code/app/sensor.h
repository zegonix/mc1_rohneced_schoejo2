/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Project     : MC1 Lab05 Encoder
 * -- Description : This code controls the motor
 * --
 * --
 * -- $Id: sensor.h 1562 2018-09-23 16:50:09Z ruan $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _SENSOR_H
#define _SENSOR_H

/* User includes */
#include <stdint.h>

/* Public defines -----------------------------------------------------------*/
#define SENSOR_U6 0x00
#define SENSOR_U5 0x01
#define SENSOR_U4 0x02
#define SENSOR_U3 0x03

/* Public function definitions -----------------------------------------------*/

/**
 *  \brief  Initialize the GPIOs for the sensor inputs and outputs
 *  \param  void: -
 */
void sensor_init(void);

/**
 *  \brief  Reads all pins on the phototransistors
 *  \param  void: -
 *  \return Measured value on the phototransistor
 */
uint8_t sensor_read_all(void);

/**
 *  \brief  Reads a selected pin on the phototransistors
 *  \param  sensor_id : the number of the selected pin
 *  \return Measured value on the selected phototransistor
 */
uint8_t sensor_read_single(uint8_t sensor_id);

/**
 *  \brief  Turns on all Infrared-LEDs
 *  \param  void: -
 */
void sensor_enable_all(void);

/**
 *  \brief  Turns off all Infrared-LEDs
 *  \param  void: -
 */
void sensor_disable_all(void);

/**
 *  \brief  Turns on a selected Infrared-LEDs
 *  \param  sensor_id: value selected LED which should turn on
 */
void sensor_enable_single(uint8_t sensor_id);

/**
 *  \brief  Turns off a selected Infrared-LEDs
 *  \param  sensor_id: value selected LED which should turn off
 */
void sensor_disable_single(uint8_t sensor_id);
#endif

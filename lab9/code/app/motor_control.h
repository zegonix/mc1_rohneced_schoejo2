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
 * -- $Id: motor_control.h 1561 2018-09-23 16:48:07Z ruan $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _MOTOR_CONTROL_H
#define _MOTOR_CONTROL_H

/* User includes */
#include <stdint.h>

/* Public defines  -----------------------------------------------------------*/

/* Public function definitions -----------------------------------------------*/

/**
 *  \brief  Initializes the motor
 *  \param  void: -
 */
void motor_init(void);

/**
 *  \brief  Controls the motor and the speed of the motor
 *  \param  void: -
 */
void motor_control_excecute(void);
#endif

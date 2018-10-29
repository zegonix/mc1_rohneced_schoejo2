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
 * -- Description : Edge detection of the buttons
 * --
 * --
 * -- $Id: buttons.h 1562 2018-09-23 16:50:09Z ruan $
 * ------------------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _BUTTONS_H
#define _BUTTONS_H

/* User includes */
#include <stdint.h>
#include <hal_common.h>

/* Public defines -----------------------------------------------------------*/

/* Public function definitions -----------------------------------------------*/

/**
 *  \brief  Detect change of buttons (rising or falling edge)
 *  \param  void: -
 */
void button_detection(void);

/**
 *  \brief  Returns the pressed state of the botton T0
 *  \param  void: -
 *  \return True if rising edge detected, else false
 */
hal_bool_t button_T0_pressed(void);

/**
 *  \brief  Returns the pressed state of the botton T1
 *  \param  void: -
 *  \return True if rising edge detected, else false
 */
hal_bool_t button_T1_pressed(void);

/**
 *  \brief  Returns the released state of the botton T0
 *  \param  void: -
 *  \return True if falling edge detected, else false
 */
hal_bool_t button_T0_released(void);

/**
 *  \brief  Returns the released state of the botton T1
 *  \param  void: -
 *  \return True if falling edge detected, else false
 */
hal_bool_t button_T1_released(void);
#endif

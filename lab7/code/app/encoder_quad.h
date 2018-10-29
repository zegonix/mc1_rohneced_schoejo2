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
 * -- Description : This code controls the module encoder quadrature
 * --
 * --
 * -- $Id: encoder_quad.h 1562 2018-09-23 16:50:09Z ruan $
 * ------------------------------------------------------------------------- */

/* re-definition guard */
#ifndef _ENCODER_QUAD_H
#define _ENCODER_QUAD_H

/* user includes */
#include <stdint.h>

/* Public defines -----------------------------------------------------------*/
typedef enum {
    CW = 0x0,      // Rotation direction of encoder is counterclockwise
    CCW = 0x1      // Rotation direction of encoder is clockwise
} cw_ccw_direction;

/* Public function definitions -----------------------------------------------*/

/**
 *  \brief  Initialize the timer for encoder freq measurement,
 *  \brief  initialize and activate sensors,
 *  \brief  initialize interrupts
 *  \param  -
 */
void encoder_quad_init(void);

/// STUDENTS: To be programmed




/// END: To be programmed
#endif

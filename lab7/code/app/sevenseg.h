/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : MC1 Lab05 Encoder
 * -- Description : Interface for 7-Seg
 * --
 * -- Revised Version.
 * -- Original author: trug
 * --
 * -- $Revision: 1562 $
 * -- $Id: sevenseg.h 1562 2018-09-23 16:50:09Z ruan $
 * --------------------------------------------------------------- */

/* Re-definition guard */
#ifndef _SEVENSEG
#define _SEVENSEG

/* User includes */
#include <stdint.h>

/* Public defines -----------------------------------------------------------*/

/* Public function definitions -----------------------------------------------*/

/**
 *  \brief  displays disc sector on seven segemnt lcd in numbers
 *  \param  disc_sector: contains actual sector (1 to 4) of the disc
 */
void show_quadrant_7seg_nbrs(uint8_t disc_sector);


/**
 *  \brief  displays disc sector on seven segemnt lcd in rotating bars
 *  \param  disc_sector: contains actual sector (1 to 4) of the disc
 */
void show_quadrant_7seg_bars(uint8_t disc_sector);


/**
 *  \brief  shows encoder location on seven segemnt lcd
 *  \param  enc_location: contains actual location (0 to 15) of encoder disc
 */
void show_enc_location_7seg(uint8_t enc_location);


/* end header ----------------------------------------------------- */
#endif

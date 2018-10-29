/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : MC1 Lab05 MEncoder
 * -- Description : Interface for 7-Seg
 * --
 * -- Revised Version.
 * -- Original author: trug
 * --
 * -- $Revision: 1562 $
 * -- $Id: sevenseg.c 1562 2018-09-23 16:50:09Z ruan $
 * --------------------------------------------------------------- */

/* user includes */
#include <stdint.h>
#include <stdio.h>
#include "reg_ctboard.h"
#include "sevenseg.h"


const static uint8_t dec2seven_nbrs_lt[10] =
//  0     1    2      3     4    5      6     7    8     9
{ 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90 };

/* Use this table for inverse direction */
// seven segments:                      off    -    |    _   |
// const uint8_t dec2seven_lt[5]   =		{0xFF,0xFE,0xFD,0xBF,0xDF};

/* Use this table for normal direction */
// seven segments:                             off    -    |     _     |
const static uint8_t dec2seven_bars_lt[5] = { 0xFF, 0xFE, 0xDF, 0xBF, 0xFD };


/*
 * See header file
 */
void show_quadrant_7seg_nbrs(uint8_t disc_sector)
{
    CT_SEG7->RAW.BYTE.DS0 = dec2seven_nbrs_lt[disc_sector];
}


/*
 * See header file
 */
void show_quadrant_7seg_bars(uint8_t disc_sector)
{
    CT_SEG7->RAW.BYTE.DS0 = dec2seven_bars_lt[disc_sector];
}

/*
 * See header file
 */
void show_enc_location_7seg(uint8_t enc_location)
{
    uint8_t ones = enc_location % 10;
    uint8_t tens = enc_location / 10;

    // Write tens
    CT_SEG7->RAW.BYTE.DS3 = dec2seven_nbrs_lt[tens];

    // Write ones
    CT_SEG7->RAW.BYTE.DS2 = dec2seven_nbrs_lt[ones];
}

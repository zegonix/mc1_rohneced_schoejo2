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
 * -- Description : Implementation of module display.
 * --               Write the measured acceleration on the LC-Display.
 * --
 * --
 * -- $Revision: 1577 $
 * -- $Id: display_acc.h 1577 2018-09-28 09:23:20Z angv $
 * --------------------------------------------------------------- */


#ifndef _DISPLAY_ACC
#define _DISPLAY_ACC


#include <stdint.h>


/* type definitions ---------------------------------------------- */

/* macros for lcd */
#define RGB_CONTR_RED   (*((volatile uint16_t *)(0x60000304)))
#define RGB_CONTR_GREEN (*((volatile uint16_t *)(0x60000306)))
#define RGB_CONTR_BLUE  (*((volatile uint16_t *)(0x60000308)))
#define LCD_ASCII_CONTR (*((volatile uint16_t *)(0x60000310)))
#define LCD_ASCII_BSY   (*((volatile uint16_t *)(0x60000311)))


/* user defines */
#define CHARS_AT_ROW      14u
#define START_POS_SEC_ROW 40u


/* enum for display row */
typedef enum {
    DISP_ROW_ONE = 0x0,
    DISP_ROW_TWO = 0x1,
    DISP_ROW_BOTH = 0x2
}display_row;


/* public function declarations -------------------------------------------- */


/**
 * \brief   Convert binary value to bcd  value and write it on lcd.
 * \param   measured_acc: Binary Acceleration value.
 */
void write_acc_to_lcd(int16_t *measured_acc);

/* end header ----------------------------------------------------- */
#endif

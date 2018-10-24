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
 * -- Description : Implementation of module display.
 * --               Init the lcd display and write the measured
 * --               acceleration on the lcd display.
 * --
 * --
 * -- $Revision: 1607 $
 * -- $Id: display_acc.h 1607 2018-10-11 13:42:52Z angv $
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
 * \brief   Initialize lcd display with a background color and write
 *          the static text for frequence measurement to lcd.
 * \param   red : Background red, value between 0x0000 to 0xffff.
 * \param   green : Background green, value between 0x0000 to 0xffff.
 * \param   blue : Background blue, value between 0x0000 to 0xffff.
 */
void init_lcd_background_color(uint16_t red,
                               uint16_t green,
                               uint16_t blue);


/**
 * \brief   Write the current acceleration on the lcd.
 * \param   measured_acc: Array of acceleration data (usual size: 3).
 */
void write_acc_to_lcd(int16_t *measured_acc);


/**
 *  \brief  Write a character to the pos on lcd display.
 *  \param  character : Char to write on lcd.
 *  \param  pos : Position on lcd.
 */
void write_char_to_lcd(char character, uint8_t pos);


/* end header ----------------------------------------------------- */
#endif

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
 * -- $Id: display_acc.c 1577 2018-09-28 09:23:20Z angv $
 * --------------------------------------------------------------- */


/* includes ---------------------------------------------------------------- */
#include <stdint.h>
#include "display_acc.h"
#include "hal_ct_lcd.h"

/* -- defines----------------------------------------------------------------*/
#define WAIT_TIME 0x3fff


/* -- local variables declarations ------------------------------------------*/


/* -- local function declarations -------------------------------------------*/
static int binary_to_bcd_char_array(uint32_t binary,
                                    char *bcd_char_array_ret,
                                    uint8_t size_of_array);

static void reset_lcd_display_row(display_row row);

static void write_char_to_lcd(char character, uint8_t pos);

/* public function definitions --------------------------------------------- */

/*
 * See header file
 */
void write_acc_to_lcd(int16_t *measured_acc)
{
    uint8_t i = 0;
    uint8_t digs = 1;
    char integer_char[16];
    char si_unit[] = "  x      y      z";
    uint8_t sign = 0;
    uint32_t unsigned_acc;
    uint8_t axis;

    reset_lcd_display_row(DISP_ROW_BOTH);

    /* write axis to LCD */
    digs = sizeof(si_unit) - 1;
    for (i = 0; i < digs; i++) {
        write_char_to_lcd(si_unit[i], i);
    }

    /* write all three values to LCD */
    for (axis = 0; axis < 3; axis++) {
        /* get sign and convert to unsigned */
        sign = (measured_acc[axis] & 0x8000) >> 15;

        /* write sign on LCD */
        if (sign) {
            unsigned_acc = (uint32_t)(~measured_acc[axis] + 1);
            write_char_to_lcd('-', 20 + axis * 7);
        }else {
            unsigned_acc = (uint32_t)measured_acc[axis];
        }

        /* Display Acceleration  */
        integer_char[12] = '0';
        integer_char[13] = '0';
        integer_char[14] = '0';
        integer_char[15] = '0';
        digs = binary_to_bcd_char_array(unsigned_acc, integer_char,
                                        sizeof(integer_char));
        for (i = 15; i > 15 - 4; i--) {
            write_char_to_lcd(integer_char[i], i + 9 + 7 * axis);
        }
    }
}

/**
 *  \brief  Write a character to the pos on lcd display.
 *  \param  character : Char to write on lcd.
 *  \param  pos : Position on lcd.
 */
static void write_char_to_lcd(char character, uint8_t pos)
{
    uint16_t i;
    for (i = WAIT_TIME; i <= 0; i--);
    hal_ct_lcd_write(pos, &character);
    for (i = WAIT_TIME; i <= 0; i--);
}


/**
 *  \brief  Reset the chars on the lcd display.
 *  \param  row : Row to reset.
 */
static void reset_lcd_display_row(display_row row)
{
    switch (row) {
        case DISP_ROW_ONE:
            hal_ct_lcd_write(0x00, "          ");
            break;
        case DISP_ROW_TWO:
            hal_ct_lcd_write(0x10, "          ");
            break;
        default:
            hal_ct_lcd_clear();
            break;
    }
}


/**
 *  \brief  Calculate from a value the bcd number
            and returns it in a char array.
 *          Every 3 digs (1000er) is inserted a space.
 *          Eg. int 123456789 to char array {1,2,3,.,4,5,6, ,7,8,9}
 *  \param  binary  : The value to convert.
 *  \param  bcd_char_array_ret :
            Pointer to char array, which save the bcd value (return).
 *  \param  size_of_array : Size of char array bcd_char_array_ret.
 *  \return Nummber of digs in the char array.
 */
static int binary_to_bcd_char_array(uint32_t binary,
                                    char *bcd_char_array_ret,
                                    uint8_t size_of_array)
{
    int i = 0, tenner = 0, counter = 0;
    for (i = size_of_array - 1; i >= 0; i--) {
        bcd_char_array_ret[i] = binary % 10 + 48;
        binary = binary / 10;
        if (binary == 0) {
            break;
        }
        tenner++;
        counter++;
    }
    return counter;
}

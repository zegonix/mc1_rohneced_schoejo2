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
 * -- $Id: buttons.c 1562 2018-09-23 16:50:09Z ruan $
 * --------------------------------------------------------------------------*/

/* Includes -----------------------------------------------------------------*/
#include "reg_ctboard.h"
#include "buttons.h"

/* User define    -----------------------------------------------------------*/
#define MASK_BUTTONS 0x03
#define MASK_T0      0x01
#define MASK_T1      0x02

/* Variables and functions with module wide scope ----------------------------*/
static volatile hal_bool_t T0_pressed = FALSE;
static volatile hal_bool_t T1_pressed = FALSE;
static volatile hal_bool_t T0_released = FALSE;
static volatile hal_bool_t T1_released = FALSE;

/* Public function definitions ----------------------------------------------*/

/*
 * See header file
 */
void button_detection(void)
{
    static uint8_t old_buttons_value = 0;
    uint8_t buttons_value = 0;
    uint8_t button_rising = 0;
    uint8_t button_falling = 0;

    // Read and mask button state
    buttons_value = CT_BUTTON;
    buttons_value &= MASK_BUTTONS;
    // Detect edges
    button_rising = (buttons_value & (~old_buttons_value));
    // Detect edges
    button_falling = ((~buttons_value) & old_buttons_value);
    // Store button state for next detection
    old_buttons_value = buttons_value;

    if (button_rising & MASK_T0) {
        T0_pressed = TRUE;
    } else {
        T0_pressed = FALSE;
    }

    if (button_rising & MASK_T1) {
        T1_pressed = TRUE;
    } else {
        T1_pressed = FALSE;
    }

    if (button_falling & MASK_T0) {
        T0_released = TRUE;
    } else {
        T0_released = FALSE;
    }

    if (button_falling & MASK_T1) {
        T1_released = TRUE;
    } else {
        T1_released = FALSE;
    }
}

/*
 * See header file
 */
hal_bool_t button_T0_pressed(void)
{
    return T0_pressed;
}

/*
 * See header file
 */
hal_bool_t button_T1_pressed(void)
{
    return T1_pressed;
}

/*
 * See header file
 */
hal_bool_t button_T0_released(void)
{
    return T0_released;
}

/*
 * See header file
 */
hal_bool_t button_T1_released(void)
{
    return T1_released;
}

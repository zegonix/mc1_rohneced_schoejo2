/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Project     : CT2 Lab Matrix-Keypad
 * -- Description : Modul handles the reading of the keypad
 * --
 * -- $Id: keypad.c 1440 2018-08-21 15:51:07Z ruan $
 * ------------------------------------------------------------------------- */

#include "keypad.h"
#include "hal_gpio.h"

/* -- Macros, type definitions and function prototypes
 * --------------------------------------------------------------------------*/
 
/// STUDENTS: To be programmed




/// END: To be programmed

/* internal variable definitions ------------------------------------------- */

static const int key_array [4][4] = { { 0x1, 0x2, 0x3, 0xF },
                                      { 0x4, 0x5, 0x6, 0xE },
                                      { 0x7, 0x8, 0x9, 0xD },
                                      { 0xA, 0x0, 0xB, 0xC } };


/* public function definitions --------------------------------------------- */
                                      
/*
 * See header file
 */
uint8_t scan_keypad_cbc(void)
{
    uint8_t ret_val = NOKEY_PRESSED;
    /// STUDENTS: To be programmed

		

    /// END: To be programmed
    return ret_val;
}

/*
 * See header file
 */
uint8_t scan_keypad_fast(void)
{
    uint8_t ret_val = NOKEY_PRESSED;
    /// STUDENTS: To be programmed




    /// END: To be programmed
    return ret_val;
}

/* internal functions definitions ------------------------------------------ */
/// STUDENTS: To be programmed

/*
* config: 0 = input, 1 = output
 */
void pin_config(uint8_t config)
{
	hal_gpio_input_t input = {~config, HAL_GPIO_PUPD_UP};
	hal_gpio_output_t output = { config,
                               HAL_GPIO_PUPD_UP,
                               HAL_GPIO_OUT_SPEED_2MHZ,
                               HAL_GPIO_OUT_TYPE_PP };
	hal_gpio_init_input(GPIOA, input);
	hal_gpio_init_output(GPIOA, output);
}

/// END: To be programmed


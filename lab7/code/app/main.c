/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Project     : MC1 Lab Encoder
 * -- Description : This code controls the dc-motor and the encoders
 * --
 * --				The value of the encoded position will be displayed
 * --				With a button the Forward-Reverse direction can be set
 * --				For more information see description of the lab.
 * --
 * -- $Id: main.c 1561 2018-09-23 16:48:07Z ruan $
 * ------------------------------------------------------------------------- */
#include <stdint.h>
#include <stdio.h>

/* User includes */
#include "hal_gpio.h"
#include "motor_control.h"
#include "encoder_quad.h"
#include "hal_ct_lcd.h"
#include "sevenseg.h"


/* User define    -----------------------------------------------------------*/
#define MASK_DIPSW_S31 0x80
#define LCD_ADDR_LINE1 0u
#define LCD_ADDR_LINE2 20u
#define LCD_CLEAR      "                    "
#define LCD_ON         0xffff
#define LCD_OFF        0x0000

#define LED_D2         0x010
#define LED_D1         0x020

/* Local function declarations --------------------------------------------- */
static void init_components(void);

/* MAIN FUNCTION ----------------------------------------------------------- */
int main(void)
{
    //----------------------------------------------------
    // Initialization of the variables
    //----------------------------------------------------

    /// STUDENTS: To be programmed

		uint32_t counter=0;
		char * temp;

    /// END: To be programmed

    // Initializes all components

		init_components();

    //----------------------------------------------------
    // Endless loop :
    //----------------------------------------------------
    while (1) {
        /// STUDENTS: To be programmed
				if(getSensors()==(1<<U6))
				{
					counter++;
					resetSensors();
				}
				sprintf(temp, "c:%d, n:%d", counter, counter>>4);
				hal_ct_lcd_write(0, temp);

        /// END: To be programmed

        // Function processes potentiometer to control the motor
        motor_control_excecute();
    }
}

/**
 *  \brief  Initializes components: Encoder | Motor | Poti | Display
 *  \param  state: define used encoder to make encoder specific initialisation
 */
static void init_components(void)
{
    // Disable all interrupts
    RCC->CIR = 0x00000000;

    // Enable syscfg clock
    SYSCFG_ENABLE();

    // Initialize display and apply background color
    hal_ct_lcd_color(HAL_LCD_GREEN, LCD_ON);  // green on
    hal_ct_lcd_color(HAL_LCD_RED, LCD_OFF);   // red off
    hal_ct_lcd_color(HAL_LCD_BLUE, LCD_OFF);  // blue off

    // Clear display
    hal_ct_lcd_clear();

    // Initializes PWM signal, speed, direction for the motor
    motor_init();

    encoder_quad_init();
}

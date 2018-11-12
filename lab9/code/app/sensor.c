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
 * -- Description : This code controls the motor
 * --
 * --
 * -- $Id: sensor.c 1562 2018-09-23 16:50:09Z ruan $
 * ------------------------------------------------------------------------- */

/* Includes ------------------------------------------------------------------*/
#include "sensor.h"
#include "hal_gpio.h"

/* User define    -----------------------------------------------------------*/
#define SENSOR_ALL 0x001 | 0x002 | 0x004 | 0x008

/* Variables and functions with module wide scope ----------------------------*/

/* Public function definitions ----------------------------------------------*/

/*
 * See header file
 */
void sensor_init(void)
{
    hal_gpio_output_t gpio_output_init_struct;
    hal_gpio_input_t gpio_input_init_struct;

    // Enable clock for peripheral
    GPIOB_ENABLE();

    // Initialize input pin for processing sensor signal
    gpio_input_init_struct.pins = 0x100 | 0x200 | 0x400 | 0x800;
    gpio_input_init_struct.pupd = HAL_GPIO_PUPD_NOPULL;
    hal_gpio_init_input(GPIOB, gpio_input_init_struct);

    // Initialize output pin for using sensor U3-U6
    gpio_output_init_struct.out_speed = HAL_GPIO_OUT_SPEED_100MHZ;
    gpio_output_init_struct.out_type = HAL_GPIO_OUT_TYPE_PP;
    gpio_output_init_struct.pupd = HAL_GPIO_PUPD_NOPULL;
    gpio_output_init_struct.pins = 0x001 | 0x002 | 0x004 | 0x008;
    hal_gpio_init_output(GPIOB, gpio_output_init_struct);
}

/*
 * See header file
 */
uint8_t sensor_read_all(void)
{
    uint16_t gpios_port_b = 0;
    uint8_t encoder = 0;

    gpios_port_b = hal_gpio_input_read(GPIOB);
    encoder = (gpios_port_b & 0x0F00) >> 8;

    return encoder;
}

/*
 * See header file
 */
uint8_t sensor_read_single(uint8_t sensor_id)
{
    uint16_t gpios_port_b = 0;
    uint8_t encoder = 0;

    gpios_port_b = hal_gpio_input_read(GPIOB);
    encoder = ((gpios_port_b & 0x0F00) >> (sensor_id + 8)) & 0x01;

    return encoder;
}

/*
 * See header file
 */
void sensor_enable_all(void)
{
    // Turn on all encoders
    hal_gpio_bit_set(GPIOB, SENSOR_ALL);
}

/*
 * See header file
 */
void sensor_disable_all(void)
{
    // Turn off all encoders
    hal_gpio_bit_reset(GPIOB, SENSOR_ALL);
}

/*
 * See header file
 */
void sensor_enable_single(uint8_t sensor_id)
{
    if (sensor_id < 4) {
        hal_gpio_bit_set(GPIOB, (1 << sensor_id));
    }
}

/*
 * See header file
 */
void sensor_disable_single(uint8_t sensor_id)
{
    if (sensor_id < 4) {
        hal_gpio_bit_reset(GPIOB, (1 << sensor_id));
    }
}

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
 * -- Description : This module handles the commands of the Accelerometer
 *                  of the Type LSM6DSL. The access to the peripherals
 *                  is executed through the file hal_acc.c
 *                  The addresses for register initialitation are written
 *                  in the file lsm6dsl_reg.h
 * -- $Revision: 1591 $
 * -- $Id: accelerometer.c 1591 2018-10-02 11:10:19Z angv $
 * ------------------------------------------------------------------------- */

/* includes ---------------------------------------------------------------- */
#include <stdint.h>
#include "accelerometer.h"
#include "hal_acc.h"
#include "lsm6dsl_reg.h"
#include "reg_ctboard.h"

/* -- defines----------------------------------------------------------------*/

/* Correction factors for accelerometer */
#define          X_OFFSET              0
#define          Y_OFFSET              0
#define          Z_OFFSET              0

#define         NR_OF_DMA_TRANSACTIONS 7    //1 Byte to send, 6 to receive

/* -- global variables declarations -----------------------------------------*/
static uint8_t tx_buffer[NR_OF_DMA_TRANSACTIONS]; // Transmition buffer for SPI
static uint8_t rx_buffer[NR_OF_DMA_TRANSACTIONS]; // Reception buffer for SPI


/* -- local function declarations -------------------------------------------*/
static void write_reg(uint8_t address, uint8_t value);

/* public function definitions --------------------------------------------- */

/*
 * See header file
 */
void accelerometer_init(void)
{
    /* init Peripherals */
    hal_acc_init(tx_buffer, rx_buffer, NR_OF_DMA_TRANSACTIONS);

    /* Reset Sensor */
    write_reg(CTRL3_C_ADD, 0x01);   //Reset Sensor

    /// STUDENTS: To be programmed




    /// END: To be programmed
}

/**
 *  See header file
 */
void accelerometer_read_acceleration(int16_t *acceleration)
{
    int16_t x = 0;
    int16_t y = 0;
    int16_t z = 0;
    int16_t max_integer_val = 32767;
    int16_t max_G_force = 4000;
    

    /* Acceleration Register -> other Bytes can stay 0. 
     * Sensor increments adresses automatically. */
    tx_buffer[0] = 0xA8;

    /* chose between polling (0) or DMA (1) using switch S0 */
    if ((CT_DIPSW->BYTE.S7_0 & 0x01) == 0) {
        hal_acc_spi_read_write(NR_OF_DMA_TRANSACTIONS, tx_buffer, rx_buffer);
        CT_LCD->BG.BLUE = (uint16_t)0x0000;
        CT_LCD->BG.RED = (uint16_t)0xFFFF;      //Display red in polling-mode

    }else {
        hal_acc_spi_read_write_dma();
        CT_LCD->BG.RED = (uint16_t)0x0000;
        CT_LCD->BG.BLUE = (uint16_t)0xFFFF;     //Display blue in DMA-Mode
    }

    x = (int16_t)((rx_buffer[2] << 8) + rx_buffer[1]);
    y = (int16_t)((rx_buffer[4] << 8) + rx_buffer[3]);
    z = (int16_t)((rx_buffer[6] << 8) + rx_buffer[5]);

    /* calculate Acceleration in mg */
    acceleration[0] = (x * max_G_force/max_integer_val) + X_OFFSET;
    acceleration[1] = (y * max_G_force/max_integer_val) + Y_OFFSET;
    acceleration[2] = (z * max_G_force/max_integer_val) + Z_OFFSET;

    return;
}

/*
 * See header file
 */
uint8_t accelerometer_read_acc_id(void)
{
    uint8_t tx_data[4];
    uint8_t rx_data[4];
    
    tx_buffer[0] = 0x8F;    //Sensor Register adress of "WHO_AM_I" in read mode
    hal_acc_spi_read_write(2, tx_data, rx_data);


    return rx_data[1];
}

/* internal functions definitions ------------------------------------------ */

/**
 *  \brief  Writes LSM6DSL register
 */
static void write_reg(uint8_t address, uint8_t value)
{
    uint8_t tx_data[2];
    uint8_t rx_data[2]; // No further usage - only needed for function calling
    
    tx_data[0] = address;
    tx_data[1] = value;
    hal_acc_spi_read_write(2, tx_data, rx_data);
}

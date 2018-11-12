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
 * -- Description : This modul handles the commands of the Accelerometer
                    of the Type LSM6DSL. The access to the peripherals is
                    executed trough the file hal_acc.h
 * -- $Revision: 1623 $
 * -- $Id: accelerometer.c 1623 2018-10-17 09:00:42Z angv $
 * --------------------------------------------------------------- */

/* includes ---------------------------------------------------------------- */
#include <stdint.h>
#include "accelerometer.h"
#include "hal_acc.h"
#include "lsm6dsl_reg.h"


/* -- defines----------------------------------------------------------------*/
/* DMA defines */
#define FIFO_DMA_TRANSACTIONS    601


/* Correction factors for accelerometer */
#define         X_OFFSET 0
#define         Y_OFFSET 0
#define         Z_OFFSET 0

#define MAX_INTEGER_VAL  32767
#define MAX_G_FORCE      4000

/* -- local variables declarations -------------------------------------------*/
/* used in accelerometer_init() and */
static uint8_t tx_buffer[FIFO_DMA_TRANSACTIONS]; // Transmition buffer for SPI
static uint8_t rx_buffer[FIFO_DMA_TRANSACTIONS]; // Reception buffer for SPI

/* -- local function declarations --------------------------------------------*/
static void init_fifo(void);

static void fifo_calculations(int16_t *acceleration);

static void write_reg(uint8_t address, uint8_t value);
static void wait_100_us(void);


/**
 * \brief Initialize the Sensor with FIFO-mode
 */
static void init_fifo(void)
{
    write_reg(CTRL3_C_ADD, 0x01);   //Reset Sensor
    wait_100_us();                  //Wait after reset
    write_reg(CTRL10_C_ADD, 0x00);

    /* Configure Sensor Register for FIFO */

    write_reg(FIFO_CTRL1_ADD, 0x2C);    // lower register, nr. of data
    write_reg(FIFO_CTRL2_ADD, 0x01);    // Nr. of data -> 0x12C = 300
    write_reg(FIFO_CTRL3_ADD, 0x01);    // acceleration data in FIFO,
    write_reg(FIFO_CTRL4_ADD, 0x00);
    write_reg(FIFO_CTRL5_ADD, 0x31);    //FIFO mode, ODR 416 Hz
    write_reg(INT1_CTRL_ADD, 0x08);     //Interupt on INT1: FIFO Threshold

}

/**
 * \brief calculate the FIFO values into accelerations
 */
static void fifo_calculations(int16_t *acceleration)
{
    int16_t x = 0;
    int16_t y = 0;
    int16_t z = 0;
    uint16_t i;

    for (i = 0; i < FIFO_DMA_TRANSACTIONS; i = i + 3) {
        /* connect two uint8 to int16 */
        x = (int16_t)((rx_buffer[2 * i + 2] << 8) + rx_buffer[2 * i + 1]);
        y = (int16_t)((rx_buffer[2 * i + 4] << 8) + rx_buffer[2 * i + 3]);
        z = (int16_t)((rx_buffer[2 * i + 6] << 8) + rx_buffer[2 * i + 5]);

        /* calculate Acceleration in mg */
        acceleration[i] = (x * MAX_G_FORCE / MAX_INTEGER_VAL) + X_OFFSET;
        acceleration[i + 1] = (y * MAX_G_FORCE / MAX_INTEGER_VAL) + Y_OFFSET;
        acceleration[i + 2] = (z * MAX_G_FORCE / MAX_INTEGER_VAL) + Z_OFFSET;
    }
}

/*
 * See header file
 */
void accelerometer_init(void)
{
//    hal_acc_init(tx_buffer, rx_buffer, FIFO_DMA_TRANSACTIONS);
    init_fifo();

    /* Standard configuration. Not depending on modes */
    write_reg(ORIENT_CFG_G_ADD, 0x00);
    write_reg(INT2_CTRL_ADD, 0x00);
    write_reg(CTRL2_G_ADD, 0x00);
    write_reg(CTRL3_C_ADD, 0x04);
    write_reg(CTRL4_C_ADD, 0x04);
    write_reg(CTRL5_C_ADD, 0x30);
    write_reg(CTRL6_C_ADD, 0x40);
    write_reg(CTRL7_G_ADD, 0x84);
    write_reg(CTRL8_XL_ADD, 0x00);
    write_reg(CTRL9_XL_ADD, 0x00);
    write_reg(MASTER_CONFIG_ADD, 0x00);

    write_reg(CTRL1_XL_ADD, 0x68); // start measurement
}

/*
 * See header file
 */
uint16_t accelerometer_read_acceleration(int16_t *acceleration)
{
    uint16_t length;

            tx_buffer[0] = 0xBE;
              /* SPI_DMA_LSM_FIFO */
//            hal_acc_spi_read_write_dma();
//            fifo_calculations(acceleration);
  
              /* SPI_POLLING_LSM_FIFO */
            hal_acc_spi_read_write(FIFO_DMA_TRANSACTIONS, tx_buffer, rx_buffer);
            fifo_calculations(acceleration);

            length = (FIFO_DMA_TRANSACTIONS - 1) / 6;

    return length;
}

/**
 *  \brief  Writes LSM6DSL register
 */
static void write_reg(uint8_t address, uint8_t value)
{
    tx_buffer[0] = address;
    tx_buffer[1] = value;

    hal_acc_spi_read_write(2, tx_buffer, rx_buffer);
}

/**
 * \brief Generate a delay
 */
static void wait_100_us(void)
{
    uint16_t counter = 0;
    while (counter < 1600) {
        counter++;
    }
}

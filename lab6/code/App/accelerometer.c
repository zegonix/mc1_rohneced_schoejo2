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
#define POLLING_DMA_TRANSACTIONS 7
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
static void init_continous(void);
static void init_fifo(void);
static void init_significant_motion(void);

static void fifo_calculations(int16_t *acceleration);
static void continous_calculations(int16_t *acceleration);

static void write_reg(uint8_t address, uint8_t value);
static void wait_100_us(void);


/**
 * \brief Initialize the Sensor with continous-mode
 */
static void init_continous(void)
{
    write_reg(CTRL3_C_ADD, 0x01);       //Reset Sensor
    wait_100_us();                      //Wait after reset
    write_reg(INT1_CTRL_ADD, 0x01);     //interrupt on INT1 if data ready
}

/**
 * \brief Initialize the Sensor with FIFO-mode
 */
static void init_fifo(void)
{
    write_reg(CTRL3_C_ADD, 0x01);   //Reset Sensor
    wait_100_us();                  //Wait after reset
    write_reg(CTRL10_C_ADD, 0x00);

    /* Configure Sensor Register for FIFO */
    /// STUDENTS: To be programmed

		write_reg(FIFO_CTRL1_ADD, 0x2C);
		write_reg(FIFO_CTRL2_ADD, 0x01);
		write_reg(FIFO_CTRL3_ADD, 0x01);
		write_reg(FIFO_CTRL4_ADD, 0x00);
		write_reg(FIFO_CTRL5_ADD, 0x31);
		write_reg(INT1_CTRL_ADD, 0x08);

    /// END: To be programmed
}

/**
 * \brief Initialize the Sensor with significant-motion mode
 */
static void init_significant_motion(void)
{
    write_reg(CTRL3_C_ADD, 0x01);   //Reset Sensor
    wait_100_us();                  //Wait after reset

    /* Configure Sensor Register for Significant Motion */

    write_reg(FUNC_CFG_ACC_ADD, 0x80);      //access to bank A
    write_reg(SIG_MOT_THRESHOLD_ADD, 0x01); // significant motion threshold
    write_reg(FUNC_CFG_ACC_ADD, 0x00);

    /// STUDENTS: To be programmed

		write_reg(INT1_CTRL_ADD, 0x40);
		write_reg(CTRL10_C_ADD, 0x05);

    /// END: To be programmed
}

/**
 * \brief calculate the FIFO values into accelerations
 */
static void fifo_calculations(int16_t *acceleration)
{
    /// STUDENTS: To be programmed

    int16_t x = 0;
    int16_t y = 0;
    int16_t z = 0;		
		int16_t n;
	
		for(n=0; n<((FIFO_DMA_TRANSACTIONS-1)/3/2); n++)
		{
			x = (uint16_t)((rx_buffer[2+6*n]<<8) + rx_buffer[1+6*n]);
			y = (uint16_t)((rx_buffer[4+6*n]<<8) + rx_buffer[3+6*n]);
			z = (uint16_t)((rx_buffer[6+6*n]<<8) + rx_buffer[5+6*n]);
		
			acceleration[3*n] = (x * MAX_G_FORCE / MAX_INTEGER_VAL) + X_OFFSET;
			acceleration[3*n+1] = (y * MAX_G_FORCE / MAX_INTEGER_VAL) + Y_OFFSET;
			acceleration[3*n+2] = (z * MAX_G_FORCE / MAX_INTEGER_VAL) + Z_OFFSET;
		}

    /// END: To be programmed
}

/**
 * \brief calculate the continous values into accelerations
 */
static void continous_calculations(int16_t *acceleration)
{
    int16_t x = 0;
    int16_t y = 0;
    int16_t z = 0;

    /* connect two uint8 to int16 */
    x = (int16_t)((rx_buffer[2] << 8) + rx_buffer[1]);
    y = (int16_t)((rx_buffer[4] << 8) + rx_buffer[3]);
    z = (int16_t)((rx_buffer[6] << 8) + rx_buffer[5]);

    /* calculate Acceleration in mg */
    acceleration[0] = (x * MAX_G_FORCE / MAX_INTEGER_VAL) + X_OFFSET;
    acceleration[1] = (y * MAX_G_FORCE / MAX_INTEGER_VAL) + Y_OFFSET;
    acceleration[2] = (z * MAX_G_FORCE / MAX_INTEGER_VAL) + Z_OFFSET;
}

/*
 * See header file
 */
void accelerometer_init(mode transmission_mode)
{
    switch (transmission_mode) {
        case SPI_POLLING: case SPI_DMA:
            hal_acc_init(tx_buffer, rx_buffer, POLLING_DMA_TRANSACTIONS);
            init_continous();
            break;

        case SPI_POLLING_LSM_FIFO: case SPI_DMA_LSM_FIFO:
            hal_acc_init(tx_buffer, rx_buffer, FIFO_DMA_TRANSACTIONS);
            init_fifo();
            break;

        case SPI_POLLING_LSM_SIG_MOT: case SPI_DMA_LSM_SIG_MOT:
            hal_acc_init(tx_buffer, rx_buffer, POLLING_DMA_TRANSACTIONS);
            init_significant_motion();
            break;

        default:
            hal_acc_init(tx_buffer, rx_buffer, POLLING_DMA_TRANSACTIONS);
            init_continous();
            break;
    }

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
uint16_t accelerometer_read_acceleration(int16_t *acceleration,
                                         mode transmission_mode)
{
    uint16_t length;

    switch (transmission_mode) {
        case SPI_POLLING: case SPI_POLLING_LSM_SIG_MOT:
            tx_buffer[0] = 0xA8;
            hal_acc_spi_read_write(POLLING_DMA_TRANSACTIONS,
                                   tx_buffer, rx_buffer);
            continous_calculations(acceleration);

            length = (POLLING_DMA_TRANSACTIONS - 1) / 6;
            break;

        case SPI_DMA: case SPI_DMA_LSM_SIG_MOT:
            tx_buffer[0] = 0xA8;
            hal_acc_spi_read_write_dma();
            continous_calculations(acceleration);

            length = (POLLING_DMA_TRANSACTIONS - 1) / 6;
            break;

        case SPI_POLLING_LSM_FIFO:
            tx_buffer[0] = 0xBE;
            hal_acc_spi_read_write(FIFO_DMA_TRANSACTIONS,
                                   tx_buffer, rx_buffer);
            fifo_calculations(acceleration);

            length = (FIFO_DMA_TRANSACTIONS - 1) / 6;
            break;

        case SPI_DMA_LSM_FIFO:
            tx_buffer[0] = 0xBE;
            hal_acc_spi_read_write_dma();
            fifo_calculations(acceleration);

            length = (FIFO_DMA_TRANSACTIONS - 1) / 6;
            break;

        default: //SPI-Polling
            tx_buffer[0] = 0xA8;
            hal_acc_spi_read_write(POLLING_DMA_TRANSACTIONS,
                                   tx_buffer, rx_buffer);
            continous_calculations(acceleration);

            length = (POLLING_DMA_TRANSACTIONS - 1) / 6;
            break;
    }

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

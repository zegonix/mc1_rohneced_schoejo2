/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur   -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences) -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Module      : SPI Library
 * --
 * -- $Id: hal_spi.c 1577 2018-09-28 09:23:20Z angv $
 * ------------------------------------------------------------------
 */

/* includes ---------------------------------------------------------------- */
#include <reg_stm32f4xx.h>
#include "hal_spi.h"

/* -- defines----------------------------------------------------------------*/
#define BIT_TXE   (uint32_t)0x00000002
#define BIT_RXNE  (uint32_t)0x00000001
#define BIT_BSY   (uint32_t)0x00000080
#define BIT_TCIF3 (uint32_t)0x08000000
#define BIT_TCIF0 (uint32_t)0x00000020

/* -- local function declarations -------------------------------------------*/
static void wait_10_us(void);

/* public function definitions --------------------------------------------- */

/*
 * See header File
 */
void hal_spi_init(reg_spi_t *spi, hal_spi_init_t init)
{
    uint32_t reg = 0;
    uint32_t clear_reg = 0;

    /* process mode */
    clear_reg &= ~(0x1 << 2u);
    reg |= (init.mode << 2u);

    /* process direction */
    clear_reg &= ~(0x3 << 14u);
    reg |= (init.direction << 14u);

    /* process size */
    clear_reg &= ~(0x1 << 11u);
    reg |= (init.data_size << 11u);

    /* process nss mode */
    clear_reg &= ~(0x3 << 8u);
    reg |= (init.nss_mode << 8u);

    /* process prescaler */
    clear_reg &= ~(0x7 << 3u);
    reg |= (init.prescaler << 3u);

    /* process crc mode */
    clear_reg &= ~(0x1 << 13u);
    reg |= (init.hw_crc << 13u);

    spi->CR1 &= clear_reg;
    spi->CR1 |= reg;

    /* process frame type */
    clear_reg = ~(0x1 << 4u);
    reg = (init.frame_type << 4u);

    spi->CR2 &= clear_reg;
    spi->CR2 |= reg;


    /* enable spi */
    spi->CR1 |= (0x1 << 6u);
}



/*
 * See header File
 */
void hal_spi_read_write_bytes(uint16_t nr_of_bytes, uint8_t *tx_buffer,
                              uint8_t *rx_buffer, reg_spi_t *spi)
{
    int i;

    /* for-loop to send and receive SPI-Data */
    for (i = 0; i < nr_of_bytes; i++) {
        SPI1->DR = tx_buffer[i];          // Start transmission by writing to DR
        while (!(spi->SR & BIT_TXE)) {    // wait for transmission to complete
        }
        while (!(spi->SR & BIT_RXNE)) {   // wait for received data to complete
        }
        while ((spi->SR & BIT_BSY)) {     // wait for transfer to complete
        }
        rx_buffer[i] = spi->DR;           // read received data
    }

    wait_10_us();
    return;
}


/**
 *  See header File
 */
void set_spi_read_write_request(reg_spi_t *spi)
{
    spi->CR2 |= 0x0002;
    spi->CR2 |= 0x0001;
}

/**
 *  See header File
 */
void clear_spi_read_write_request(reg_spi_t *spi)
{
    spi->CR2 &= ~(0x0002);
    spi->CR2 &= ~(0x0001);
}


/**
 * \brief Wait for approximately 10us
 */
static void wait_10_us(void)
{
    uint8_t counter = 0;
    while (counter < 160) {
        counter++;
    }
}

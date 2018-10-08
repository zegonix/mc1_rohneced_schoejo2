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
 * -- Description : This module controls all peripherals which are
 *                  needed to communicate with the accelerometer
 *
 * -- $Revision: 1577 $
 * -- $Id: hal_acc.h 1577 2018-09-28 09:23:20Z angv $
 * --------------------------------------------------------------- */

#ifndef _HAL_ACC
#define _HAL_ACC

#include <stdint.h>

/* public function declarations -------------------------------------------- */

/**
 *  \brief  initialize hal of accelerometer
 *					Clock       Enable for SPI,GPIO
 *					GPIO        ACC-INT1 -> PA8 with rising edge interrupt
 *					SPI         init SPI1
 *                                   - CS:    PA4
 *                                   - SCK:   PA5
 *                                   - MISO:  PA6
 *                                   - MOSI:  PA7
 *					DMA         Init DMA to read Acceleration trough SPI1
 *	\param	uint8_t *hal_tx_buffer	pointer to the buffer which
 *          will be sent by DMA
 *	\param	uint8_t *hal_rx_buffer	pointer to the buffer which
 *          will receive the data from DMA
 *	\param	uint16_t length	defines the length of DMA transfer
 */
void hal_acc_init(uint8_t *hal_tx_buffer, uint8_t *hal_rx_buffer,
                  uint16_t length);


/**
 * \brief Read/Write of accelerometer via SPI1
 *
 * PINs:
 * - CS:    PA4
 * - SCK:   PA5
 * - MISO:  PA6
 * - MOSI:  PA7
 *
 * \param uint8_t nr_of_bytes: Total number of Bytes which have to be transfered
 *        Example: nr_of_bytes = send 1 Bytes + receive 3 Bytes = 4 Bytes
 * \param uint8_t *tx_buffer:  Pointer to an array which
 *        contain the data to be sent
 * \param uint8_t *rx_buffer:  Pointer to an array on which
 *        the received bytes will be stored
 *
 */
void hal_acc_spi_read_write(uint16_t nr_of_bytes, uint8_t *tx_buffer,
                            uint8_t *rx_buffer);


/**
 * \breif Read/Write data via SPI1 on Port P5 using with DMA,
 *        the dataflow is defined with the DMA Settings
 *
 * PINs:
 * - CS:    PA4
 * - SCK:   PA5
 * - MISO:  PA6
 * - MOSI:  PA7
 */
void hal_acc_spi_read_write_dma(void);


/* end header ----------------------------------------------------- */
#endif

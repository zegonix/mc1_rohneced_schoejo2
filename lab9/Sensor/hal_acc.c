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
 * -- Description : This modul controls all peripherals which are
 *                  needed to communicate with the accelerometer
 *
 * -- $Revision: 1623 $
 * -- $Id: hal_acc.c 1623 2018-10-17 09:00:42Z angv $
 * ------------------------------------------------------------------------- */

/* includes ---------------------------------------------------------------- */
#include <stdint.h>
#include "hal_acc.h"
#include "spi.h"


/* -- defines----------------------------------------------------------------*/

/* Defines  for initialization */
#define EXTI_PINSOURCE8 ((uint8_t)0x08)
#define EXTI9_5_IRQN    23      //External Line[9:5] Interrupts

/* DMA Defines */
#define BIT_TCIF3 (uint32_t)0x08000000
#define BIT_TCIF0 (uint32_t)0x00000020
#define BIT_TCIF5 (uint32_t)0x00000800
#define BIT_TCIF2 (uint32_t)0x00200000
#define BIT_BSY   (uint32_t)0x00000080
#define TIMEOUT_VAL_SPI     50u



/* -- local variables declarations -----------------------------------------*/

/* -- local function declarations -------------------------------------------*/
static void set_ss_pin_low(void);
static void set_ss_pin_high(void);

/* public function definitions --------------------------------------------- */

/**
 *  See header File
 */
HAL_StatusTypeDef hal_acc_spi_read_write(uint16_t nr_of_bytes, uint8_t *tx_buffer,
                            uint8_t *rx_buffer)
{
    HAL_StatusTypeDef status;
  
    set_ss_pin_low();
    //hal_spi_read_write_bytes(nr_of_bytes, tx_buffer, rx_buffer, SPI1);
    status = HAL_SPI_TransmitReceive(&hspi1, tx_buffer, rx_buffer, nr_of_bytes, TIMEOUT_VAL_SPI);
    set_ss_pin_high();
  
    return status;
}

/* internal function definitions ------------------------------------------- */

/**
 * \brief Set Slave-Select Pin (Port A, Pin 5) low
 */
static void set_ss_pin_low(void)
{
    //GPIOA->BSRR[1] |= 0x0010;              // Set Port A, Pin 5 low
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}

/**
 * \brief Set Slave-Select Pin (Port A, Pin 5) high
 */
static void set_ss_pin_high(void)
{
    //GPIOA->BSRR[0] |= 0x0010;             // Set Port A, Pin 5 high
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}


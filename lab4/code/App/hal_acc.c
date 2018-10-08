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
 * -- $Revision: 1581 $
 * -- $Id: hal_acc.c 1581 2018-09-28 13:56:48Z angv $
 * ------------------------------------------------------------------------- */

/* includes ---------------------------------------------------------------- */
#include <stdint.h>
#include "hal_gpio.h"
#include "hal_rcc.h"
#include "hal_dma.h"
#include "hal_spi.h"
#include "hal_acc.h"

/* -- defines----------------------------------------------------------------*/

/* Defines  for initialization */
#define EXTI_PINSOURCE9 ((uint8_t)0x09)
#define EXTI9_5_IRQN    23      // !< External Line[9:5] Interrupts

/* DMA Defines */
#define BIT_TCIF3 (uint32_t)0x08000000
#define BIT_TCIF0 (uint32_t)0x00000020
#define BIT_TCIF5 (uint32_t)0x00000800
#define BIT_TCIF2 (uint32_t)0x00200000
#define BIT_BSY   (uint32_t)0x00000080


/* -- static variables declarations -----------------------------------------*/
/* Set in DMA-Interrupt if dma-transfer is completed. 
 * Used to break a loop to demonstrate the independence 
 * of a dma-stream from the uC*/
static uint8_t transfer_complete = 0; 


/* -- local function declarations -------------------------------------------*/
static void init_spi(void);
static void init_gpio(void);
static void init_rcc(void);
static void init_dma(uint8_t *hal_tx_buffer, uint8_t *hal_rx_buffer,
                     uint16_t length);
static void set_ss_pin_low(void);
static void set_ss_pin_high(void);

static void set_all_led_high(void);

static void wait_5_us(void);

/* public function definitions --------------------------------------------- */

/**
 *  See header File
 */
void hal_acc_init(uint8_t *hal_tx_buffer, uint8_t *hal_rx_buffer,
                  uint16_t length)
{
    init_rcc();
    init_gpio();
    init_spi();
    init_dma(hal_tx_buffer, hal_rx_buffer, length);
    set_all_led_high();
}

/**
 *  See header File
 */
void hal_acc_spi_read_write(uint16_t nr_of_bytes, uint8_t *tx_buffer,
                            uint8_t *rx_buffer)
{
    set_ss_pin_low();
    hal_spi_read_write_bytes(nr_of_bytes, tx_buffer, rx_buffer, SPI1);
    set_ss_pin_high();
}


/**
 *  See header File
 */
void hal_acc_spi_read_write_dma(void)
{
    uint8_t status = 0; //Indicator for state of GPIO Output 3

    set_ss_pin_low();

    /* enable the DMA streams */
    hal_dma_start(DMA2, HAL_DMA_STREAM_3);
    hal_dma_start(DMA2, HAL_DMA_STREAM_0);

    /* send SPI request to DMA */
    set_spi_read_write_request(SPI1);

    /* DMA handles now communicatin. The uC can do something different as long
    *  as the transfer is in progress. */
    while (transfer_complete == 0) {
        if (status == 0) {
            GPIOA->BSRR[0] |= (0x1 << 3);   // Set GPIO Pin 3 to HIGH
            status = 1;                     // GPIO Pin 3 status high
            wait_5_us();
        }
        else  {
            GPIOA->BSRR[1] |= (0x1 << 3);   // Set GPIO Pin 3 to LOW
            status = 0;                     // GPIO Pin 3 status low
            wait_5_us();
        }
    }
    transfer_complete = 0;

    /* wait until SPI communication done*/
    while ((SPI1->SR & BIT_BSY)) {
    }
    ;

    /* disable the DMA streams */
    hal_dma_stop(DMA2, HAL_DMA_STREAM_3);
    hal_dma_stop(DMA2, HAL_DMA_STREAM_0);

    set_ss_pin_high();
}

/* internal function definitions ------------------------------------------- */

/**
 *  \brief  initialize SPI1 for accelerometer
 *          - CS:    PA4
 *          - SCK:   PA5
 *          - MISO:  PA6
 *          - MOSI:  PA7
 */
static void init_spi(void)
{
    hal_spi_init_t init;

    init.data_size = HAL_SPI_SIZE_8B;
    init.direction = HAL_SPI_DIR_BOTH;
    init.frame_type = HAL_SPI_FRAME_MOTOROLLA;
    init.hw_crc = FALSE;
    init.mode = HAL_SPI_MODE_MASTER;
    init.nss_mode = HAL_SPI_NSS_SW;
    init.prescaler = HAL_SPI_PRE_256;

    hal_spi_init(SPI1, init);
}

/**
 *  \brief  initialize GPIO for LSM6DSL
 *			Define PA4-PA7 for SPI
 *			ACC-INT1 -> PA8 input with rising edge interrupt
 */
static void init_gpio(void)
{
    hal_gpio_input_t hal_gpio_input;
    uint32_t exti_pinsource = 0;
    uint32_t irq = 0;
    uint32_t temp = 0x00;
    uint32_t iocurrent = 0x100;

    /* SPI Pins */
    GPIOA->OSPEEDR &= 0xFFFF0000;       // clear P4 to P7
    GPIOA->OSPEEDR |= 0x0000FFFF;       // set P4 to P7 to 100 MHz

    GPIOA->MODER &= 0xFFFF0000;       // clear mode on P5 to P7, P5 to P7, P4 output mode
    GPIOA->MODER |= 0x0000A955;       // Set alternate function mode on, P5 to P7, P4 output mode

    GPIOA->AFR[0] &= 0x0000FFFF;      // clear alternate function
    GPIOA->AFR[0] |= 0x55550000;      // Set SPI1 alternate function

    GPIOB->PUPDR &= 0xFFFFFFC0;
    GPIOB->PUPDR |= 0x0000002A;     //link P6, P7 to internal pullup register

    hal_gpio_input.pins = 0x0100;     //Set Pin 8
    hal_gpio_input.pupd = HAL_GPIO_PUPD_NOPULL;

    hal_gpio_init_input(GPIOA, hal_gpio_input);

    set_ss_pin_high();


    /* define PA9 interrupt on rising edge */

    /* Clear EXTI line configuration */
    temp = EXTI->IMR;
    temp &= ~(iocurrent);
    temp |= iocurrent;
    EXTI->IMR = temp;

    /* Enable Rising  edge configuration */
    temp = EXTI->RTSR;
    temp &= ~(iocurrent);
    temp |= iocurrent;
    EXTI->RTSR = temp;

    /* Disable Falling  edge configuration */
    temp = EXTI->FTSR;
    temp &= ~(iocurrent);
    EXTI->FTSR = temp;

    exti_pinsource = EXTI_PINSOURCE9;
    irq = EXTI9_5_IRQN;

    /* Enable EXTI */
    temp = ~(((uint32_t)0x0F) << (0x04 * (exti_pinsource & 0x03)));
    SYSCFG->EXTICR[exti_pinsource >> 0x02] &= temp;

    /* Enable NVIC interrupt */
    NVIC->ISER[irq >> 0x05] = (uint32_t)0x01 << (irq & (uint8_t)0x1F);
}

/**
 *  \brief  initialize Clock for LSM6DSL
 */
static void init_rcc(void)
{
    RCC->APBENR[1] |= 0x00001000;     // enable SPI clock
    RCC->AHBENR[0] |= 0x00000001;     // start clock on GPIO A

    /* enable peripheral clock for DMA2 Controller */
    DMA2_ENABLE();
}

/**
 *  \brief  initialize DMA for LSM6DSL
 *					1. DMA Transfer:
 *					Write read-Command from memory to the SPI1->DR register
 *					2. DMA Transfer:
 *					Read Acceleration data from SPI1-DR
 *  \param  *hal_tx_buffer: Pointer to buffer which contains the data to send
 *  \param  *hal_rx_buffer: Pointer to buffer on which the received data will
 *          be stored
 *  \param  length: Length of the DMA transfers
 */
static void init_dma(uint8_t *hal_tx_buffer, uint8_t *hal_rx_buffer,
                     uint16_t length)
{
    uint32_t irq = 56;
    hal_dma_init_t dma_settings;
    /// STUDENTS: To be programmed




    /// END: To be programmed

    /* Enable NVIC interrupt */
    NVIC->ISER[1] = (uint32_t)0x01 << (irq - 32);
}

/**
 * \brief Set Slave-Select Pin (Port A, Pin 5) low
 */
static void set_ss_pin_low(void)
{
    GPIOA->BSRR[1] |= 0x0010;              // Set Port A, Pin 5 low
}

/**
 * \brief Set Slave-Select Pin (Port A, Pin 5) high
 */
static void set_ss_pin_high(void)
{
    GPIOA->BSRR[0] |= 0x0010;             // Set Port A, Pin 5 high
}

/**
 * \brief Turn off all LED on accelerometer-Board (P0, P1, P2)
 */
static void set_all_led_high(void)
{
    GPIOA->BSRR[0] |= (0x1 << 0);   // Set Port B, Pin 0 high (BLUE, active low)
    GPIOA->BSRR[0] |= (0x1 << 1);   // Set Port B, Pin 1 high (RED, active low)
    GPIOA->BSRR[0] |= (0x1 << 2);   // Set Port B, Pin 2 high (GREEN, active low)
}

/**
 * \brief Wait for approximately 10us
 */
static void wait_5_us(void)
{
    uint8_t counter = 0;
    while (counter < 80) {
        counter++;
    }
}

/**
 * \brief   Handles the DMA interrupts
 *          the interrupt indicates the end of the DMA transfer
 */
extern void DMA2_Stream0_IRQHandler(void)
{
    /*check if transfer complete */
    if ((DMA2->ISR[0] & BIT_TCIF0) != 0) {
        /* clear spi request to stop transfer*/
        clear_spi_read_write_request(SPI1);

        /* clear all pending interupts */
        DMA2->IFCR[0] = 0xFFFFFFFF;
        transfer_complete = 1;
    }
}

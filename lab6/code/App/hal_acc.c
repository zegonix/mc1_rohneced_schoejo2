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
#include <reg_stm32f4xx.h>
#include "reg_ctboard.h"
#include "hal_gpio.h"
#include "hal_rcc.h"
#include "hal_dma.h"
#include "hal_spi.h"
#include "hal_acc.h"

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



/* -- local variables declarations -----------------------------------------*/
static volatile uint8_t transfer_complete = 0;

/* -- local function declarations -------------------------------------------*/
static void init_spi(void);
static void init_gpio(void);
static void init_rcc(void);
static void init_dma(uint8_t *hal_tx_buffer, uint8_t *hal_rx_buffer,
                     uint16_t length);
static void set_ss_pin_low(void);
static void set_ss_pin_high(void);

static void set_all_led_high(void);

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
    set_ss_pin_low();

    /* enable the DMA streams */
    hal_dma_start(DMA2, HAL_DMA_STREAM_3);
    hal_dma_start(DMA2, HAL_DMA_STREAM_0);

    /* send SPI request to DMA */
    set_spi_read_write_request(SPI1);

    /* enter sleep mode during transfer*/
    while (transfer_complete == 0) {
        SCB->SCR &= ~(0x1 << 2u);   // Clear SLEEPDEEP bit
        __asm volatile ("wfi");
    }
    transfer_complete = 0;

    /* wait until SPI communication done*/
    while ((SPI1->SR & BIT_BSY)) {
    }

    /* disable the DMA streams */
    hal_dma_stop(DMA2, HAL_DMA_STREAM_3);
    hal_dma_stop(DMA2, HAL_DMA_STREAM_0);

    set_ss_pin_high();
}

/* internal function definitions ------------------------------------------- */

/**
 *  \brief  initialize SPI1 for accelerometer
 *                                    - CS:    PA4
 *                                    - SCK:   PA5
 *                                    - MISO:  PA6
 *                                    - MOSI:  PA7
 */
static void init_spi(void)
{
    hal_spi_init_t init;

    init.mode = HAL_SPI_MODE_MASTER;
    init.direction = HAL_SPI_DIR_BOTH;
    init.data_size = HAL_SPI_SIZE_8B;
    init.nss_mode = HAL_SPI_NSS_SW;
    init.prescaler = HAL_SPI_PRE_64;
    init.frame_type = HAL_SPI_FRAME_MOTOROLLA;
    init.hw_crc = FALSE;

    hal_spi_init(SPI1, init);
}

/**
 *  \brief  initialize GPIO for LSM6DSL
 *            Define PA4-PA7 for SPI
 *            ACC-INT1 -> PA8 input with rising edge interrupt
 */
static void init_gpio(void)
{
    hal_gpio_input_t hal_gpio_input;
    hal_gpio_output_t hal_gpio_output;
    uint32_t exti_pinsource = 0;
    uint32_t irq = 0;
    uint32_t temp = 0x00;
    uint32_t iocurrent = 0x100;

    /* SPI Pins */
    GPIOA->OSPEEDR &= 0xFFFF00C0;     // clear P4 to P7, and P0 to P3
    GPIOA->OSPEEDR |= 0x0000FF3F;     // set P4 to P7 and P0 to P2 to 100 MHz

    GPIOA->MODER &= 0xFFFF00C0;       // clear mode on P5 to P7 and P0 to P2
    // P5 to P7, P4 output mode
    GPIOA->MODER |= 0x0000A98A;       // Set alternate function mode on
    // P5 to P7, P4 output mode

    GPIOA->AFR[0] &= 0x0000FFFF;      // clear alternate function
    GPIOA->AFR[0] |= 0x55550000;      // Set SPI1 alternate function

    hal_gpio_input.pins = 0x0100;     //Set Pin 8
    hal_gpio_input.pupd = HAL_GPIO_PUPD_NOPULL;
    hal_gpio_init_input(GPIOA, hal_gpio_input);
    set_ss_pin_high();


    /* LED outputs */
    hal_gpio_output.out_speed = HAL_GPIO_OUT_SPEED_2MHZ;
    hal_gpio_output.out_type = HAL_GPIO_OUT_TYPE_PP;
    hal_gpio_output.pins = 0x0007;
    hal_gpio_output.pupd = HAL_GPIO_PUPD_NOPULL;
    hal_gpio_init_output(GPIOA, hal_gpio_output);
    GPIOA->BSRR[0] |= 0x0007;

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

    exti_pinsource = EXTI_PINSOURCE8;
    irq = EXTI9_5_IRQN;

    /* Enable EXTI */
    temp = ~(((uint32_t)0x0F) << (0x04 * (exti_pinsource & 0x03)));
    //SYSCFG->EXTICR[exti_pinsource >> 0x02] &= temp;

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
 *                    1. DMA Transfer:
 *                    Write read-Command from memory to the SPI1->DR register
 *                    2. DMA Transfer:
 *                    Read Acceleration data from SPI1-DR
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

    /* Init DMA2 channel 3 for SPI_TX */
    dma_settings.channel = HAL_DMA_CHANNEL_3; // SPI1_TX channel
    dma_settings.direction = HAL_DMA_MEM_TO_PER;
    dma_settings.size = HAL_DMA_SIZE_8B;
    dma_settings.nr_transactions = length;
    dma_settings.continous = DISABLE;
    dma_settings.source = (uint32_t)(hal_tx_buffer);
    dma_settings.destination = (uint32_t)&(SPI1->DR);
    dma_settings.increment = ENABLE;

    hal_dma_init_base(DMA2, HAL_DMA_STREAM_3, dma_settings);

    /* Init DMA2 channel 3 for SPI_RX */
    dma_settings.channel = HAL_DMA_CHANNEL_3;   // SPI1_RX channel
    dma_settings.direction = HAL_DMA_PER_TO_MEM;
    dma_settings.size = HAL_DMA_SIZE_8B;
    dma_settings.nr_transactions = length;
    dma_settings.continous = DISABLE;
    dma_settings.source = (uint32_t)&(SPI1->DR);
    dma_settings.destination = (uint32_t)(hal_rx_buffer);
    dma_settings.increment = ENABLE;

    hal_dma_init_base(DMA2, HAL_DMA_STREAM_0, dma_settings);

    /* Enable NVIC interrupt */
    NVIC->ISER[1] = (uint32_t)0x01 << (irq - 32);
}


/*
 * See header file
 */
void power_set_clock()
{
    hal_rcc_pll_init_t pll_init;
    hal_rcc_clk_init_t clk_init;

    /* Fill init structs based on speed */
    /* PLL config 15MHz */
    pll_init.source = HAL_RCC_OSC_HSE; // Use HSE (8 MHz) as PLL source
    pll_init.m_divider = 4u;   // VCOin: HSE / m_divider -> 2 MHz
    pll_init.n_factor = 120u;  // VCOout: VCOin * n_factor -> 240 MHz
    pll_init.p_divider = 16u;   // PLL: VCOout / p_divider -> 15 MHz
    pll_init.q_divider = 5u;   // 48 MHz out: VCOout / q_div -> 48 MHz
    /* Clock config */
    clk_init.osc = HAL_RCC_OSC_PLL; // Use PLL as SYSCLK source
    clk_init.hpre = HAL_RCC_HPRE_2; // HCLK: SYSCLK / hpre -> 7.5 MHz
    clk_init.ppre1 = HAL_RCC_PPRE_2; // PCLK1: HCLK / ppre1 -> 3.75 MHz
    clk_init.ppre2 = HAL_RCC_PPRE_2; // PCLK2: HCLK / ppre2 -> 3.75 MHz

    /* Reset */
    RCC->CR |= 0x00000001;      // Set HSION bit first -> keep cpu running
    RCC->CFGR &= 0x1111111c;    // Set system clock to HSI
    RCC->CR &= 0xeaf6ffff;      // Reset HSEON, CSSON, PLLON, PLLI2S,
                                //   PLLSAI bits (STM32F42xx/43xx)
    RCC->CR &= 0xfffbffff;      // Reset HSEBYP bit
    RCC->CFGR = 0u;             // Reset RCC->CFGR to default values
    RCC->PLLCFGR = 0x24003010;  // Reset RCC->PLLxCFGR to default values

    /* Enable HSE oscillator and proceed if ok */
    if (hal_rcc_set_osc(HAL_RCC_OSC_HSE, ENABLE)) {
        /* Select regulator voltage output Scale 1 mode */
        PWR->CR |= (0x3 << 14u);

        /* Configure PLL */
        hal_rcc_setup_pll(HAL_RCC_OSC_PLL, pll_init);

        /* Enable PLL */
        hal_rcc_set_osc(HAL_RCC_OSC_PLL, ENABLE);

        /* Setup system clock */
        hal_rcc_setup_clock(clk_init);
    } else {
        /* If HSE fails to start-up, the application will have wrong clock con-
           figuration. User can add here some code to deal with this error */
    }

    /* Configure Flash prefetch, Instruction cache, Data cache
     * and wait state */
    FLASH->ACR = 0x00000705;
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

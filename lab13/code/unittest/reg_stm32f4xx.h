/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Registries for STM32F4xx.
 *
 *  \file   reg_stm32f4xx.h
 *  $Id: reg_stm32f4xx.h 2345 2015-07-29 08:35:08Z feur $
 * ------------------------------------------------------------------------- */
#ifndef _REG_STM32F4xx_H
#define _REG_STM32F4xx_H

#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
#include "arm_peripherals.h"

#define APB1_OFFSET      0x00000
#define APB2_OFFSET      0x10000
#define AHB1_OFFSET      0x20000
#define AHB2_OFFSET      0x00000
#define INTERNAL_OFFSET  0x00000
//#define FMC_OFFSET       0x00000

#define APB1_BASE        apb1     // address of starting position
#define APB2_BASE        apb2     // address of starting position
#define AHB1_BASE        ahb1     // address of starting position
#define AHB2_BASE        ahb2     // address of starting position
#define INTERNAL_BASE    intperiph    // address of starting position
#define FMC_BASE         fmc    // address of starting position

#define APB1_SECTION     (APB1_BASE - APB1_OFFSET)
#define APB2_SECTION     (APB2_BASE - APB2_OFFSET)
#define AHB1_SECTION     (AHB1_BASE - AHB1_OFFSET)
#define AHB2_SECTION     (AHB2_BASE - AHB2_OFFSET)
#define INTERNAL_SECTION (INTERNAL_BASE - INTERNAL_OFFSET)
//#define FMC_SECTION      (FMC_BASE - FMC_OFFSET)

/* Set and clear bit in register. */
#define REG_SET(REG, BIT)       ( (REG) |= (BIT) )
#define REG_CLR(REG, BIT)       ( (REG) &= ~(BIT) )

/*  FLASH - flash interface register
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_flash_t
 *  \brief  Representation of FLASH register.
 *
 *  Described in reference manual p.73ff.
 */
typedef struct {
    volatile uint32_t ACR;      /**< . */
    volatile uint32_t KEYR;     /**< . */
    volatile uint32_t OPTKEYR;  /**< . */
    volatile uint32_t SR;       /**< . */
    volatile uint32_t CR;       /**< . */
    volatile uint32_t OPTCR[2]; /**< . */
} reg_flash_t;


#define FLASH ((reg_flash_t *)(AHB1_SECTION + 0x23c00)


/*  PWR - power controller
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_pwr_t
 *  \brief  Representation of PWR register.
 *
 *  Described in reference manual p.115ff.
 */
typedef struct {
    volatile uint32_t CR;       /**< . */
    volatile uint32_t CSR;      /**< . */
} reg_pwr_t;


#define PWR ((reg_pwr_t *)(APB1_SECTION + 0x7000))


/*  RCC - register clock control
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_rcc_t
 *  \brief  Representation of RCC register.
 *
 *  Described in reference manual p.147ff.
 */
typedef struct {
    volatile uint32_t CR;           /**< Clock control register. */
    volatile uint32_t PLLCFGR;      /**< PLL configuration register. */
    volatile uint32_t CFGR;         /**< Clock configuration register. */
    volatile uint32_t CIR;          /**< Clock interrupt register. */
    volatile uint32_t AHBRSTR[3];   /**< AHB reset register. */
    uint32_t RESERVED;
    volatile uint32_t APBRSTR[2];   /**< APB reset register. */
    uint32_t RESERVED1[2];
    volatile uint32_t AHBENR[3];    /**< AHB clock enable register. */
    uint32_t RESERVED2;
    volatile uint32_t APBENR[2];    /**< APB clock enable register. */
    uint32_t RESERVED3[2];
    volatile uint32_t AHBLPENR[3];  /**< AHB clock enable in lp register. */
    uint32_t RESERVED4;
    volatile uint32_t APBLPENR[2];  /**< APB clock enable in lp register. */
    uint32_t RESERVED5[2];
    volatile uint32_t BDCR;         /**< Backup domain control register. */
    volatile uint32_t CSR;          /**< Clock controll and status register. */
    uint32_t RESERVED6[2];
    volatile uint32_t SSCGR;        /**< Spreadspectrum clock gen. register. */
    volatile uint32_t PLLI2SCFGR;   /**< PLLI2S configuration register. */
    volatile uint32_t PLLSAICFGR;   /**< PLLSAI configuration register. */
    volatile uint32_t DCKCFGR;      /**< Dedicated clock conf. register. */
} reg_rcc_t;


#define RCC ((reg_rcc_t *)(AHB1_SECTION + 0x23800))


/*  GPIO - general purpose input / output
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_gpio_t
 *  \brief  Representation of GPIO register.
 *
 *  Described in reference manual p.265ff.
 */
typedef struct {
    volatile uint32_t MODER;    /**< Port mode register. */
    volatile uint32_t OTYPER;   /**< Output type register. */
    volatile uint32_t OSPEEDR;  /**< Output speed register. */
    volatile uint32_t PUPDR;    /**< Port pull-up/pull-down register. */
    volatile uint32_t IDR;      /**< Input data register. */
    volatile uint32_t ODR;      /**< output data register. */
    volatile uint16_t BSRR[2];  /**< [0]: Bit set register,
                                     [1]: Bit reset register. */
    volatile uint32_t LCKR;     /**< Port lock register. */
    volatile uint32_t AFR[2];   /**< [0]: AF register pin 0..7,
                                     [1]: AF register pin 8..15. */
} reg_gpio_t;


#define GPIOA ((reg_gpio_t *)(AHB1_SECTION + 0x20000))
#define GPIOB ((reg_gpio_t *)(AHB1_SECTION + 0x20400))
#define GPIOC ((reg_gpio_t *)(AHB1_SECTION + 0x20800))
#define GPIOD ((reg_gpio_t *)(AHB1_SECTION + 0x20c00))
#define GPIOE ((reg_gpio_t *)(AHB1_SECTION + 0x21000))
#define GPIOF ((reg_gpio_t *)(AHB1_SECTION + 0x21400))
#define GPIOG ((reg_gpio_t *)(AHB1_SECTION + 0x21800))
#define GPIOH ((reg_gpio_t *)(AHB1_SECTION + 0x21c00))
#define GPIOI ((reg_gpio_t *)(AHB1_SECTION + 0x22000))
#define GPIOJ ((reg_gpio_t *)(AHB1_SECTION + 0x22400))
#define GPIOK ((reg_gpio_t *)(AHB1_SECTION + 0x22800))

/* Helper macros */
#define GPIOA_RCC_PATTERN       ( 0x1 << 0u )
#define GPIOA_ENABLE()          REG_SET(RCC->AHBENR[0],   GPIOA_RCC_PATTERN)
#define GPIOA_DISABLE()         REG_CLR(RCC->AHBENR[0],   GPIOA_RCC_PATTERN)

#define GPIOB_RCC_PATTERN       ( 0x1 << 1u )
#define GPIOB_ENABLE()          REG_SET(RCC->AHBENR[0],   GPIOB_RCC_PATTERN)
#define GPIOB_DISABLE()         REG_CLR(RCC->AHBENR[0],   GPIOB_RCC_PATTERN)

/*  SYSCFG - system configuration controller
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_syscfg_t
 *  \brief  Representation of SYSCFG register.
 *
 *  Described in reference manual p.286ff.
 */
typedef struct {
    volatile uint32_t MEMRMP;       /**< Memory remap register. */
    volatile uint32_t PMC;          /**< Peripheral mode register. */
    volatile uint32_t EXTICR[4];    /**< External interrupt conf. register. */
    uint32_t RESERVED[2];
    volatile uint32_t CMPCR;        /**< Compensation cell control register. */
} reg_syscfg_t;


#define SYSCFG ((reg_syscfg_t *)(APB2_SECTION + 0x13800))


/*  DMA - direct memory access
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_dma_stream_t
 *  \brief  Representation of DMA stream register.
 */
typedef struct {
    volatile uint32_t CR;       /**< Stream configuration register. */
    volatile uint32_t NDTR;     /**< Number of data register. */
    volatile uint32_t PAR;      /**< Peripheral address register. */
    volatile uint32_t M0AR;     /**< Memory 0 address register. */
    volatile uint32_t M1AR;     /**< Memory 1 address register. */
    volatile uint32_t FCR;      /**< FIFO control register. */
} reg_dma_stream_t;


/**
 *  \struct reg_dma_t
 *  \brief  Representation of DMA register.
 *
 *  Described in reference manual p.299ff.
 */
typedef struct {
    volatile uint32_t ISR[2];       /**< Interrupt status register. */
    volatile uint32_t IFCR[2];      /**< Interrupt flag clear register. */
    reg_dma_stream_t STREAM[8];     /**< Struct of stream registers. */
} reg_dma_t;


#define DMA1 ((reg_dma_t *)(AHB1_SECTION + 0x26000))
#define DMA2 ((reg_dma_t *)(AHB1_SECTION + 0x26400))


/*  DMA2D - direct memory access
 * ------------------------------------------------------------------------- */


/*  NVIC - nested vector interrupt controller
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_nvic_t
 *  \brief  Representation of NVIC register.
 *
 *  Described in programming manual p.193ff.
 */
typedef struct {
    volatile uint32_t ISER[3];  /**< Interrupt set-enable register. */
    uint32_t RESERVED1[29];
    volatile uint32_t ICER[3];  /**< Interrupt clear-enable register. */
    uint32_t RESERVED2[29];
    volatile uint32_t ISPR[3];  /**< Interrupt set-pending register. */
    uint32_t RESERVED3[29];
    volatile uint32_t ICPR[3];  /**< Interrupt clear-pending register. */
    uint32_t RESERVED4[29];
    volatile uint32_t IABR[3];  /**< Interrupt active bit register. */
    uint32_t RESERVED5[61];
    volatile uint8_t IP[81];    /**< Interrupt priority. */
    uint8_t RESERVED6[3];
    uint32_t RESERVED7[684];
    volatile uint32_t STIR;     /**< Software trigger interrupt register. */
} reg_nvic_t;


#define NVIC ((reg_nvic_t *)(INTERNAL_SECTION + 0xe100))


/*  EXTI - external interrupt
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_exti_t
 *  \brief  Representation of EXTI register.
 *
 *  Described in reference manual p.368ff.
 */
typedef struct {
    volatile uint32_t IMR;      /**< Interrupt mask register. */
    volatile uint32_t EMR;      /**< Event mask register. */
    volatile uint32_t RTSR;     /**< Rising trigger selection register. */
    volatile uint32_t FTSR;     /**< Falling trigger selection register. */
    volatile uint32_t SWIER;    /**< Software interrupt event register. */
    volatile uint32_t PR;       /**< Pending register. */
} reg_exti_t;


#define EXTI ((reg_exti_t *)APB2_SECTION + 0x13c00)


/*  ADC - analog digital converter
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_adc_t
 *  \brief  Representation of ADC register.
 *
 *  Described in reference manual p.385ff.
 */
typedef struct {
    volatile uint32_t SR;       /**< Status register. */
    volatile uint32_t CR1;      /**< Control register 1. */
    volatile uint32_t CR2;      /**< Control register 2. */
    volatile uint32_t SMPR[2];  /**< Sample time register. */
    volatile uint32_t JOFR[4];  /**< Injected channel data offset register. */
    volatile uint32_t TR[2];    /**< [0]: Higher treshold register,
                                     [1]: Lower treshold register. */
    volatile uint32_t SQR[3];   /**< Regular sequence register. */
    volatile uint32_t JSQR;     /**< Injected sequence register. */
    volatile uint32_t JDR[4];   /**< Injected data register. */
    volatile uint32_t DR;       /**< Regular data register. */
} reg_adc_t;


#define ADC1 ((reg_adc_t *)APB2_SECTION + 0x12000)
#define ADC2 ((reg_adc_t *)APB2_SECTION + 0x12100)
#define ADC3 ((reg_adc_t *)APB2_SECTION + 0x12200)


/**
 *  \struct reg_adccom_t
 *  \brief  Representation of ADC common register.
 *
 *  Described in reference manual p.385ff.
 */
typedef struct {
    volatile uint32_t CSR;      /**< Common status register. */
    volatile uint32_t CCR;      /**< Common control register. */
    volatile uint32_t CDR;      /**< Common regular data register. */
} reg_adccom_t;


#define ADCCOM ((reg_adccom_t *)APB2_SECTION + 0x12300)


/*  DAC - digital analog converter
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_dac_t
 *  \brief  Representation of DAC register.
 *
 *  Described in reference manual p.430ff.
 */
typedef struct {
    volatile uint32_t CR;       /**< Control register. */
    volatile uint32_t SWTRIGR;  /**< Software trigger register. */
    volatile uint32_t DHR12R1;  /**< Ch1 12-bit right-aligned data register. */
    volatile uint32_t DHR12L1;  /**< Ch1 12-bit left-aligned data register. */
    volatile uint32_t DHR8R1;   /**< Ch1 8-bit right-aligned data register. */
    volatile uint32_t DHR12R2;  /**< Ch2 12-bit right-aligned data register. */
    volatile uint32_t DHR12L2;  /**< Ch2 12-bit left-aligned data register. */
    volatile uint32_t DHR8R2;   /**< Ch2 8-bit right-aligned data register. */
    volatile uint32_t DHR12RD;  /**< Dual 12-bit right-align. data register. */
    volatile uint32_t DHR12LD;  /**< Dual 12-bit left-aligned data register. */
    volatile uint32_t DHR8RD;   /**< Dual 8-bit right-aligned data register. */
    volatile uint32_t DOR1;     /**< Ch1 data output register. */
    volatile uint32_t DOR2;     /**< Ch2 data output register. */
    volatile uint32_t SR;       /**< Status register. */
} reg_dac_t;


#define DAC ((reg_dac_t *)(APB1_SECTION + 0x7400))


/*  DCMI - digital camery mxxx interface
 * ------------------------------------------------------------------------- */


/*  LTDC - liquid txxx display controller
 * ------------------------------------------------------------------------- */


/*  TIM - timer
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_tim_t
 *  \brief  Representation of TIM register.
 *
 *  Described in reference manual p.576ff.
 */
typedef struct {
    volatile uint32_t CR1;      /**< . */
    volatile uint32_t CR2;      /**< . */
    volatile uint32_t SMCR;     /**< . */
    volatile uint32_t DIER;     /**< . */
    volatile uint32_t SR;       /**< . */
    volatile uint32_t EGR;      /**< . */
    volatile uint32_t CCMR[2];  /**< . */
    volatile uint32_t CCER;     /**< . */
    volatile uint32_t CNT;      /**< . */
    volatile uint32_t PSC;      /**< . */
    volatile uint32_t ARR;      /**< . */
    volatile uint32_t RCR;      /**< . */
    volatile uint32_t CCR[4];   /**< . */
    volatile uint32_t BDTR;     /**< . */
    volatile uint32_t DCR;      /**< . */
    volatile uint32_t DMAR;     /**< . */
    volatile uint32_t OR;       /**< . */
} reg_tim_t;


#define TIM1  ((reg_tim_t *)(APB2_SECTION + 0x10000))
#define TIM2  ((reg_tim_t *)(APB1_SECTION + 0x0000))
#define TIM3  ((reg_tim_t *)(APB1_SECTION + 0x0400))
#define TIM4  ((reg_tim_t *)(APB1_SECTION + 0x0800))
#define TIM5  ((reg_tim_t *)(APB1_SECTION + 0x0c00))
#define TIM6  ((reg_tim_t *)(APB1_SECTION + 0x1000))
#define TIM7  ((reg_tim_t *)(APB1_SECTION + 0x1400))
#define TIM8  ((reg_tim_t *)(APB2_SECTION + 0x10400))
#define TIM9  ((reg_tim_t *)(APB2_SECTION + 0x14000))
#define TIM10 ((reg_tim_t *)(APB2_SECTION + 0x14400))
#define TIM11 ((reg_tim_t *)(APB2_SECTION + 0x14800))
#define TIM12 ((reg_tim_t *)(APB1_SECTION + 0x1800))
#define TIM13 ((reg_tim_t *)(APB1_SECTION + 0x1c00))
#define TIM14 ((reg_tim_t *)(APB1_SECTION + 0x2000))


/* Helper macros */

#define TIM4_RCC_PATTERN        ( 0x1 << 2u )
#define TIM4_ENABLE()           REG_SET(RCC->APBENR[0],   TIM4_RCC_PATTERN)
#define TIM4_DISABLE()          REG_CLR(RCC->APBENR[0],   TIM4_RCC_PATTERN)


/*  IWDG - independent watchdog
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_iwdg_t
 *  \brief  Representation of IWDG register.
 *
 *  Described in reference manual p.689ff.
 */
typedef struct {
    volatile uint32_t KR;       /**< . */
    volatile uint32_t PR;       /**< . */
    volatile uint32_t RLR;      /**< . */
    volatile uint32_t SR;       /**< . */
} reg_iwdg_t;


#define IWDG ((reg_iwdg_t *)(APB1_SECTION + 0x3000))


/*  WWDG - window watch dog
 * ------------------------------------------------------------------------- */


/*  CRYP - cryptxxxx
 * ------------------------------------------------------------------------- */


/*  RNG - random number generator
 * ------------------------------------------------------------------------- */


/*  HASH - hashxxx
 * ------------------------------------------------------------------------- */


/*  RTC - real time clock
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_rtc_t
 *  \brief  Representation of RTC register.
 *
 *  Described in reference manual p.778ff.
 */
typedef struct {
    volatile uint32_t TR;       /**< . */
    volatile uint32_t DR;       /**< . */
    volatile uint32_t CR;       /**< . */
    volatile uint32_t ISR;      /**< . */
    volatile uint32_t PRER;     /**< . */
    volatile uint32_t WUTR;     /**< . */
    volatile uint32_t CALIBR;   /**< . */
    volatile uint32_t ALRMAR;   /**< . */
    volatile uint32_t ALRMBR;   /**< . */
    volatile uint32_t WPR;      /**< . */
    volatile uint32_t SSR;      /**< . */
    volatile uint32_t SHIFTR;   /**< . */
    volatile uint32_t TSTR;     /**< . */
    volatile uint32_t TSDR;     /**< . */
    volatile uint32_t TSSSR;    /**< . */
    volatile uint32_t CALR;     /**< . */
    volatile uint32_t TAFCR;    /**< . */
    volatile uint32_t ALRMASSR; /**< . */
    volatile uint32_t ALRMBSSR; /**< . */
    uint32_t RESERVED;
    volatile uint32_t BKPR[20]; /**< . */
} reg_rtc_t;


#define RTC ((reg_rtc_t *)(APB1_SECTION + 0x2800))


/*  I2C - inter-integrated circuit interface
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_i2c_t
 *  \brief  Representation of I2C register.
 *
 *  Described in reference manual p.817ff.
 */
typedef struct {
    volatile uint32_t CR1;      /**< . */
    volatile uint32_t CR2;      /**< . */
    volatile uint32_t OAR1;     /**< . */
    volatile uint32_t OAR2;     /**< . */
    volatile uint32_t DR;       /**< . */
    volatile uint32_t SR1;      /**< . */
    volatile uint32_t SR2;      /**< . */
    volatile uint32_t CCR;      /**< . */
    volatile uint32_t TRISE;    /**< . */
    volatile uint32_t FLTR;     /**< . */
} reg_i2c_t;


#define I2C1 ((reg_i2c_t *)(APB1_SECTION + 0x5400))
#define I2C2 ((reg_i2c_t *)(APB1_SECTION + 0x5800))
#define I2C3 ((reg_i2c_t *)(APB1_SECTION + 0x5c00))


/*  SPI - serial peripheral interface
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_spi_t
 *  \brief  Representation of SPI register.
 *
 *  Described in reference manual p.852ff.
 */
typedef struct {
    volatile uint32_t CR1;      /**< . */
    volatile uint32_t CR2;      /**< . */
    volatile uint32_t SR;       /**< . */
    volatile uint32_t DR;       /**< . */
    volatile uint32_t CRCPR;    /**< . */
    volatile uint32_t RXCRCR;   /**< . */
    volatile uint32_t TXCRCR;   /**< . */
    volatile uint32_t I2SCFGR;  /**< . */
    volatile uint32_t I2SPR;    /**< . */
} reg_spi_t;


#define SPI1 ((reg_spi_t *)(APB2_SECTION + 0x13000))
#define SPI2 ((reg_spi_t *)(APB1_SECTION + 0x3800))
#define SPI3 ((reg_spi_t *)(APB1_SECTION + 0x3c00))
#define SPI4 ((reg_spi_t *)(APB2_SECTION + 0x13400)
#define SPI5 ((reg_spi_t *)(APB2_SECTION + 0x15000)
#define SPI6 ((reg_spi_t *)(APB2_SECTION + 0x15400)


/*  SAI - sxxx audio interface
 * ------------------------------------------------------------------------- */


/*  USART - universal synchronous/asynchronous receiver transmitter
 * ------------------------------------------------------------------------- */


/*  SDIO - secure digital input output
 * ------------------------------------------------------------------------- */


/*  CAN - cxxx
 * ------------------------------------------------------------------------- */


/*  ETH/MAC - network
 * ------------------------------------------------------------------------- */


/*  FMC - flexible memory controller
 * ------------------------------------------------------------------------- */
//#define FMC_BASE                        (0xa0000000)

/**
 *  \struct reg_fmc_sram_t
 *  \brief  Representation of SRAM register (Bank 1).
 */
typedef struct {
    volatile uint32_t BCR1;     /**< (Sub)Bank 1 control register. */
    volatile uint32_t BTR1;     /**< (Sub)Bank 1 timing registers. */
    volatile uint32_t BCR2;     /**< (Sub)Bank 2 control register. */
    volatile uint32_t BTR2;     /**< (Sub)Bank 2 timing registers. */
    volatile uint32_t BCR3;     /**< (Sub)Bank 3 control register. */
    volatile uint32_t BTR3;     /**< (Sub)Bank 3 timing registers. */
    volatile uint32_t BCR4;     /**< (Sub)Bank 4 control register. */
    volatile uint32_t BTR4;     /**< (Sub)Bank 4 timing registers. */
    uint32_t RESERVED1[57];
    volatile uint32_t BWTR1;    /**< (Sub)Bank 1 write timing registers. */
    uint32_t RESERVED2;
    volatile uint32_t BWTR2;    /**< (Sub)Bank 2 write timing registers. */
    uint32_t RESERVED3;
    volatile uint32_t BWTR3;    /**< (Sub)Bank 3 write timing registers. */
    uint32_t RESERVED4;
    volatile uint32_t BWTR4;    /**< (Sub)Bank 4 write timing registers. */
} reg_fmc_sram_t;


/**
 *  \struct reg_fmc_t
 *  \brief  Representation of FMC register.
 *
 *  Described in reference manual p.1580ff.
 */
typedef struct {
    reg_fmc_sram_t SRAM;        /**< SRAM registers (Bank 1). */
} reg_fmc_t;


/* Register macro */

#define FMC                     ( (reg_fmc_t *) FMC_BASE )


/* Helper macros */

#define FMC_ENABLE()            REG_SET(RCC->AHBENR[2],   FMC_RCC_PATTERN)
#define FMC_DISABLE()           REG_CLR(RCC->AHBENR[2],   FMC_RCC_PATTERN)



///* Bank 1 */
#define REG_FMC_BCR1 (*((volatile uint32_t *)(FMC_BASE + 0x00)))
#define REG_FMC_BTR1 (*((volatile uint32_t *)(FMC_BASE + 0x04)))
#define REG_FMC_BCR2 (*((volatile uint32_t *)(FMC_BASE + 0x08)))
#define REG_FMC_BTR2 (*((volatile uint32_t *)(FMC_BASE + 0x0c)))

#define REG_FMC_BCR3 (*((volatile uint32_t *)FMC_BASE + 0x10))
#define REG_FMC_BTR3 (*((volatile uint32_t *)FMC_BASE + 0x14))
#define REG_FMC_BCR4 (*((volatile uint32_t *)FMC_BASE + 0x18))
#define REG_FMC_BTR4 (*((volatile uint32_t *)FMC_BASE + 0x1c))


/* Bank 2 */
#define REG_FMC_PCR2  (*((volatile uint32_t *)(FMC_BASE + 0x60)))
#define REG_FMC_SR2   (*((volatile uint32_t *)(FMC_BASE + 0x64)))
#define REG_FMC_PMEM2 (*((volatile uint32_t *)(FMC_BASE + 0x68)))
#define REG_FMC_PATT2 (*((volatile uint32_t *)(FMC_BASE + 0x6c)))

#define REG_FMC_ECCR2 (*((volatile uint32_t *)(FMC_BASE + 0x74)))


/* Bank 3 */
#define REG_FMC_PCR3  (*((volatile uint32_t *)FMC_BASE + 0x80))
#define REG_FMC_SR3   (*((volatile uint32_t *)FMC_BASE + 0x84))
#define REG_FMC_PMEM3 (*((volatile uint32_t *)FMC_BASE + 0x88))
#define REG_FMC_PATT3 (*((volatile uint32_t *)FMC_BASE + 0x8c))

#define REG_FMC_ECCR3 (*((volatile uint32_t *)FMC_BASE + 0x94))

/* Bank 4 */
#define REG_FMC_PCR4  (*((volatile uint32_t *)FMC_BASE + 0xa0))
#define REG_FMC_SR4   (*((volatile uint32_t *)FMC_BASE + 0xa4))
#define REG_FMC_PMEM4 (*((volatile uint32_t *)FMC_BASE + 0xa8))
#define REG_FMC_PATT4 (*((volatile uint32_t *)FMC_BASE + 0xac))

#define REG_FMC_PIO4  (*((volatile uint32_t *)FMC_BASE + 0xb0))

/* Bank 1E */
#define REG_FMC_BWTR1 (*((volatile uint32_t *)(FMC_BASE + 0x104)))
#define REG_FMC_BWTR2 (*((volatile uint32_t *)(FMC_BASE + 0x10c)))

#define REG_FMC_BWTR3 (*((volatile uint32_t *)FMC_BASE + 0x114))
#define REG_FMC_BWTR4 (*((volatile uint32_t *)FMC_BASE + 0x11c))

/* SD RAM - Bank 5/6 */
#define REG_FMC_SDCR1 (*((volatile uint32_t *)FMC_BASE + 0x140))
#define REG_FMC_SDCR2 (*((volatile uint32_t *)FMC_BASE + 0x144))
#define REG_FMC_SDTR1 (*((volatile uint32_t *)FMC_BASE + 0x148))
#define REG_FMC_SDTR2 (*((volatile uint32_t *)FMC_BASE + 0x14c))
#define REG_FMC_SDCMR (*((volatile uint32_t *)FMC_BASE + 0x150))
#define REG_FMC_SDRTR (*((volatile uint32_t *)FMC_BASE + 0x154))
#define REG_FMC_SDSR  (*((volatile uint32_t *)FMC_BASE + 0x158))

/** \TODO ALLES. */


/*  MPU - memory protection unit
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_mpu_t
 *  \brief  Representation of MPU register.
 *
 *  Described in programming manual p.178ff.
 */
typedef struct {
    volatile uint32_t TYPER;    /**< . */
    volatile uint32_t CTRL;     /**< . */
    volatile uint32_t RNR;      /**< . */
    volatile uint32_t RBAR;     /**< . */
    volatile uint32_t RASR;     /**< . */
    /* Alias of previous two registers
       volatile uint32_t RBAR_A1;
       volatile uint32_t RASR_A1;
       volatile uint32_t RBAR_A2;
       volatile uint32_t RASR_A2; */
} reg_mpu_t;


#define MPU ((reg_mpu_t *)(INTERNAL_SECTION + 0xed90))


/*  SCB - system control block
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_scb_t
 *  \brief  Representation of SCB register.
 *
 *  Described in programming manual p.206ff.
 */
typedef struct {
    volatile uint32_t CPUID;    /**< . */
    volatile uint32_t ICSR;     /**< . */
    volatile uint32_t VTOR;     /**< . */
    volatile uint32_t AIRCR;    /**< . */
    volatile uint32_t SCR;      /**< . */
    volatile uint32_t CCR;      /**< . */
    volatile uint32_t SHPR[4];  /**< . */
    volatile uint32_t CFSR;     /**< . */
    volatile uint32_t HFSR;     /**< . */
    volatile uint32_t NMAR;     /**< . */
    volatile uint32_t BFAR;     /**< . */
    volatile uint32_t AFSR;     /**< . */
//    volatile uint32_t PFR[2];   /**< . */
//    volatile uint32_t DFR;      /**< . */
//    volatile uint32_t ADR;      /**< . */
//    volatile uint32_t MMFR[4];  /**< . */
//    volatile uint32_t ISAR;     /**< Instruction Set Attributes Register. */
//    uint32_t RESERVED[5];
//    volatile uint32_t CPACR;    /**< Coprocessor Access Control Register. */
} reg_scb_t;


#define SCB ((reg_scb_t *)(INTERNAL_SECTION + 0xed00))


/*  STK - system tick timer
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_stk_t
 *  \brief  Representation of STK register.
 *
 *  Described in programming manual p.230ff.
 */
typedef struct {
    volatile uint32_t CTRL;     /**< . */
    volatile uint32_t LOAD;     /**< . */
    volatile uint32_t VAL;      /**< . */
    volatile uint32_t CALIB;    /**< . */
} reg_stk_t;


#define STK ((reg_stk_t *)(INTERNAL_SECTION + 0xe010))


/*  FPU - floating point unit
 * ------------------------------------------------------------------------- */

/**
 *  \struct reg_fpu_t
 *  \brief  Representation of FPU register.
 *
 *  Described in programming manual p.236ff.
 */
typedef struct {
    volatile uint32_t CPACR;    /**< . */
    uint32_t RESERVED[106];     /**< . */
    volatile uint32_t FPCCR;    /**< . */
    volatile uint32_t FPCAR;    /**< . */
    volatile uint32_t FPDSCR;   /**< . */
} reg_fpu_t;


#define FPU ((reg_fpu_t *)(INTERNAL_SECTION + 0xed88))
#endif

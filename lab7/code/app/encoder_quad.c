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
 * -- Description : This code controls the module encoder quadrature
 * --
 * --
 * -- $Id: encoder_quad.c 1562 2018-09-23 16:50:09Z ruan $
 * ------------------------------------------------------------------------- */

/* Includes ------------------------------------------------------------------*/
#include "reg_ctboard.h"
#include "encoder_quad.h"
#include "hal_gpio.h"
#include "hal_timer.h"
#include "sensor.h"

/* User define    -----------------------------------------------------------*/
#define EXTI_PINSOURCE8      ((uint8_t)0x08)
#define EXTI_PINSOURCE9      ((uint8_t)0x09)
#define EXTI_PINSOURCE10     ((uint8_t)0x0A)
#define EXTI_PINSOURCE11     ((uint8_t)0x0B)
#define EXTI_PORTSOURCEGPIOB ((uint8_t)0x01)

#define EXTI_LINE8           ((uint32_t)0x00100) // !< External interrupt line 8
#define EXTI_LINE9           ((uint32_t)0x00200) // !< External interrupt line 9
#define EXTI_LINE10          ((uint32_t)0x00400) // !< External interrupt line 10
#define EXTI_LINE11          ((uint32_t)0x00800) // !< External interrupt line 11

#define EXTI_PR_PR8          ((uint32_t)0x00000100) // !< Pending bit for line 8
#define EXTI_PR_PR9          ((uint32_t)0x00000200) // !< Pending bit for line 9
#define EXTI_PR_PR10         ((uint32_t)0x00000400) // !< Pending bit for line 10
#define EXTI_PR_PR11         ((uint32_t)0x00000800) // !< Pending bit for line 11

#define EXTI15_10_IRQN       40 // !< External Line[15:10] Interrupts
#define EXTI9_5_IRQN         23 // !< External Line[9:5] Interrupts

#define TIM3_PRESCALER       8400 // f = 84MHz/8400 = 10kHz --> 100us
                                  // --> frequency measurement range=[0.15,10k]Hz
#define TIM3_1HZ_PERIOD      10000 // number of clockcycles (ticks) of TIM3
                                   // required for one second
#define RPM_MULTIPLIER       60 // constant to convert from seconds to minutes

/// STUDENTS: To be programmed
uint8_t sensors = 0;



/// END: To be programmed

/* variables and functions with module wide scope ----------------------------*/

/// STUDENTS: To be programmed
uint8_t getSensors(void)
{
	return sensors;
}

void resetSensors(void)
{
	sensors = 0;
}


/// END: To be programmed

static void encoder_interrupts_enable(uint8_t sensor_id);

/* Public function definitions ----------------------------------------------*/

/*
 * See header file
 */
void encoder_quad_init()
{
    // Init SENSOR_Ux GPIOs as input
    sensor_init();

    /// STUDENTS: To be programmed

		hal_gpio_init_output

    /// END: To be programmed
}

/// STUDENTS: To be programmed




/// END: To be programmed

/**
 *  \brief  Initialize the interrupts (EXTI)
 *  \param  sensor_id: the number of the selected pin
 */
static void encoder_interrupts_enable(uint8_t sensor_id)
{
    uint32_t exti_pinsource = 0;
    uint32_t irq = 0;
    uint32_t temp = 0x00;
    uint32_t iocurrent = (0x100 << sensor_id);

    // Clear EXTI line configuration
    temp = EXTI->IMR;
    temp &= ~(iocurrent);
    temp |= iocurrent;
    EXTI->IMR = temp;

    // Enable Rising  edge configuration
    temp = EXTI->RTSR;
    temp &= ~(iocurrent);
    temp |= iocurrent;
    EXTI->RTSR = temp;

    // Disable Falling  edge configuration
    temp = EXTI->FTSR;
    temp &= ~(iocurrent);
    EXTI->FTSR = temp;

    switch (sensor_id) {
        case SENSOR_U6:
            exti_pinsource = EXTI_PINSOURCE8;
            irq = EXTI9_5_IRQN;
            break;
        case SENSOR_U5:
            exti_pinsource = EXTI_PINSOURCE9;
            irq = EXTI9_5_IRQN;
            break;
        case SENSOR_U4:
            exti_pinsource = EXTI_PINSOURCE10;
            irq = EXTI15_10_IRQN;
            break;
        case SENSOR_U3:
            exti_pinsource = EXTI_PINSOURCE11;
            irq = EXTI15_10_IRQN;
            break;
        default:  exti_pinsource = EXTI_PINSOURCE8;
            break;
    }

    // Enable EXTI
    temp = ~(((uint32_t)0x0F) << (0x04 * (exti_pinsource & 0x03)));
    SYSCFG->EXTICR[exti_pinsource >> 0x02] &= temp;

    temp =
        (((uint32_t)EXTI_PORTSOURCEGPIOB) << (0x04 * (exti_pinsource & 0x03)));
    SYSCFG->EXTICR[exti_pinsource >> 0x02] |= temp;

    // Enable NVIC interrupt
    NVIC->ISER[irq >> 0x05] = (uint32_t)0x01 << (irq & (uint8_t)0x1F);
}

/**
 *  \brief  Interrupt Service Routine for Line 5 to 9
 *  \param  void: -
 */
void EXTI9_5_IRQHandler(void)
{
    // Reset interrupt flags
    if ((EXTI->IMR & EXTI_LINE8) && (EXTI->PR & EXTI_PR_PR8)) {
        EXTI->PR |= EXTI_PR_PR8;//interrupt sensor u6
    }else if ((EXTI->IMR & EXTI_LINE9) && (EXTI->PR & EXTI_PR_PR9)) {
        EXTI->PR |= EXTI_PR_PR9;
    }

    /// STUDENTS: To be programmed
	



    /// END: To be programmed
}

/**
 *  \brief  Interrupt Service Routine for Line 10 to 15
 *  \param  void: -
 */
void EXTI15_10_IRQHandler(void)
{
    // Reset interrupt flags
    if ((EXTI->IMR & EXTI_LINE10) && (EXTI->PR & EXTI_PR_PR10)) {
        EXTI->PR |= EXTI_PR_PR10;
				sensors|=(1<<U6);
    }
    if ((EXTI->IMR & EXTI_LINE11) && (EXTI->PR & EXTI_PR_PR11)) {
        EXTI->PR |= EXTI_PR_PR11;
				sensors|=(1<<U5);
    }

    // This task has not to be solved by the students.
    // It is a optional task.    
    /// STUDENTS: To be programmed




    /// END: To be programmed
}

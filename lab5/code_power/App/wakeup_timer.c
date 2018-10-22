/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Implementation of module wakeup timer.
 *
 *  $Id: wakeup_timer.c 1597 2018-10-04 09:07:09Z ruan $
 * ------------------------------------------------------------------------- */


/* Standard includes */
#include <reg_stm32f4xx.h>

/* User includes */
#include "wakeup_timer.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define TIME_OUT 0x1000
#define LSION    (0x01 << 0u)
#define LSIRDY   (0x01 << 1u)
#define WUTWF    (0x01 << 2u)


/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void wakeup_init(void)
{
    uint32_t reg = 0;
    uint32_t count = 0;

    /* In LP mode the clocks might be disabled.
     * Enable LSI and wait till its ready */
    RCC->CSR |= LSION;
    do {
        reg = RCC->CSR & LSIRDY;
        count++;
    } while ((reg == 0) && (count != TIME_OUT));

    /*
     * There is no wakeup timer hal, please read/write
     * directly to the registers.
     */

    /// STUDENTS: To be programmed
		
			PWR->CR|=(1<<8); //DWP bit set
			RTC->WPR=0xCA;
			RTC->WPR=0x53;		//
			RCC->BDCR|=(1<<9);
			RCC->BDCR&=~(1<<8);
			RCC->BDCR|=(1<<15);

			RTC->CR&=~(1<<10);//disable wakup timer
			while((RTC->ISR&(1<<2))==0){}
			RTC->CR&=~(1<<2);
			RTC->CR|=(3);
			RTC->WUTR=1600;
			RTC->CR|=(1<<14);//
			RTC->ISR&=~(1<<10);
			EXTI->RTSR|=(1<<22);
			EXTI->IMR|=(1<<22);
			NVIC->ISER[0]|=(1<<3);
			RTC->CR|=(1<<10);//enable wakup timer
			RTC->WPR=0xFF;
			
				
    /// END: To be programmed
}

/* -- Interrupt function definitions
 * ------------------------------------------------------------------------- */

/// STUDENTS: To be programmed

	void RTC_WKUP_IRQHandler(void)
	{
		RTC->ISR&=~(1<<10);
		EXTI->PR|=(1<<22);
	}


/// END: To be programmed

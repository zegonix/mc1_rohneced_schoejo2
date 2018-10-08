/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ------------------------------------------------------------------------- */
/**
 *  \brief  Implementation of module hal_adc.
 *
 *  The hardware abstraction layer for the analog to digital converter.
 *
 *  $Id$
 * ------------------------------------------------------------------------- */

/* User includes */
#include "hal_adc.h"


/* -- Public function definitions
 * ------------------------------------------------------------------------- */

/*
 * See header file
 */
void hal_adc_reset(reg_adc_t *adc)
{
    adc->CR1 = 0u;
    adc->CR2 = 0u;
    adc->SQR[0] = 0u;
}


/*
 * See header file
 */
void hal_adc_init_base(reg_adc_t *adc, hal_adc_init_t init)
{
    uint32_t reg;

    /* process resolution and scan mode */
    reg = (init.scan_mode << 8u) |
          (init.resolution << 24u);
    adc->CR1 = reg;

    /* process others */
    reg = (init.alignment << 11u) |
          (init.trigger << 24u) |
          (init.polarity << 28u) |
          (init.continuous_mode << 1u);
    adc->CR2 = reg;

    /* process nr_conversions */
    if (init.nr_conversions == 0) {
        reg = (0u << 20u);
    } else {
        reg = ((init.nr_conversions - 1) << 20u);
    }
    adc->SQR[0] &= ~(0xf << 20u);
    adc->SQR[0] |= reg;
    
    /* enable */
    adc->CR2 |= (0x1 << 0u);
}


/*
 * See header file
 */
void hal_adc_init_channel(reg_adc_t *adc, 
                          hal_adc_channel_t channel,
                          hal_adc_ch_init_t init)
{
    uint32_t reg, clear_reg;
    uint8_t shift_reg;
    
    /* check rank */
    if (init.rank < 1u || 
        init.rank > 16u) {
        return;
    }
        
    /* process sampling rank */
    clear_reg = 0x1f;
    reg = channel;
    if (init.rank >= 13u) {
        /* rank 13..16 -> SQR1 */
        shift_reg = (init.rank - 13u) * 5u;
        clear_reg <<= shift_reg;
        reg <<= shift_reg;
        adc->SQR[0] &= clear_reg;
        adc->SQR[0] |= reg;
    } else if (init.rank >= 7u) {
        /* rank 7..12 -> SQR2 */
        shift_reg = (init.rank - 7u) * 5u;
        clear_reg <<= shift_reg;
        reg <<= shift_reg;
        adc->SQR[1] &= clear_reg;
        adc->SQR[1] |= reg;
    } else if (init.rank >= 1u) {
        /* rank 1..6 -> SQR3 */
        shift_reg = (init.rank - 1u) * 5u;
        clear_reg <<= shift_reg;
        reg <<= shift_reg;
        adc->SQR[2] &= clear_reg;
        adc->SQR[2] |= reg;
    }
        
    /* process sampling cycles */
    clear_reg = 0x7;
    reg = init.cycles;
    if (channel >= HAL_ADC_CH10) {
        /* rank 10..15 -> SMPR1 */
        shift_reg = (channel - HAL_ADC_CH10) * 3u;
        clear_reg <<= shift_reg;
        reg <<= shift_reg;
        adc->SMPR[0] &= clear_reg;
        adc->SMPR[0] |= reg;
    } else {//if (channel >= HAL_ADC_CH0) {
        /* rank 0..9 -> SMPR2 */
        shift_reg = (channel - HAL_ADC_CH0) * 3u;
        clear_reg <<= shift_reg;
        reg <<= shift_reg;
        adc->SMPR[1] &= clear_reg;
        adc->SMPR[1] |= reg;
    }
}


/*
 * See header file
 */
void hal_adc_start(reg_adc_t *adc)
{
    adc->CR2 |= (0x1 << 30u);
}


/*
 * See header file
 */
void hal_adc_stop(reg_adc_t *adc)
{
    adc->CR2 &= ~(0x1 << 30u);
}


/*
 * See header file
 */
void hal_adc_set_dma(reg_adc_t *adc,
                     hal_bool_t status)
{
    if (status == DISABLE) {
        adc->CR2 &= ~((1u << 8u) |
                      (1u << 9u));
    } else {
        adc->CR2 |= (1u << 8u) |
                    (1u << 9u);
    }
}

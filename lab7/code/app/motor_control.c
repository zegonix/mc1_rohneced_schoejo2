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
 * -- Description : This code controls the motor
 * --
 * --
 * -- $Id: motor_control.c 1561 2018-09-23 16:48:07Z ruan $
 * --------------------------------------------------------------------------*/

/* Includes -----------------------------------------------------------------*/
#include "motor_control.h"
#include "hal_timer.h"
#include "hal_gpio.h"
#include "hal_adc.h"
#include "buttons.h"

/* User define    -----------------------------------------------------------*/
#define ADC_IRQn            18    /*!< ADC1, ADC2 and ADC3 global Interrupts */
#define TIMER4_COUNT_VALUE  1680

#define MOTOR_STATE_IDLE    1
#define MOTOR_STATE_STOP    2
#define MOTOR_STATE_LEFT    3
#define MOTOR_STATE_RIGHT   4

#define MOTOR_SPEED_DEFAULT 35

/* Variables and functions with module wide scope ----------------------------*/
static uint16_t ccr1_val = TIMER4_COUNT_VALUE / 100 * 30;
static uint16_t global_speed = 0;
static uint8_t flag_new_speed_value = 0;

static void motor_control_init(void);
static void motor_control_speed_init(void);
static void motor_control_rotation_direction_init(void);
static void motor_stop(uint8_t stop_mode);
static void motor_adc_speed_measure_start(void);
//static void motor_adc_speed_measure_stop(void);
static uint8_t motor_start(uint8_t direction, uint8_t speed_in_percent);

/* Public function definitions ----------------------------------------------*/

/*
 * See header file
 */
void motor_init(void)
{
    // Initializes the GPIOs for the PWM output sigal
    motor_control_init();

    // Initialize LED D2 and D1 on encounter-module
    motor_control_rotation_direction_init();

    // Stop motor at startup
    motor_stop(MOTOR_STATE_IDLE);

    // Initialize the ADC, Potentiometer and Timer2
    motor_control_speed_init();

    // Starts Timer2 and ADC conversion:
    // 10ms intervall for reading ADC
    motor_adc_speed_measure_start();
}

/*
 * See header file
 */
void motor_control_excecute(void)
{
    static uint8_t speedMotor = MOTOR_SPEED_DEFAULT;

    // Function processes the edge detection of the buttons
    button_detection();

    //--------------------------------------------------------
    // Velocity control unit
    // Reads the speed of the motor over the potentiometer
    //--------------------------------------------------------
    if (flag_new_speed_value == 1) {
        speedMotor = global_speed;
    }

    //--------------------------------------------------------
    // Motor control unit
    // Controls and starts the motor through Buttons T0, T1
    //--------------------------------------------------------

    if (button_T0_pressed()) {
        motor_start(MOTOR_STATE_RIGHT, speedMotor);
    }else if (button_T1_pressed()) {
        motor_start(MOTOR_STATE_LEFT, speedMotor);
    }else if (button_T0_released() || button_T1_released()) {
        motor_stop(MOTOR_STATE_STOP);
    }
}

/**
 *  \brief  Initializes the GPIOs for the PWM output sigal
 *  \param  void: -
 */
void motor_control_init(void)
{
    hal_timer_base_init_t timer_base_init;
    hal_timer_output_init_t timer_out_init;
    hal_gpio_output_t gpio_output_init;

    // Periphery enable
    TIM4_ENABLE();
    GPIOB_ENABLE();

    // Motor speed init
    // Timer base initialization
    timer_base_init.count = TIMER4_COUNT_VALUE;
    timer_base_init.mode = HAL_TIMER_MODE_DOWN;
    timer_base_init.prescaler = 2;
    timer_base_init.run_mode = HAL_TIMER_RUN_CONTINOUS;
    timer_base_init.master_mode = HAL_TIMER_MASTER_RESET;
    hal_timer_init_base(TIM4, timer_base_init);

    // Timer PWM initialization
    timer_out_init.mode = HAL_TIMER_OCMODE_PWM1;
    timer_out_init.output_state = ENABLE;
    timer_out_init.polarity = HAL_TIMER_POLARITY_HIGH;
    timer_out_init.pulse = ccr1_val;
    hal_timer_init_output(TIM4, HAL_TIMER_CH1, timer_out_init);

    // Motor control output - Init the GPIO-TypeDef
    gpio_output_init.out_speed = HAL_GPIO_OUT_SPEED_100MHZ;
    gpio_output_init.out_type = HAL_GPIO_OUT_TYPE_PP;
    gpio_output_init.pupd = HAL_GPIO_PUPD_UP;

    // Motor speed - PWM Signal
    gpio_output_init.pins = 0x40;
    hal_gpio_init_alternate(GPIOB, HAL_GPIO_AF_TIM4, gpio_output_init);

    // Motor Direction - Output
    gpio_output_init.pins = 0x80;
    hal_gpio_init_output(GPIOB, gpio_output_init);

    // Debug signal - irq active state
    gpio_output_init.pins = 0x01;
    hal_gpio_init_output(GPIOA, gpio_output_init);
}

/**
 *  \brief  Initializes LED on encoder-module for showing the motor rotation direction
 *  \param  void: -
 */
void motor_control_rotation_direction_init(void)
{
    hal_gpio_output_t gpio_output_init;

    // Motor direction output- LEDS - Init the GPIO
    gpio_output_init.out_speed = HAL_GPIO_OUT_SPEED_100MHZ;
    gpio_output_init.out_type = HAL_GPIO_OUT_TYPE_PP;
    gpio_output_init.pupd = HAL_GPIO_PUPD_NOPULL;
    gpio_output_init.pins = 0x010 | 0x020;
    hal_gpio_init_output(GPIOB, gpio_output_init);
}

/**
 *  \brief  Initialize the ADC, Potentiometer and Timer2
 *  \param  void: -
 */
void motor_control_speed_init(void)
{
    hal_timer_base_init_t timer_base_init;
    hal_timer_output_init_t timer_out_init;
    hal_adc_ch_init_t adc_ch_init;
    hal_gpio_input_t gpio_input_init;
    hal_adc_init_t adc_init;

    // Periphery enable
    ADC3_ENABLE();
    TIM2_ENABLE();

    //----------------------------------------
    // Speed of motor - ADC
    //----------------------------------------
    // Timer init - Init the TIM2 for ADC measure frequency
    timer_base_init.count = 1000;
    timer_base_init.mode = HAL_TIMER_MODE_DOWN;
    timer_base_init.prescaler = 8400; // f = 84MHz/8400/1000 = 10Hz --> 100ms
    timer_base_init.run_mode = HAL_TIMER_RUN_CONTINOUS;
    timer_base_init.master_mode = HAL_TIMER_MASTER_RESET;
    hal_timer_init_base(TIM2, timer_base_init);

    // Timer PWM init
    timer_out_init.mode = HAL_TIMER_OCMODE_PWM1;
    timer_out_init.output_state = ENABLE;
    timer_out_init.polarity = HAL_TIMER_POLARITY_LOW;
    timer_out_init.pulse = 500;
    hal_timer_init_output(TIM2, HAL_TIMER_CH2, timer_out_init);

    TIM2->DIER |= 0x04;

    // ADC init - Init the ADC
    adc_init.alignment = HAL_ADC_ALIGN_RIGHT;
    adc_init.continuous_mode = DISABLE;
    adc_init.nr_conversions = 1;
    adc_init.polarity = HAL_ADC_POLARITY_RISING;
    adc_init.resolution = HAL_ADC_RES_6B;
    adc_init.scan_mode = DISABLE;
    adc_init.trigger = HAL_ADC_REGTRG_T2CC2;
    hal_adc_init_base(ADC3, adc_init);

    //ADC channel selection
    adc_ch_init.cycles = HAL_ADC_SAMPLING_56C;
    adc_ch_init.rank = 1; //1-16
    hal_adc_init_channel(ADC3, HAL_ADC_CH4, adc_ch_init);

    // Enable interrupt for ADC
    ADC3->CR1 |= 1 << 5;

    // Enable interrupt request for ADC in NVIC
    NVIC->ISER[ADC_IRQn >> 0x05] =
        (uint32_t)0x01 << (ADC_IRQn & (uint8_t)0x1F);

    // Init GPIO: PF6 as analog pin
    gpio_input_init.pins = 0x040;
    gpio_input_init.pupd = HAL_GPIO_PUPD_NOPULL;
    hal_gpio_init_analog(GPIOF, gpio_input_init);
}

/**
 *  \brief  Interrupt Handler of the ADC
 *  \param  void: -
 */
void ADC_IRQHandler(void)
{
    uint16_t data = 0;

    if ((ADC3->SR & 0x02) >= 1) {
        ADC3->SR &= ~0x02; //Clear EOC and JEOC Flag
        data = ADC3->DR;
        data = (float)(data + 20);  //Data is a 6-Bit Value from 0 to 63!
        //Motor is not possible to drive over values of 80!
        //Else use: data/64.0f*100.0f for 0% to 100%
        //Or: data/64.0f*80.0f for 0% to 80%

        if (data != global_speed) {
            global_speed = data;
            flag_new_speed_value = 1;
        } else {
            flag_new_speed_value = 0;
        }
    }
}

/**
 *  \brief  Starts the ADC3 to measure the poti
 *  \param  void: -
 */
void motor_adc_speed_measure_start(void)
{
    hal_timer_start(TIM2);
    hal_adc_start(ADC3);
}

/**
 *  \brief  Stops the ADC3 to measure the poti
 *  \param  void: -
 */
//void motor_adc_speed_measure_stop(void)
//{
//    hal_timer_stop(TIM2);
//    hal_adc_stop(ADC3);
//}

/**
 *  \brief  Starts the motor
 *  \param  direction: sets the rotation direction LEFT or RIGHT
 *  \param  speed_in_percent: sets the velocity of the motor (1 to 100%)
 *  \return returns error (1) or no error (0)
 */
uint8_t motor_start(uint8_t direction, uint8_t speed_in_percent)
{
    uint8_t error = 0;
    hal_timer_output_init_t timer_out_init;
    hal_gpio_output_t gpio_output_init;

    hal_timer_stop(TIM4);

    // Motor control output - Init the GPIO-TypeDef
    gpio_output_init.out_speed = HAL_GPIO_OUT_SPEED_100MHZ;
    gpio_output_init.out_type = HAL_GPIO_OUT_TYPE_PP;
    gpio_output_init.pupd = HAL_GPIO_PUPD_UP;

    // Motor state stop
    gpio_output_init.pins = 0x40;
    hal_gpio_init_output(GPIOB, gpio_output_init);
    hal_gpio_bit_set(GPIOB, 0x40);
    gpio_output_init.pins = 0x80;
    hal_gpio_init_output(GPIOB, gpio_output_init);
    hal_gpio_bit_set(GPIOB, 0x80);

    // Timer PWM init
    timer_out_init.mode = HAL_TIMER_OCMODE_PWM1;
    timer_out_init.output_state = ENABLE;
    timer_out_init.polarity = HAL_TIMER_POLARITY_HIGH;

    // Read speed and calculate pwm
    if (speed_in_percent <= 100) {
        ccr1_val = TIMER4_COUNT_VALUE / 100 * speed_in_percent;
        timer_out_init.pulse = ccr1_val;
        error |= 0;
    }else {
        error |= 1;
    }

    // Read direction and configure pwm output mode
    switch (direction) {
        case MOTOR_STATE_LEFT:
            gpio_output_init.pins = 0x40;
            hal_gpio_init_alternate(GPIOB, HAL_GPIO_AF_TIM4, gpio_output_init);
            hal_timer_init_output(TIM4, HAL_TIMER_CH1, timer_out_init);

            gpio_output_init.pins = 0x80;
            hal_gpio_init_output(GPIOB, gpio_output_init);
            hal_gpio_bit_set(GPIOB, 0x80);

            error |= 0;
            break;
        case MOTOR_STATE_RIGHT:
            gpio_output_init.pins = 0x80;
            hal_gpio_init_alternate(GPIOB, HAL_GPIO_AF_TIM4, gpio_output_init);
            hal_timer_init_output(TIM4, HAL_TIMER_CH2, timer_out_init);

            gpio_output_init.pins = 0x40;
            hal_gpio_init_output(GPIOB, gpio_output_init);
            hal_gpio_bit_set(GPIOB, 0x40);

            error |= 0;
            break;
        default:
            error |= 2;
            break;
    }
    // Start timer 4 as pwm
    hal_timer_start(TIM4);

    return error;
}

/**
 *  \brief  Stops the motor
 *  \param  void: -
 */
void motor_stop(uint8_t stop_mode)
{
    hal_gpio_output_t gpio_output_init;

    // Motor state stop - stop pwm signal
    hal_timer_stop(TIM4);

    // Motor control output - Init the GPIO-TypeDef
    gpio_output_init.out_speed = HAL_GPIO_OUT_SPEED_100MHZ;
    gpio_output_init.out_type = HAL_GPIO_OUT_TYPE_PP;
    gpio_output_init.pupd = HAL_GPIO_PUPD_UP;

    switch (stop_mode) {
        case MOTOR_STATE_IDLE:
            gpio_output_init.pins = 0x40;
            hal_gpio_init_output(GPIOB, gpio_output_init);
            hal_gpio_bit_reset(GPIOB, 0x40);

            gpio_output_init.pins = 0x80;
            hal_gpio_init_output(GPIOB, gpio_output_init);
            hal_gpio_bit_reset(GPIOB, 0x80);
            break;
        case MOTOR_STATE_STOP:
            gpio_output_init.pins = 0x40;
            hal_gpio_init_output(GPIOB, gpio_output_init);
            hal_gpio_bit_set(GPIOB, 0x40);

            gpio_output_init.pins = 0x80;
            hal_gpio_init_output(GPIOB, gpio_output_init);
            hal_gpio_bit_set(GPIOB, 0x80);
            break;
        default:
            gpio_output_init.pins = 0x40;
            hal_gpio_init_output(GPIOB, gpio_output_init);
            hal_gpio_bit_set(GPIOB, 0x40);

            gpio_output_init.pins = 0x80;
            hal_gpio_init_output(GPIOB, gpio_output_init);
            hal_gpio_bit_set(GPIOB, 0x80);
            break;
    }
}

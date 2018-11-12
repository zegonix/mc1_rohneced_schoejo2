
/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Project     : MC1 lab file system
 * -- Description : This code controls the accelerometer LSM6DSL
 * --               and writes its values on the SD card
 * --
 * -- $Id: main.c 1583 2018-11-11 09:48:52Z kjaz $
 * ------------------------------------------------------------------------- */

/* includes ---------------------------------------------------------------- */
#include "main.h"
#include "stm32f4xx_hal.h"
#include "fatfs.h"
#include "sdio.h"
#include "spi.h"
#include "gpio.h"
#include "accelerometer.h"
#include <stdint.h>
#include <string.h>

/* defines ------------------------------------------------------------------*/
/* LED0 - LED1 on additional HW. Mapped on GPIOD */
#define SET_LED0_ON         HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET)
#define SET_LED1_ON         HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET)
#define SET_LED2_ON         HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET)
#define SET_LED3_ON         HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET)

#define SET_LED0_OFF        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET)
#define SET_LED1_OFF        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET)
#define SET_LED2_OFF        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET)
#define SET_LED3_OFF        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET)

/* Switch S0-S3 on additional HW. Mapped on GPIOD */
#define READ_S0_STATE       HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5)
#define READ_S1_STATE       HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6)
#define READ_S2_STATE       HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7)
#define READ_S3_STATE       HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8)

/* Button T0-T3 on additional HW. Mapped on GPIOD */
#define READ_T0_STATE       HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9)
#define READ_T1_STATE       HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10)
#define READ_T2_STATE       HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_11)
#define READ_T3_STATE       HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12)

#define MIN_NR_OF_SAMPLES             100
#define NR_OF_AXES                    3
#define FILE_NAME_LENGTH              25

volatile uint8_t data_ready;           // indicates that an interrupt occured
                                       // shared with EXTI9_5_IRQHandler(void)
                                       // in module stm32f4xx_it

static char my_path[FILE_NAME_LENGTH] = {0}; // :TODO: local variable in main
                                             // parameter to write_sd_card fct


/* -- local function declarations -------------------------------------------*/
static void SystemClock_Config(void);
static void init_system(void);
static void calculate_acc_average(int16_t *buffer, int16_t *result,
                                  uint16_t buffer_length);
static void write_to_sd_card(int16_t *acceleration);
static void write_raw_data_to_sd_card(int16_t *acceleration,
                                      uint16_t sample_nr);

/* MAIN FUNCTION ----------------------------------------------------------- */
int main(void)
{
    int16_t acceleration[3];        // average values
    uint8_t switch_pos;
    int16_t acc_buffer[MIN_NR_OF_SAMPLES * NR_OF_AXES];
    uint16_t sample_nr;             // Indicates how many sample(x,y,z value)
                                    // are currently in the buffer
  
    /* MCU Configuration */
	init_system();
  
    /* if swtich S0 = 1 => save data directly form acc_buffer
       if switch S0 = 0 => save calculated average data */
    switch_pos = (~READ_S0_STATE) & 0x01;
  
    if (switch_pos == 0) {
        strncpy(my_path, "average_values.csv\0", FILE_NAME_LENGTH); 
    } else {
        strncpy(my_path, "row_values.csv\0", FILE_NAME_LENGTH);
    }

    /* Exercise 1: Register filesystem object to the fatfs module 
       if mount returns FR_OK set led0 on else toggle led0 */
    /// STUDENTS: To be programmed
    
    
    
    /// END: To be programmed    
    
    /* Exercise 2: Create file, if it exists overwrite it (FA_CREATE_ALWAYS).
       write into file the specified header line */
    /// STUDENTS: To be programmed
    
    
    
    /// END: To be programmed
	
    while (!READ_T0_STATE) {
      if (data_ready > 0) {
          sample_nr = accelerometer_read_acceleration(acc_buffer);  
          if (switch_pos == 0) {
              /* save the calculated average values of the acceleration 
                 to the sd card. */
              calculate_acc_average(acc_buffer, acceleration, sample_nr);
              write_to_sd_card(acceleration);
          } else {
              /* save the raw values of the acceleration to the sd card. */
              write_raw_data_to_sd_card(acc_buffer, sample_nr);
          }
          data_ready = 0;
      } 
    }

    /// STUDENTS: To be programmed
    
    
    
    /// END: To be programmed	
	
}


/**
 *  \brief  calculate the average of a buffer filled with 3 axis values
 *  \param  buffer: buffer which contain 3-axis values
 *  \param  result: array with 3 positions on
 *                  which the result will be stored
 *  \param  buffer_length: length of the input buffer
 */
static void calculate_acc_average(int16_t *buffer, int16_t *result,
                                  uint16_t buffer_length)
{
    int32_t x_sum = 0;
    int32_t y_sum = 0;
    int32_t z_sum = 0;
    uint16_t i;

    for (i = 0; i < buffer_length; i++) {
        x_sum += buffer[i * 3];
        y_sum += buffer[i * 3 + 1];
        z_sum += buffer[i * 3 + 2];
    }

    result[0] = (int16_t)(x_sum / buffer_length);
    result[1] = (int16_t)(y_sum / buffer_length);
    result[2] = (int16_t)(z_sum / buffer_length);
    return;
}


/**
 *  \brief  write average acceleration to the sd card with fatfs
 *  \param  acceleration: mean acceleration data. array size: 3
 */
static void write_to_sd_card(int16_t *acceleration)
{
    /// STUDENTS: To be programmed
    
    
    
    /// END: To be programmed
}

/**
 *  \brief  write row acceleration data to the sd card with fatfs
 *  \param  acceleration: row acceleration data
 *  \param  sample_nr: number of measured acc-data in the x, y and z axis
 */
static void write_raw_data_to_sd_card(int16_t *acceleration, uint16_t sample_nr)
{
    /// STUDENTS: To be programmed
    
    
    
    /// END: To be programmed
}

/**
 *  \brief  performs all the configurations and initializations
 */
static void init_system(void){

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_SDIO_SD_Init();
    MX_FATFS_Init();
    MX_SPI1_Init();

    /* init Sensor */
    accelerometer_init();
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
static void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 

}
#endif /* USE_FULL_ASSERT */


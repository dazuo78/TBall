/**
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards_ComPolling/Src/main.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    29-April-2016
  * @brief   This sample code shows how to use STM32F1xx I2C HAL API to transmit
  *          and receive a data buffer with a communication process based on
  *          Polling transfer.
  *          The communication is done using 2 Boards.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ADXL345.h"

/** @addtogroup STM32F1xx_HAL_Examples
  * @{
  */

/** @addtogroup I2C_TwoBoards_ComPolling
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	float x,y,z;
    short sx,sy,sz;
    unsigned char regVal;
    unsigned char regVals[29]; //0x1D~0x39
	/* STM32F103xG HAL library initialization:
	   - Configure the Flash prefetch
	   - Systick timer is configured by default as source of time base, but user 
		 can eventually implement his proper time base source (a general purpose 
		 timer for example or other time source), keeping in mind that Time base 
		 duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
		 handled in milliseconds basis.
	   - Set NVIC Group Priority to 4
	   - Low Level Initialization
	*/
	HAL_Init();

	/* Configure the system clock to 72 MHz */
	SystemClock_Config();
    
    //uart
    BSP_COM_Init(COM1, &huart);

	/* Configure LED1 and LED3 */
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED3);

	/* Configure Key push-button */
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

	/*##-1- Configure the I2C peripheral ######################################*/
	ADXL345_Init(ADXL345_I2C_COMM);
  
   
    // full resolution
    ADXL345_SetRangeResolution(ADXL345_RANGE_PM_2G, 1); 
    
    // enter measurement mode
    ADXL345_SetPowerMode(1);
    
    // self test mode
//    regVal = ADXL345_GetRegisterValue(ADXL345_DATA_FORMAT);
//    regVal |= ADXL345_SELF_TEST;
//    ADXL345_SetRegisterValue(ADXL345_DATA_FORMAT, regVal);
    // dump ADXL345 registers
    HAL_Delay(1);
    //ADXL345_GetMultiRegisterValue(ADXL345_THRESH_TAP, sizeof(regVals), regVals);

	/* Infinite loop */  
    printf("\n\r UART Printf Example: retarget the C library printf function to the UART\n\r");
	while (1)
	{
        
        ADXL345_GetGxyz(&x, &y, &z);
        printf("\n\rx: %.1f, y: %.1f, z: %.1f\n\r", x, y, z);
       // ADXL345_GetXyz(&sx, &sy, &sz);
       // printf("\n\rx: %d, y: %d, z: %d\n\r", sx, sy, sz);
        HAL_Delay(1000);
	}
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV1                    = 1
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}


/**
  * @brief  I2C error callbacks.
  * @param  I2cHandle: I2C handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Turn LED3 on: Transfer error in reception/transmission process */
  BSP_LED_On(LED3); 
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

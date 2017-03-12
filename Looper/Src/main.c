/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
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
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "fmc.h"

/* USER CODE BEGIN Includes */
#include "ads1256_test.h"
#include "stm32f429i_discovery_sdram.h"
#include "main.h"
#include "string.h"
#include "math.h"
#define pi 3.14159
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

uint16_t s_index = 0;

__IO uint32_t CommandKey = 0;
extern __IO uint8_t Recording;
extern __IO uint8_t Playback;
extern __IO uint32_t CommandKey;
extern __IO uint8_t ToggleRecording;
extern __IO uint8_t StartApp;
extern uint8_t CommandKeyTable[4][4];
extern __IO ButtonStates DubbingPressed;
extern __IO ButtonStates RecordingButton;
extern __IO ButtonStates PlaybackButton;
extern __IO GPIO_PinState GuitarTrigger;

uint16_t taptone_buffer[100];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

uint32_t selectRow(int rowNum);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	uint8_t data;
	HAL_StatusTypeDef status;
	g_tADS1256.ScanMode = 2;
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_SPI3_Init();
  MX_TIM2_Init();
  MX_I2C3_Init();
  MX_FMC_Init();
  MX_ADC1_Init();
  MX_TIM8_Init();

  /* USER CODE BEGIN 2 */
	HAL_NVIC_DisableIRQ(EXTI2_IRQn);
	//KeyboardConfig();
	status = HAL_TIM_Base_Start(&htim2);

	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);

	BSP_SDRAM_Init();
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

	//initTapTone();
//  ShortDelayUS(10);
	ADS1256_WriteCmd(CMD_SELFCAL);
	ADS1256_WriteCmd(CMD_SDATAC);

	data = ADS1256_ReadChipID();
	ADS1256_CfgADC(ADS1256_GAIN_2, ADS1256_15000SPS);

	ADS1256_WriteCmd(CMD_RDATAC);
	HAL_Delay(10);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);

	ADS1256_StartScan(0);
	ADS1256_SetChannel(0);

	status = HAL_TIM_Base_Start_IT(&htim3);
	status = HAL_ADC_Start_IT(&hadc1);
//status = HAL_TIM_Base_Start(&htim8);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//eraseMemory();


	while (StartApp == 0)
		continue;

	while (1) {

		if (Recording == 1) {
			recordLoop();

		} else if (Playback == 1) {
			playbackLoop();

		}

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

	}
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
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

/* USER CODE BEGIN 4 */

/**
 * @brief  Perform the SDRAM exernal memory inialization sequence
 * @param  hsdram: SDRAM handle
 * @param  Command: Pointer to SDRAM command structure
 * @retval None
 */
void initTapTone() {
	int i;
	for (i = 0; i < 100; i++) {
		taptone_buffer[i] = i;
	}
}

void triggerHandler(){


}

void buttonHandler() {
	if(HAL_GPIO_ReadPin(Recording_GPIO_Port,Recording_Pin) == GPIO_PIN_SET) {
		RecordingButton = RELEASE;

	}
	if(HAL_GPIO_ReadPin(Playback_GPIO_Port,Playback_Pin) == GPIO_PIN_SET) {
		PlaybackButton = RELEASE;

		}
//	else
//		RecordingPressed = RELEASE;
//	if (HAL_GPIO_ReadPin(Dubbing_On_GPIO_Port, Dubbing_On_Pin)	== GPIO_PIN_RESET) {
//		DubbingPressed = PRESS;
//
//	}
//	else
//		DubbingPressed = RELEASE;


}
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
		//TapToneBufferCount--;

		//if(TapToneBufferCount == 0)
		//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);

		//if(htim->Instance == TIM3){

//	}

//}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
			/* User can add his own implementation to report the HAL error return state */
			while (1) {
			}
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
			/* User can add his own implementation to report the file name and line number,
			 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

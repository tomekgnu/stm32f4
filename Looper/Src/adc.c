/**
  ******************************************************************************
  * File Name          : ADC.c
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
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
#include "adc.h"

#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "stdlib.h"
#include "main.h"
#include "audio.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_sdram.h"
#include "math.h"
#include "tm_stm32_hd44780.h"
#include "midi.h"

#define pi 3.14159
extern __IO ButtonStates ToggleDubbing;
extern uint32_t read_pointer;
extern uint32_t DataReady;
extern __IO uint32_t BufferCount;
extern uint16_t * rdptr;
extern uint16_t * wrptr;
extern __IO uint8_t Playback;
extern __IO uint8_t Recording;
extern __IO uint8_t Dubbing;
extern uint32_t SamplesRead;
extern uint32_t SamplesWritten;
extern __IO uint32_t CurrentSize;
extern __IO uint8_t ToggleRecording;
extern uint16_t readADC[];
extern __IO uint8_t StartApp;
extern uint8_t key_to_drum[];
struct tracks trcs;
uint8_t currentLoop;
uint8_t tracksPlaying;
__IO uint8_t conversions = 0;

uint32_t adcval;
char strval[5];

uint32_t key_ticks_button_up = 0;
uint32_t key_ticks_button_down = 0;

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef * hadc){
	utoa(hadc->ErrorCode,strval,10);
	TM_HD44780_Puts(0,1,strval);
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){

		key_ticks_button_down = HAL_GetTick();
		adcval = HAL_ADC_GetValue(hadc);
//		if(key_ticks_button_down - key_ticks_button_up < 250){
//			return;
//		}
		//if(StartApp == 0){
		//

		switch(adcval){
		case 0:
		case 1:
		case 2:
		case 3:adcval = 1;
				break;
		case 9:
		case 10:
		case 11:
		case 12:adcval = 2;
				break;
		case 18:
		case 19:
		case 20:
		case 21:adcval = 3;
				break;
		case 25:
		case 26:
		case 27:
		case 28:adcval = 4;
				break;
		case 31:
		case 32:
		case 33:
		case 34:adcval = 5;
				break;
		case 35:
		case 36:
		case 37:
		case 38:adcval = 6;
				break;
		case 40:
		case 41:
		case 42:adcval = 7;
				break;
		case 44:
		case 45:
		case 46:adcval = 8;
				break;
		case 47:
		case 48:
		case 49:adcval = 9;
				break;
		case 50:
		case 51:
		case 52:adcval = 10;
				break;
		case 53:
		case 54:adcval = 11;
				break;
		case 55:
		case 56:adcval = 12;
				break;
		case 57:
		case 58:adcval = 13;
				break;
		case 59:
		case 60:adcval = 14;
				break;
		case 61: adcval = 15;
				break;
		case 62: adcval = 16;
				Dubbing = !Dubbing;
				break;
		default: return;

		}
		TM_HD44780_Clear();
		utoa(adcval,strval,10);
		TM_HD44780_Puts(0,0,strval);
		playPercussion(NOTEON,key_to_drum[adcval - 1]);
}

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_6B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_Ext_IT11;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();
  
    /**ADC1 GPIO Configuration    
    PA4     ------> ADC1_IN4 
    */
    GPIO_InitStruct.Pin = AD_KBD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(AD_KBD_GPIO_Port, &GPIO_InitStruct);

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
  /* USER CODE BEGIN ADC1_MspInit 1 */
    HAL_GPIO_WritePin(AD_KBD_GPIO_Port,AD_KBD_Pin,GPIO_PIN_RESET);
  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();
  
    /**ADC1 GPIO Configuration    
    PA4     ------> ADC1_IN4 
    */
    HAL_GPIO_DeInit(AD_KBD_GPIO_Port, AD_KBD_Pin);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(ADC_IRQn);

  }
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
} 

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

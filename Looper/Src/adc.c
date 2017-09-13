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
#include "main.h"
#include "audio.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_sdram.h"
#include "math.h"
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

struct tracks trcs;
uint8_t currentLoop;
uint8_t tracksPlaying;
__IO uint8_t conversions = 0;

uint32_t adcval;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){


	if(StartApp == 0){
		adcval = HAL_ADC_GetValue(hadc);
		//HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,readADC[0]);
		return;
	}

	if(Recording == 1)
		recordMulti(TRACK1,readADC[0],readADC[1],&trcs);
	if(Playback == 1){
		Dubbing = ToggleDubbing;
		if(Dubbing == 1){
			trcs.samples[currentLoop] = readADC[0];
			trcs.sum = trcs.samples[TRACK1] + trcs.samples[TRACK2] + trcs.samples[TRACK3] + trcs.samples[TRACK4];
			BSP_SDRAM_WriteData(SDRAM_DEVICE_ADDR + read_pointer,(uint32_t *) &trcs, 3);
		}
		SamplesRead++;
		if(SamplesRead == SamplesWritten){
			if(Dubbing == 1){
				tracksPlaying++;
				currentLoop++;
			}
			if(tracksPlaying == 5)
				tracksPlaying = 4;
			if(currentLoop == 4)
				currentLoop = 0;
				SamplesRead = 0;
				read_pointer = 0;
				return;
			}

			read_pointer += 12;
			if(read_pointer == SDRAM_SIZE)
				read_pointer = 0;

	}

}
//
//	//if(StartApp == 0)
//		//return;
//	if(hadc->Instance == ADC1){
//
//		//HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,readADC);
////		if(Recording == 1)
////			record((uint16_t)readADC);
////		if(Playback == 1)
////			play((uint16_t)readADC);
//		//readADC = HAL_ADC_GetValue(hadc);
//		//HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,readADC);
//		//BSP_LED_On(RED);
//		//BSP_LED_Off(RED);
//	}
//	if(hadc->Instance == ADC2){
//
//		//HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,readADC);
//		//BSP_LED_On(GREEN);
//		//BSP_LED_Off(GREEN);
//	}
//	//BSP_LED_Off(RED);
//}
/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
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
  sConfig.SamplingTime = ADC_SAMPLETIME_112CYCLES;
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

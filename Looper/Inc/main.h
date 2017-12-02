/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "stm32f4xx.h"
#include "stdint.h"

#define SAMPLE_SIZE			4
#define SAMPLE_ARRAY		1024
#define SAMPLE_BYTES		SAMPLE_ARRAY * SAMPLE_SIZE
#define READ_SIZE			((SAMPLE_BYTES) / 4)
#define WRITE_SIZE			((SAMPLE_BYTES) / 4)
#define MEM_BLOCK			READ_SIZE

typedef enum {FALSE,TRUE} BOOL;
typedef enum{BUT_UP,BUT_DOWN} BUTTON ;
typedef uint8_t byte;

void play_record();
void lowerMixedSamples(uint32_t * buf);
void buttonHandler();
uint16_t drumHandler();
void SystemClock_Config(void);
void Error_Handler(void);

extern __IO BOOL Recording;
extern __IO BOOL Playback;
extern __IO BOOL Overdubbing;
extern __IO BOOL ToggleFunction;
extern __IO BOOL StartApp;
extern __IO BOOL clipping;
extern uint8_t footswitch;

#define delayUS_ASM(us) do {\
	asm volatile (	"MOV R0,%[loops]\n\t"\
			"1: \n\t"\
			"SUB R0, #1\n\t"\
			"CMP R0, #0\n\t"\
			"BNE 1b \n\t" : : [loops] "r" (16*us) : "memory"\
		      );\
} while(0)
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define SF3_CS_Pin GPIO_PIN_2
#define SF3_CS_GPIO_Port GPIOE
#define VS1053_RESET_Pin GPIO_PIN_3
#define VS1053_RESET_GPIO_Port GPIOE
#define KEYPAD_COLUMN_1_Pin GPIO_PIN_4
#define KEYPAD_COLUMN_1_GPIO_Port GPIOE
#define KEYPAD_COLUMN_2_Pin GPIO_PIN_5
#define KEYPAD_COLUMN_2_GPIO_Port GPIOE
#define KEYPAD_COLUMN_3_Pin GPIO_PIN_6
#define KEYPAD_COLUMN_3_GPIO_Port GPIOE
#define KEYPAD_COLUMN_4_Pin GPIO_PIN_13
#define KEYPAD_COLUMN_4_GPIO_Port GPIOC
#define PC14_OSC32_IN_Pin GPIO_PIN_14
#define PC14_OSC32_IN_GPIO_Port GPIOC
#define PC15_OSC32_OUT_Pin GPIO_PIN_15
#define PC15_OSC32_OUT_GPIO_Port GPIOC
#define A0_Pin GPIO_PIN_0
#define A0_GPIO_Port GPIOF
#define A1_Pin GPIO_PIN_1
#define A1_GPIO_Port GPIOF
#define A2_Pin GPIO_PIN_2
#define A2_GPIO_Port GPIOF
#define A3_Pin GPIO_PIN_3
#define A3_GPIO_Port GPIOF
#define A4_Pin GPIO_PIN_4
#define A4_GPIO_Port GPIOF
#define A5_Pin GPIO_PIN_5
#define A5_GPIO_Port GPIOF
#define Recording_Pin GPIO_PIN_6
#define Recording_GPIO_Port GPIOF
#define Recording_EXTI_IRQn EXTI9_5_IRQn
#define KEYPAD_ROW_1_Pin GPIO_PIN_7
#define KEYPAD_ROW_1_GPIO_Port GPIOF
#define KEYPAD_ROW_2_Pin GPIO_PIN_8
#define KEYPAD_ROW_2_GPIO_Port GPIOF
#define KEYPAD_ROW_3_Pin GPIO_PIN_9
#define KEYPAD_ROW_3_GPIO_Port GPIOF
#define AD_KBD_BUT_DOWN_Pin GPIO_PIN_10
#define AD_KBD_BUT_DOWN_GPIO_Port GPIOF
#define AD_KBD_BUT_DOWN_EXTI_IRQn EXTI15_10_IRQn
#define PH0_OSC_IN_Pin GPIO_PIN_0
#define PH0_OSC_IN_GPIO_Port GPIOH
#define PH1_OSC_OUT_Pin GPIO_PIN_1
#define PH1_OSC_OUT_GPIO_Port GPIOH
#define SDNWE_Pin GPIO_PIN_0
#define SDNWE_GPIO_Port GPIOC
#define SPI2_MISO_Pin GPIO_PIN_2
#define SPI2_MISO_GPIO_Port GPIOC
#define SPI2_MOSI_Pin GPIO_PIN_3
#define SPI2_MOSI_GPIO_Port GPIOC
#define B5_Pin GPIO_PIN_3
#define B5_GPIO_Port GPIOA
#define AD_KBD_Pin GPIO_PIN_4
#define AD_KBD_GPIO_Port GPIOA
#define G2_Pin GPIO_PIN_6
#define G2_GPIO_Port GPIOA
#define ACP_RST_Pin GPIO_PIN_7
#define ACP_RST_GPIO_Port GPIOA
#define OTG_FS_PSO_Pin GPIO_PIN_4
#define OTG_FS_PSO_GPIO_Port GPIOC
#define OTG_FS_OC_Pin GPIO_PIN_5
#define OTG_FS_OC_GPIO_Port GPIOC
#define OTG_FS_OC_EXTI_IRQn EXTI9_5_IRQn
#define R3_Pin GPIO_PIN_0
#define R3_GPIO_Port GPIOB
#define R6_Pin GPIO_PIN_1
#define R6_GPIO_Port GPIOB
#define BOOT1_Pin GPIO_PIN_2
#define BOOT1_GPIO_Port GPIOB
#define SDNRAS_Pin GPIO_PIN_11
#define SDNRAS_GPIO_Port GPIOF
#define A6_Pin GPIO_PIN_12
#define A6_GPIO_Port GPIOF
#define A7_Pin GPIO_PIN_13
#define A7_GPIO_Port GPIOF
#define A8_Pin GPIO_PIN_14
#define A8_GPIO_Port GPIOF
#define A9_Pin GPIO_PIN_15
#define A9_GPIO_Port GPIOF
#define A10_Pin GPIO_PIN_0
#define A10_GPIO_Port GPIOG
#define A11_Pin GPIO_PIN_1
#define A11_GPIO_Port GPIOG
#define D4_Pin GPIO_PIN_7
#define D4_GPIO_Port GPIOE
#define D5_Pin GPIO_PIN_8
#define D5_GPIO_Port GPIOE
#define D6_Pin GPIO_PIN_9
#define D6_GPIO_Port GPIOE
#define D7_Pin GPIO_PIN_10
#define D7_GPIO_Port GPIOE
#define D8_Pin GPIO_PIN_11
#define D8_GPIO_Port GPIOE
#define D9_Pin GPIO_PIN_12
#define D9_GPIO_Port GPIOE
#define D10_Pin GPIO_PIN_13
#define D10_GPIO_Port GPIOE
#define D11_Pin GPIO_PIN_14
#define D11_GPIO_Port GPIOE
#define D12_Pin GPIO_PIN_15
#define D12_GPIO_Port GPIOE
#define SPI2_SCK_Pin GPIO_PIN_10
#define SPI2_SCK_GPIO_Port GPIOB
#define G5_Pin GPIO_PIN_11
#define G5_GPIO_Port GPIOB
#define OTG_FS_ID_Pin GPIO_PIN_12
#define OTG_FS_ID_GPIO_Port GPIOB
#define VBUS_FS_Pin GPIO_PIN_13
#define VBUS_FS_GPIO_Port GPIOB
#define OTG_FS_DM_Pin GPIO_PIN_14
#define OTG_FS_DM_GPIO_Port GPIOB
#define OTG_FS_DP_Pin GPIO_PIN_15
#define OTG_FS_DP_GPIO_Port GPIOB
#define D13_Pin GPIO_PIN_8
#define D13_GPIO_Port GPIOD
#define D14_Pin GPIO_PIN_9
#define D14_GPIO_Port GPIOD
#define D15_Pin GPIO_PIN_10
#define D15_GPIO_Port GPIOD
#define TE_Pin GPIO_PIN_11
#define TE_GPIO_Port GPIOD
#define RDX_Pin GPIO_PIN_12
#define RDX_GPIO_Port GPIOD
#define WRX_DCX_Pin GPIO_PIN_13
#define WRX_DCX_GPIO_Port GPIOD
#define D0_Pin GPIO_PIN_14
#define D0_GPIO_Port GPIOD
#define D1_Pin GPIO_PIN_15
#define D1_GPIO_Port GPIOD
#define BA0_Pin GPIO_PIN_4
#define BA0_GPIO_Port GPIOG
#define BA1_Pin GPIO_PIN_5
#define BA1_GPIO_Port GPIOG
#define R7_Pin GPIO_PIN_6
#define R7_GPIO_Port GPIOG
#define HD44780_D7_Pin GPIO_PIN_7
#define HD44780_D7_GPIO_Port GPIOG
#define SDCLK_Pin GPIO_PIN_8
#define SDCLK_GPIO_Port GPIOG
#define HD44780_D6_Pin GPIO_PIN_6
#define HD44780_D6_GPIO_Port GPIOC
#define HD44780_D5_Pin GPIO_PIN_7
#define HD44780_D5_GPIO_Port GPIOC
#define HD44780_D4_Pin GPIO_PIN_8
#define HD44780_D4_GPIO_Port GPIOC
#define HD44780_E_Pin GPIO_PIN_9
#define HD44780_E_GPIO_Port GPIOC
#define HD44780_RS_Pin GPIO_PIN_8
#define HD44780_RS_GPIO_Port GPIOA
#define STLINK_RX_Pin GPIO_PIN_9
#define STLINK_RX_GPIO_Port GPIOA
#define STLINK_TX_Pin GPIO_PIN_10
#define STLINK_TX_GPIO_Port GPIOA
#define R4_Pin GPIO_PIN_11
#define R4_GPIO_Port GPIOA
#define R5_Pin GPIO_PIN_12
#define R5_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define TP_INT1_Pin GPIO_PIN_15
#define TP_INT1_GPIO_Port GPIOA
#define TP_INT1_EXTI_IRQn EXTI15_10_IRQn
#define ADS1256_CS_Pin GPIO_PIN_10
#define ADS1256_CS_GPIO_Port GPIOC
#define ADS1256_MISO_Pin GPIO_PIN_11
#define ADS1256_MISO_GPIO_Port GPIOC
#define ADS1256_MOSI_Pin GPIO_PIN_12
#define ADS1256_MOSI_GPIO_Port GPIOC
#define ADS1256_DRDY_Pin GPIO_PIN_2
#define ADS1256_DRDY_GPIO_Port GPIOD
#define ADS1256_DRDY_EXTI_IRQn EXTI2_IRQn
#define ADS1256_SYNC_Pin GPIO_PIN_3
#define ADS1256_SYNC_GPIO_Port GPIOD
#define ADS1256_RESET_Pin GPIO_PIN_4
#define ADS1256_RESET_GPIO_Port GPIOD
#define DAC8552_CS_Pin GPIO_PIN_5
#define DAC8552_CS_GPIO_Port GPIOD
#define B2_Pin GPIO_PIN_6
#define B2_GPIO_Port GPIOD
#define Overdubbing_Pin GPIO_PIN_7
#define Overdubbing_GPIO_Port GPIOD
#define Overdubbing_EXTI_IRQn EXTI9_5_IRQn
#define Playback_Pin GPIO_PIN_9
#define Playback_GPIO_Port GPIOG
#define Playback_EXTI_IRQn EXTI9_5_IRQn
#define AD_KBD_INT_Pin GPIO_PIN_11
#define AD_KBD_INT_GPIO_Port GPIOG
#define AD_KBD_INT_EXTI_IRQn EXTI15_10_IRQn
#define ADC3_Trigger_Pin GPIO_PIN_12
#define ADC3_Trigger_GPIO_Port GPIOG
#define LD3_Pin GPIO_PIN_13
#define LD3_GPIO_Port GPIOG
#define LD4_Pin GPIO_PIN_14
#define LD4_GPIO_Port GPIOG
#define SDNCAS_Pin GPIO_PIN_15
#define SDNCAS_GPIO_Port GPIOG
#define ADS1256_SCK_Pin GPIO_PIN_3
#define ADS1256_SCK_GPIO_Port GPIOB
#define ToggleFunction_Pin GPIO_PIN_4
#define ToggleFunction_GPIO_Port GPIOB
#define ToggleFunction_EXTI_IRQn EXTI4_IRQn
#define SDCKE1_Pin GPIO_PIN_5
#define SDCKE1_GPIO_Port GPIOB
#define SDNE1_Pin GPIO_PIN_6
#define SDNE1_GPIO_Port GPIOB
#define KEYPAD_ROW_4_Pin GPIO_PIN_9
#define KEYPAD_ROW_4_GPIO_Port GPIOB
#define NBL0_Pin GPIO_PIN_0
#define NBL0_GPIO_Port GPIOE
#define NBL1_Pin GPIO_PIN_1
#define NBL1_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */
#define SDRAM_SIZE			((uint32_t)0x800000)

typedef enum
{
  LED_GREEN = 0,
  LED_RED = 1
}Led_Colors;

#define BUT_REC				1
#define BUT_PLAY			2
#define BUT_OVERDUB			4
#define BUT_TOGFUN			8
#define BUT_DOWN(BUT)		footswitch |= BUT
#define BUT_UP(BUT)			footswitch ^= BUT
#define IS_BUT_DOWN(BUT)	((footswitch & BUT) > 0)
/* USER CODE END Private defines */

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

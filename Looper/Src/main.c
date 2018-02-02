/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
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
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* USER CODE BEGIN Includes */
#include "tm_stm32_hd44780.h"
#include "ads1256_test.h"
#include "tm_stm32f4_keypad.h"
#include "tm_stm32f4_ili9341.h"
#include "stm32f429i_discovery_sdram.h"
#include "main.h"
#include "midi.h"
#include "menu.h"
#include "drums.h"
#include "string.h"
#include "math.h"
#include "drums.h"
#include "stdlib.h"
#include "audio.h"
#include "SF3.h"
#include "spi_flash.h"
#include "spiffs.h"
#include "SRAMDriver.h"
#include "tm_stm32f4_fatfs.h"
#define pi 3.14159
#define LOG_PAGE_SIZE       256
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

__IO BOOL Recording = FALSE;
__IO BOOL Playback = FALSE;
__IO BOOL Overdubbing = FALSE;
__IO BOOL StartLooper = FALSE;
__IO DrumFunction DrumState = DRUM_STOP;
uint8_t footswitch = 0;


extern __IO CHANNEL ch1;
extern __IO CHANNEL ch2;
extern uint32_t adc1val;
extern BOOL ShowBeat;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

static void my_spiffs_mount();


/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
static u8_t spiffs_work_buf[LOG_PAGE_SIZE*2];
static u8_t spiffs_fds[32*4];
static u8_t spiffs_cache_buf[(LOG_PAGE_SIZE+32)*4];
static spiffs fs;

TM_KEYPAD_Button_t Keypad_Button;

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	extern uint16_t drumTracks[4][2][16];
	uint32_t data;
	char lcdline[30];
	uint32_t SF3ID;
	HAL_StatusTypeDef status;
	spiffs_file fd1;
	//Fatfs object
	    FATFS FatFs;
	    //File object
	    FIL fil;
	    //Free and total space
	    uint32_t total, free;
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;


  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  ch1.Active = TRUE;
  ch1.Monitor = TRUE;
  ch2.Active = FALSE;
  ch2.Monitor = TRUE;
  ch1.Number = ONE;
  ch2.Number = TWO;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_FMC_Init();
  MX_TIM4_Init();
  MX_SPI3_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_SPI5_Init();

  /* USER CODE BEGIN 2 */

  sFLASH_Init();
  my_spiffs_mount();
  BSP_SDRAM_Init();
  HAL_NVIC_DisableIRQ(EXTI2_IRQn);
  //HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);
  TM_ILI9341_Init();
  //Rotate LCD for 90 degrees
  TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_1);
  //FIll lcd with color
  TM_ILI9341_Fill(ILI9341_COLOR_MAGENTA);

  status = HAL_TIM_Base_Start_IT(&htim2);

  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

  status = HAL_TIM_Base_Start_IT(&htim4);
  status = HAL_ADC_Start_IT(&hadc1);

  ADS1256_WriteCmd(CMD_RESET);
  ADS1256_WriteCmd(CMD_SDATAC);
  data = ADS1256_ReadChipID();


  ADS1256_CfgADC(ADS1256_GAIN_2, ADS1256_15000SPS);
  ADS1256_SetDiffChannel(0);
  ADS1256_WriteCmd(CMD_SELFCAL);

  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
  ADS1256_WriteCmd(CMD_RDATAC);
  //FATFS_UnLinkDriver(SD_Path);
  TM_KEYPAD_Init();
  setupMidi();
  memset(drumTracks,0,sizeof(drumTracks));
  initDrumBeats();
  talkMIDI(0xB0, 0, 0x01); //Default bank GM1

  TM_HD44780_Init(20, 4);
  TM_HD44780_Clear();
  TM_HD44780_Puts(0,0,"Looper");

  InitSRAM();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {

	  if(DrumState == DRUM_EDIT && adc1val > 0){
		  placeDrumSymbol(adc1val);
		  adc1val = 0;
	  }
	  Keypad_Button = TM_KEYPAD_Read();
	          /* Keypad was pressed */
	          if (Keypad_Button != TM_KEYPAD_Button_NOPRESSED) {/* Keypad is pressed */
	        	  switch (Keypad_Button) {
	                  case TM_KEYPAD_Button_0:        /* Button 0 pressed */
	                	  if(DrumState == DRUM_EDIT){
	                		  clearDrumSymbol();
	                		  break;
	                	  }
	                	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);
	                	  SF3ID = sFLASH_ReadID();
	                	  sprintf(lcdline,"SF3 manufacturer: 0x%x",(unsigned int)SF3ID);
	                	  TM_ILI9341_Puts(0, 24, lcdline, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	                	  TM_ILI9341_Puts(0, 5, (data == 3?"ADS1256 OK\n":"ADS1256 failure\n"), &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	                	  if(StartLooper == FALSE){
	                		  StartLooper = TRUE;
	                		  TM_HD44780_Puts(7,0,"started");
	                	  }
	                	  else{
	                		  StartLooper = FALSE;
	                		  TM_HD44780_Puts(7,0,"stopped");
	                	  }
	                	  break;
	                  case TM_KEYPAD_Button_1:        /* Button 1 pressed */
	                	  fd1 = SPIFFS_open(&fs, "my_file", SPIFFS_RDWR, 0);
	                	  data = SPIFFS_errno(&fs);
	                	  if(data == 0)
	                		  TM_HD44780_Puts(0,1,"Hello world");
	                	  SPIFFS_close(&fs, fd1);
	                	  break;
	                  case TM_KEYPAD_Button_2:        /* Button 2 pressed */
	                	  SRAMWriteByte(0,0,0,'c');
						  data = SRAMReadByte(0,0,0);
						  break;
	                  case TM_KEYPAD_Button_3:        /* Button 3 pressed */
	                	  if (f_mount(&FatFs, "", 1) == FR_OK) {
	                	         //Mounted OK, turn on RED LED
	                	         BSP_LED_On(LED_RED);

	                	         //Try to open file
	                	         if (f_open(&fil, "1stfile.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK) {
	                	             //File opened, turn off RED and turn on GREEN led
	                	        	 BSP_LED_On(LED_GREEN);
	                	        	 BSP_LED_Off(LED_RED);

	                	             //If we put more than 0 characters (everything OK)
	                	             if (f_puts("First string in my file\n", &fil) > 0) {
	                	                 if (TM_FATFS_DriveSize(&total, &free) == FR_OK) {
	                	                     //Data for drive size are valid
	                	                 }

	                	                 //Turn on both leds
	                	                 BSP_LED_On(LED_GREEN | LED_RED);
	                	             }

	                	             //Close file, don't forget this!
	                	             f_close(&fil);
	                	         }

	                	         //Unmount drive, don't forget this!
	                	         f_mount(0, "", 1);
	                	     }
	                	   	  break;
	                  case TM_KEYPAD_Button_4:        /* Button 4 pressed */
	                	  	  moveBeatBack();
	                	  	  break;
	                  case TM_KEYPAD_Button_5:        /* Button 5 pressed */
	                  case TM_KEYPAD_Button_6:        /* Button 6 pressed */
	                	  	  moveBeatForward();
	                	  	  break;
	                  case TM_KEYPAD_Button_7:        /* Button 7 pressed */
	                  case TM_KEYPAD_Button_8:        /* Button 8 pressed */
	                  case TM_KEYPAD_Button_9:        /* Button 9 pressed */
	                	  sprintf(lcdline,"Button %d pressed",Keypad_Button);
	                	  TM_ILI9341_Puts(5, 100, lcdline, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	                	  break;
	                  case TM_KEYPAD_Button_STAR:        /* Button STAR pressed */
							  switch(DrumState){
							  	 case DRUM_EDIT: resetDrums();
											  	 DrumState = DRUM_START;
											  	 break;
							  	 case DRUM_START: DrumState = DRUM_STOP;
											  	  break;
							  	 case DRUM_STOP: DrumState = DRUM_EDIT;
							  	 	 	 	 	 adc1val = 0;
							  	 	 	 	 	 menuDrumEdit();
											  	 break;
							  }
							  break;
	                  case TM_KEYPAD_Button_HASH:        /* Button HASH pressed */
	                	  break;
	                  case TM_KEYPAD_Button_A:        /* Button A pressed, only on large keyboard */
	                	 if(DrumState == DRUM_EDIT)
	                		 setDrumPart(R_HAND);
	                	 break;
	                  case TM_KEYPAD_Button_B:        /* Button B pressed, only on large keyboard */
	                	  if(DrumState == DRUM_EDIT)
	                	  	setDrumPart(L_HAND);
	                      break;
	                  case TM_KEYPAD_Button_C:        /* Button C pressed, only on large keyboard */
	                	  if(DrumState == DRUM_EDIT)
	                		  setDrumPart(R_FOOT);
	                      break;
	                  case TM_KEYPAD_Button_D:        /* Button D pressed, only on large keyboard */
	                	  if(DrumState == DRUM_EDIT)
	                		  setDrumPart(L_FOOT);
	                      break;
	                  default:
	                      break;
	              } // end of switch

	        	  }// end of key pressed



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
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
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

/* USER CODE BEGIN 4 */
static s32_t my_spiffs_read(u32_t addr, u32_t size, u8_t *dst) {
    sFLASH_ReadBuffer(dst,addr,size);
    return SPIFFS_OK;
  }

  static s32_t my_spiffs_write(u32_t addr, u32_t size, u8_t *src) {
	sFLASH_WriteBuffer(src,addr,size);
    return SPIFFS_OK;
  }

  static s32_t my_spiffs_erase(u32_t addr, u32_t size) {

    return sFLASH_Erase(addr, size);
  }

void my_spiffs_mount() {
    spiffs_config cfg;
    cfg.phys_size = N25Q256A_FLASH_SIZE; // use all spi flash
    cfg.phys_addr = 0; // start spiffs at start of spi flash
    cfg.phys_erase_block = N25Q256A_SECTOR_SIZE; // according to datasheet
    cfg.log_block_size = N25Q256A_SECTOR_SIZE; // let us not complicate things
    cfg.log_page_size = N25Q256A_PAGE_SIZE; // as we said

    cfg.hal_read_f = my_spiffs_read;
    cfg.hal_write_f = my_spiffs_write;
    cfg.hal_erase_f = my_spiffs_erase;

    int res = SPIFFS_mount(&fs,
      &cfg,
      spiffs_work_buf,
      spiffs_fds,
      sizeof(spiffs_fds),
      spiffs_cache_buf,
      sizeof(spiffs_cache_buf),
      0);
    printf("mount res: %i\n", res);
  }
/**
 * @brief  Perform the SDRAM exernal memory inialization sequence
 * @param  hsdram: SDRAM handle
 * @param  Command: Pointer to SDRAM command structure
 * @retval None
 */


void buttonHandler() {
	if(IS_BUT_DOWN(BUT_REC) && HAL_GPIO_ReadPin(Recording_GPIO_Port,Recording_Pin) == GPIO_PIN_SET )
		BUT_UP(BUT_REC);

	if(IS_BUT_DOWN(BUT_PLAY) && HAL_GPIO_ReadPin(Playback_GPIO_Port,Playback_Pin) == GPIO_PIN_SET )
		BUT_UP(BUT_PLAY);

	if(IS_BUT_DOWN(BUT_OVERDUB) && HAL_GPIO_ReadPin(Overdubbing_GPIO_Port,Overdubbing_Pin) == GPIO_PIN_SET )
		BUT_UP(BUT_OVERDUB);

	if(IS_BUT_DOWN(BUT_TOGFUN) && HAL_GPIO_ReadPin(ToggleFunction_GPIO_Port,ToggleFunction_Pin) == GPIO_PIN_SET )
		BUT_UP(BUT_TOGFUN);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
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

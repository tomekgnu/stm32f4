#include "main.h"
#include "stm32f429i_discovery.h"
#include "ff.h"
#include "waveplayer.h"
#include "drums.h"

struct drumStruct drumset;

int16_t snare[SNARE_SAMPLES];
int16_t kick[KICK_SAMPLES];
int16_t tomhi[TOM_HI_SAMPLES];
int16_t tomlo[TOM_LO_SAMPLES];
int16_t crash[CRASH_SAMPLES];
int16_t hihat[HIHAT_SAMPLES];

uint16_t snareIndex = 0;
uint16_t kickIndex = 0;
uint16_t tomhiIndex = 0;
uint16_t tomloIndex = 0;
uint16_t crashIndex = 0;
uint16_t hihatIndex = 0;
uint8_t numberOfDrums = 0;
uint8_t *pDrumset;
extern char SD_Path[];
extern uint8_t retSD;
DIR Directory;
FIL snareFile;
FIL kickFile;
FIL crashFile;
FIL tomhiFile;
FIL tomloFile;
FIL hihatFile;

static inline uint16_t mixDrums(uint8_t snare, uint16_t kick,uint16_t crash,uint16_t tomlo,uint16_t tomhi, uint16_t hihat,uint8_t number) {
	if(number == 0)
		return 0;
	return ((snare + kick + tomlo + tomhi + crash + hihat) / number);
}


uint16_t drumHandler(){


	uint16_t snareSample = 0;
	uint16_t kickSample = 0;
	uint16_t crashSample = 0;
	uint16_t tomloSample = 0;
	uint16_t tomhiSample = 0;
	uint16_t hihatSample = 0;
	numberOfDrums = 0;

	if(HAL_GPIO_ReadPin(Snare_Button_GPIO_Port,Snare_Button_Pin) == GPIO_PIN_RESET){
		if(drumset.snare == 0){
			snareIndex = 0;
			drumset.snare = 1;
		}
	}
//	if(HAL_GPIO_ReadPin(Kick_Button_GPIO_Port,Kick_Button_Pin) == GPIO_PIN_RESET){
//			if(drumset.kick == 0){
//				kickIndex = 0;
//				drumset.kick = 1;
//			}
//
//		}
	if(HAL_GPIO_ReadPin(Crash_Button_GPIO_Port,Crash_Button_Pin) == GPIO_PIN_RESET){
			if(drumset.crash == 0){
				crashIndex = 0;
				drumset.crash = 1;
			}
		}
	if(HAL_GPIO_ReadPin(TomLo_Button_GPIO_Port,TomLo_Button_Pin) == GPIO_PIN_RESET){
			if(drumset.tomlo == 0){
				tomloIndex = 0;
				drumset.tomlo = 1;
			}
	}
	if(HAL_GPIO_ReadPin(TomHi_Button_GPIO_Port,TomHi_Button_Pin) == GPIO_PIN_RESET){
		if(drumset.tomhi == 0){
			tomhiIndex = 0;
			drumset.tomhi = 1;
		}
	}
	if(HAL_GPIO_ReadPin(HiHat_Button_GPIO_Port,HiHat_Button_Pin) == GPIO_PIN_RESET){
			if(drumset.hihat == 0){
				hihatIndex = 0;
				drumset.hihat = 1;
			}
		}

	if(drumset.snare == 1){
		snareSample = snare[snareIndex++] + 32768;
		numberOfDrums++;
	}
	if(drumset.kick == 1){
		kickSample = kick[kickIndex++] + 32768;
		numberOfDrums++;
	}
	if(drumset.crash == 1){
		crashSample = crash[crashIndex++] + 32768;
		numberOfDrums++;
	}
	if(drumset.tomlo == 1){
		tomloSample = tomlo[tomloIndex++] + 32768;
		numberOfDrums++;
	}
	if(drumset.tomhi == 1){
		tomhiSample = tomhi[tomhiIndex++] + 32768;
		numberOfDrums++;
	}
	if(drumset.hihat == 1){
		hihatSample = hihat[hihatIndex++] + 32768;
		numberOfDrums++;
	}


	//mixed = mixDrums(snareSample,kickSample,crashSample,tomloSample,tomhiSample,hihatSample,numberOfDrums);
	if(snareIndex >= SNARE_SAMPLES){
		snareIndex = 0;
		drumset.snare = 0;

	}
	if(kickIndex >= KICK_SAMPLES){
		kickIndex = 0;
		drumset.kick = 0;
	}
	if(crashIndex >= CRASH_SAMPLES){
		crashIndex = 0;
		drumset.crash = 0;

	}
	if(tomloIndex >= TOM_LO_SAMPLES){
		tomloIndex = 0;
		drumset.tomlo = 0;

	}
	if(tomhiIndex >= TOM_HI_SAMPLES){
		tomhiIndex = 0;
		drumset.tomhi = 0;

	}
	if(hihatIndex >= HIHAT_SAMPLES){
		hihatIndex = 0;
		drumset.hihat = 0;

	}

	if(*pDrumset > 0)
		return  mixDrums(snareSample,kickSample,crashSample,tomloSample,tomhiSample,hihatSample,numberOfDrums);
	return 0;
}

void ReadDrumSamples(void)
{
  FATFS mynewdiskFatFs;
  UINT wbytes;
  /* File write counts */

  /* File system object for User logical drive */

  if(retSD == 0)
    {
      if(f_mount(&mynewdiskFatFs, (TCHAR const*)SD_Path, 0) != FR_OK)
      {
    	  Error_Handler();

       }
     }

  /* Get the read out protection status */
  if(f_opendir(&Directory, SD_Path) != FR_OK)
  {
	  BSP_LED_On(RED);
	  Error_Handler();
   }
    /* Open drum sample files */
    if(f_open(&kickFile, "kick.raw" , FA_READ) != FR_OK)
    {
      BSP_LED_On(RED);
      Error_Handler();
    }
    else
    {
      /* Read sizeof(WaveFormat) from the selected file */
      f_read (&kickFile, (uint8_t *)kick, sizeof(kick), &wbytes);
      f_close(&kickFile);

    }
    if(f_open(&snareFile, "snare.raw" , FA_READ) != FR_OK)
        {
          BSP_LED_On(RED);
          Error_Handler();
        }
        else
        {
          /* Read sizeof(WaveFormat) from the selected file */
          f_read (&snareFile, (uint8_t *)snare, sizeof(snare), &wbytes);
          f_close(&snareFile);
        }
    if(f_open(&hihatFile, "hihat.raw" , FA_READ) != FR_OK)
        {
          BSP_LED_On(RED);
          Error_Handler();
        }
        else
        {
          /* Read sizeof(WaveFormat) from the selected file */
          f_read (&hihatFile, (uint8_t *)hihat, sizeof(hihat), &wbytes);
          f_close(&hihatFile);
        }
    if(f_open(&crashFile, "crash.raw" , FA_READ) != FR_OK)
        {
          BSP_LED_On(RED);
          Error_Handler();
        }
        else
        {
          /* Read sizeof(WaveFormat) from the selected file */
          f_read (&crashFile, (uint8_t *)kick, sizeof(kick), &wbytes);
          f_close(&crashFile);

        }
    if(f_open(&tomloFile, "tomlo.raw" , FA_READ) != FR_OK)
        {
          BSP_LED_On(RED);
          Error_Handler();
        }
        else
        {
          /* Read sizeof(WaveFormat) from the selected file */
          f_read (&tomloFile, (uint8_t *)kick, sizeof(kick), &wbytes);
          f_close(&tomloFile);
        }
    	if(f_open(&tomhiFile, "tomhi.raw" , FA_READ) != FR_OK)
          {
            BSP_LED_On(RED);
            Error_Handler();
          }
          else
          {
            /* Read sizeof(WaveFormat) from the selected file */
            f_read (&tomhiFile, (uint8_t *)kick, sizeof(kick), &wbytes);
            f_close(&tomhiFile);
          }

  }


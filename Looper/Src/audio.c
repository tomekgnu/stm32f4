#include "main.h"
#include "audio.h"
#include "stm32f429i_discovery_sdram.h"
#include "ads1256_test.h"
#include "string.h"
#include "limits.h"
#include "waveplayer.h"
#include "math.h"
#include "tm_stm32_hd44780.h"
#include "stdlib.h"

#define USHORT_TO_DOUBLE(x)					((double)(x) / (double)(USHRT_MAX + 1))
#define DOUBLE_TO_USHORT(x)					(uint16_t)(x * ((double)(USHRT_MAX + 1)))



extern __IO uint32_t midiDrumClock;
extern __IO uint32_t midiDrumPointer;
uint32_t sdram_pointer = 0;
int16_t sample16s;

void record_sample(int16_t swrite,__IO CHANNEL *cha,__IO CHANNEL *chb){
	int16_t sread;
	if(StartApp == FALSE ){
		return;
	}
	if(cha->Active == TRUE){
		BSP_SDRAM_WriteData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *) &swrite, 1);
		BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer + 2,(uint16_t *)&sread,1);
		cha->SamplesWritten++;
		chb->CurrentSample = sread;
	}
	else{
		BSP_SDRAM_WriteData16b(SDRAM_DEVICE_ADDR + sdram_pointer  + 2,(uint16_t *) &swrite, 1);
		BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *)&sread,1);
		chb->SamplesWritten++;
		cha->CurrentSample = sread;
	}


	sdram_pointer += 4;
	if(sdram_pointer == SDRAM_SIZE){
		sdram_pointer = 0;
		cha->SamplesRead = 0;
		chb->SamplesRead = 0;
	}
}

void play_channels(__IO CHANNEL *cha,__IO CHANNEL *chb){
	if(cha->Monitor == TRUE && chb->Monitor == TRUE)
		Write_DAC8552_Both((uint16_t)(cha->CurrentSample + 16383),(uint16_t)(chb->CurrentSample + 16383));
	else if(cha->Monitor == TRUE)
		Write_DAC8552(channel_A,(uint16_t)(cha->CurrentSample + 16383));
	else if(chb->Monitor == TRUE)
		Write_DAC8552(channel_B,(uint16_t)(chb->CurrentSample + 16383));


}
void read_sample(int16_t swrite,__IO CHANNEL *cha,__IO CHANNEL *chb){
	__IO CHANNEL *cur;	// currently active channel
	int16_t sread[2];

	if(StartApp == FALSE ){
		return;
	}

	BSP_SDRAM_ReadData(SDRAM_DEVICE_ADDR + sdram_pointer,(uint32_t *) sread, 1);
	if(cha->Active == TRUE){
		cur = cha;
		cur->mix32tmp = sread[0]  + swrite;
		cur->CurrentSample = sread[0];
		chb->CurrentSample = sread[1];

	}
	else if(chb->Active == TRUE){
		cur = chb;
		cur->mix32tmp = sread[1]  + swrite;
		cur->CurrentSample = sread[1];
		cha->CurrentSample = sread[0];
	}
	else return;

	cur->SamplesRead++;
	if(cur->Clipping == TRUE)
		cur->mix32tmp = (int32_t)(cur->gain * (float)sread[0]  + cur->gain * (float)swrite);
	if(cur->mix32tmp > cur->mix32Max)
		cur->mix32Max = cur->mix32tmp;
	if(cur->Overdub == TRUE){
		if(cur->Number == ONE)
			BSP_SDRAM_WriteData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *) &cur->mix32tmp, 1);
		else if(cur->Number == TWO)
			BSP_SDRAM_WriteData16b(SDRAM_DEVICE_ADDR + sdram_pointer + 2,(uint16_t *) &cur->mix32tmp, 1);
	}
	if(cur->SamplesRead == cur->SamplesWritten){
		if(cur->Overdub == TRUE && cur->mix32Max > 16383){
			cur->Clipping = TRUE;
			cur->gain = 16383.00 / cur->mix32Max;
		}
		else{
			cur->Clipping = FALSE;
			cur->gain = 1.0;
		}

		cur->mix32Max = 16383;
		sdram_pointer = 0;
		cur->SamplesRead = 0;
		return;
		}

		sdram_pointer += 4;

	if(sdram_pointer == SDRAM_SIZE)
		sdram_pointer = 0;

}


void showMinMaxSamples(int32_t max,int32_t min){
	char minstr[10],maxstr[10];
	itoa(max,maxstr,10);
	itoa(min,minstr,10);

	TM_HD44780_Clear();
	TM_HD44780_Puts(0,0,maxstr);
	TM_HD44780_Puts(0,1,minstr);
}

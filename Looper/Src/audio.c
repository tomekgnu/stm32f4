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


uint32_t sampleADC;
int32_t sample32s;
int32_t sample32Min;
int32_t sample32Max;
int16_t sample16s;

uint32_t dub_pointer = 0;
uint32_t read_pointer = 0;
extern __IO uint32_t midiDrumClock;
extern __IO uint32_t midiDrumPointer;
uint32_t write_pointer = 0;
uint32_t SamplesWritten = 0;
uint32_t SamplesRead = 0;
uint32_t DataReady = 0;



__IO uint32_t CurrentSize;
__IO uint32_t CurrentBytes;
__IO uint8_t DmaTransferReady = 0;
__IO GPIO_PinState GuitarTrigger = 0;

uint16_t sample;
uint16_t newsample;

uint16_t upper;
uint16_t lower;
uint16_t mixed;
uint32_t upperlower;
float gain = 1.0;
uint32_t clipcnt = 0;
int32_t mix32Max = 32767;
int32_t sample32Min = 0;
int32_t sample32Max = 0;
int16_t sample16Max = 0;
int16_t sample16Min = 0;


void record32s(int16_t sample,FUNCTION ch){

	if(StartApp == 0 ){
		return;
	}

	if(ch == CH1)
		BSP_SDRAM_WriteData16b(SDRAM_DEVICE_ADDR + write_pointer,(uint16_t *) &sample, 1);
	else if(ch == CH2)
		BSP_SDRAM_WriteData16b(SDRAM_DEVICE_ADDR + write_pointer + 2,(uint16_t *) &sample, 1);
	else return;
	SamplesWritten++;
	write_pointer += 4;
	if(write_pointer == SDRAM_SIZE)
		write_pointer = 0;
}

int32_t mix32s;
uint32_t dacSample;


void play32s(int16_t newsample,FUNCTION ch){
	int16_t read16s;
	int16_t mix32tmp;

	if(ch == CH1)
		BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + read_pointer,(uint16_t *) &read16s, 1);
	else if(ch == CH2)
		BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + read_pointer + 2,(uint16_t *) &read16s, 1);

	mix32tmp = read16s  + newsample;
	if(clipping == TRUE){
		mix32tmp = (int32_t)(gain * (float)read16s  + gain * (float)newsample);
	}

	if(mix32tmp > mix32Max)
		mix32Max = mix32tmp;
	if(ch == CH1)
		Write_DAC8552(channel_A,(uint16_t)((float)read16s + 16383.00));
	else if(ch == CH2)
		Write_DAC8552(channel_B,(uint16_t)((float)read16s + 16383.00));
	else return;

	if(Overdubbing == TRUE){
		if(ch == CH1)
			BSP_SDRAM_WriteData16b(SDRAM_DEVICE_ADDR + read_pointer,(uint16_t *) &mix32tmp, 1);
		else if(ch == CH2)
			BSP_SDRAM_WriteData16b(SDRAM_DEVICE_ADDR + read_pointer + 2,(uint16_t *) &mix32tmp, 1);
		else return;
	}
	else{
		if(ch == CH1)
			BSP_SDRAM_WriteData16b(SDRAM_DEVICE_ADDR + read_pointer,(uint16_t *) &read16s, 1);
		else if(ch == CH2)
			BSP_SDRAM_WriteData16b(SDRAM_DEVICE_ADDR + read_pointer + 2,(uint16_t *) &read16s, 1);
		else return;
	}
	SamplesRead++;
	if(SamplesRead == SamplesWritten){
		if(Overdubbing == TRUE && mix32Max > 16383){
			clipping = TRUE;
			gain = 16383.00 / mix32Max;
		}
		else{
			clipping = FALSE;
			gain = 1.0;
		}

		mix32Max = 16383;
		dub_pointer = 0;
		SamplesRead = 0;
		read_pointer = 0;
		return;
	}
		dub_pointer += 4;
		read_pointer += 4;
		if(read_pointer == SDRAM_SIZE)
			read_pointer = 0;
		if(dub_pointer == SDRAM_SIZE)
			dub_pointer = 0;

}


void showMinMaxSamples(int32_t max,int32_t min){
	char minstr[10],maxstr[10];
	itoa(max,maxstr,10);
	itoa(min,minstr,10);

	TM_HD44780_Clear();
	TM_HD44780_Puts(0,0,maxstr);
	TM_HD44780_Puts(0,1,minstr);
}

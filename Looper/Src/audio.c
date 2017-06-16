#include "main.h"
#include "audio.h"
#include "stm32f429i_discovery_sdram.h"
#include "string.h"
#include "limits.h"
#include "waveplayer.h"
#define USHORT_TO_DOUBLE(x)					((double)(x) / (double)(USHRT_MAX + 1))
#define DOUBLE_TO_USHORT(x)					(uint16_t)(x * ((double)(USHRT_MAX + 1)))

uint32_t read_buffer_1[SAMPLE_ARRAY];
uint32_t read_buffer_2[SAMPLE_ARRAY];
uint32_t write_buffer_1[SAMPLE_ARRAY];
uint32_t write_buffer_2[SAMPLE_ARRAY];
uint32_t highest_sample_sum;
float mainfactor;
float tmpfactor;
extern FMC_SDRAM_CommandTypeDef SDRAMCommandStructure;
uint32_t * rdptr;
uint32_t * wrptr;	// pointer for writinguint32_t SamplesRead = 0;
uint32_t dub_pointer = 0;
uint32_t read_pointer = 0;
uint32_t write_pointer = 0;
uint32_t SamplesWritten = 0;
uint32_t SamplesRead = 0;
uint32_t DataReady = 0;
__IO uint8_t Recording = 0;
__IO uint8_t Playback = 0;
__IO uint8_t Dubbing = 0;
__IO uint8_t DubbingStarted = 0;
__IO uint8_t ToggleChannel = 0;
__IO uint8_t DrumsPlaying = 0;
extern struct drumStruct drumset;
__IO ButtonStates ToggleDubbing = UP;
__IO ButtonStates PlaybackButton = UP;
__IO ButtonStates RecordingButton = UP;
__IO uint32_t BufferCount = 0;
__IO uint32_t TapToneBufferCount = 1500;
__IO uint8_t BufferSwitch = 0;
__IO uint32_t TimeCounter = 0;
__IO uint8_t ToggleRecording = 0;
__IO uint8_t TogglePlayback = 0;
__IO uint8_t StartApp = 0;
__IO uint32_t CurrentSize;
__IO uint32_t CurrentBytes;
__IO uint8_t DmaTransferReady = 0;
__IO GPIO_PinState GuitarTrigger = 0;

uint16_t sample;

uint8_t first_filled = 0;

uint16_t readADC[2];
uint32_t audioBufferIndex;

int16_t loop = 0;
extern DAC_HandleTypeDef hdac;
//extern SPI_HandleTypeDef hspi3;
extern uint8_t Audio_Buffer[];
extern __IO BUFFER_StateTypeDef buffer_offset;

static inline uint16_t mixGuitar(uint16_t a, uint16_t b,int16_t dubbing) {

//	if(a < 0 && b < 0)
//		return (int16_t)((a + b) - ((a * b)/SHRT_MIN));
//	if( a > 0 && b > 0)
//	   return (int16_t)((a + b) - ((a * b)/SHRT_MAX));


	if(dubbing == 0)
		return b;
	//return (a + b) / 2;
	if(a < 2048 && b < 2048)
		return (a * b) / 2048;
	return 2 * (a + b) - ((a * b) /2048) - 4096;
	//return (uint16_t)((a + b) / 2);
	//return (int16_t)((a + b*1.2)*0.5);
}

int32_t ads1256data;

static uint16_t getSample() {

	//ADS1256_WaitDRDY();
	//ads1256data = ADS1256_ReadData();
	return (ads1256data >> 8) + 32768;
}


void tapToggle() {
	ToggleRecording = !ToggleRecording;
	if (ToggleRecording == 1)
		TapToneBufferCount = 0;
}

uint16_t drumsample;
uint16_t newsample;

uint16_t upper;
uint16_t lower;
uint16_t mixed;
uint32_t upperlower;
uint8_t *pDrumset = (uint8_t *)&drumset;

void play(uint16_t newsample){
	BSP_SDRAM_ReadData(SDRAM_DEVICE_ADDR + read_pointer,(uint32_t *) &upperlower, 1);
	upper = (upperlower >> 16);
	lower = (upperlower & 0x0000FFFF);
	HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,upper);
	HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,lower);
	Dubbing = ToggleDubbing;
	//Write_DAC8552(channel_A,upper);
	//if(*pDrumset > 0)
		//upper = mixGuitar(upper,drumsample,1);
	mixed = mixGuitar(newsample,lower,Dubbing);
	upperlower = upper;
	upperlower <<= 16;
	upperlower |= (uint32_t)mixed;
	BSP_SDRAM_WriteData(SDRAM_DEVICE_ADDR + read_pointer,(uint32_t *) &upperlower, 1);

	SamplesRead++;
	if(SamplesRead == SamplesWritten){
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

void record(uint16_t sample){
	drumsample = drumHandler();
	if(StartApp == 0 ){
		return;
	}

	upperlower = (((uint32_t)sample) << 16) | ((uint32_t)(sample / 2));

	BSP_SDRAM_WriteData(SDRAM_DEVICE_ADDR + write_pointer,(uint32_t *) &upperlower, 1);
	SamplesWritten++;
	write_pointer += 4;
	if(write_pointer == SDRAM_SIZE)
		write_pointer = 0;


}

void playMulti(uint8_t number,uint16_t sample,struct tracks * tr){
	BSP_SDRAM_ReadData(SDRAM_DEVICE_ADDR + read_pointer,(uint32_t *) tr, 3);
	HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,tr->sum / 1);
	HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,tr->sum / 1);
	SamplesRead++;
	if(SamplesRead == SamplesWritten){
		dub_pointer = 0;
		SamplesRead = 0;
		read_pointer = 0;
		return;
	}
	dub_pointer += 12;
	read_pointer += 12;
	if(read_pointer == SDRAM_SIZE)
		read_pointer = 0;
	if(dub_pointer == SDRAM_SIZE)
		dub_pointer = 0;
}
void recordMulti(uint8_t number,uint16_t sample,struct tracks * tr){
	if(StartApp == 0 )
		return;
	tr->samples[number] = sample;
	tr->sum = tr->samples[TRACK1] + tr->samples[TRACK2] + tr->samples[TRACK3] + tr->samples[TRACK4];
	BSP_SDRAM_WriteData(SDRAM_DEVICE_ADDR + write_pointer,(uint32_t *) tr, 3);
	SamplesWritten++;
	write_pointer += 12;
	if(write_pointer == SDRAM_SIZE)
		write_pointer = 0;
}


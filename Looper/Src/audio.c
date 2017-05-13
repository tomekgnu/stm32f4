#include "main.h"
#include "ads1256_test.h"
#include "stm32f429i_discovery_sdram.h"
#include "string.h"
#include "limits.h"

#define USHORT_TO_DOUBLE(x)					((double)(x) / (double)(USHRT_MAX + 1))
#define DOUBLE_TO_USHORT(x)					(uint16_t)(x * ((double)(USHRT_MAX + 1)))

uint32_t read_buffer_1[SAMPLE_ARRAY];
uint32_t read_buffer_2[SAMPLE_ARRAY];
uint32_t write_buffer_1[SAMPLE_ARRAY];
uint32_t write_buffer_2[SAMPLE_ARRAY];

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
uint32_t readADC;

int16_t loop = 0;
//extern DAC_HandleTypeDef hdac;
extern SPI_HandleTypeDef hspi3;

static inline uint16_t mixGuitar(uint16_t a, uint16_t b,int16_t dubbing) {
//	if(a < 0 && b < 0)
//		return (int16_t)((a + b) - ((a * b)/SHRT_MIN));
//	if( a > 0 && b > 0)
//	   return (int16_t)((a + b) - ((a * b)/SHRT_MAX));
	if(dubbing == 0)
		return b;
	if(a < 32768 && b < 32768)
		return (a * b) / 32768;
	return 2 * (a + b) - ((a * b) /32768) - 65536;
	//return (uint16_t)((a + b) / 2);
	//return (int16_t)((a + b*1.2)*0.5);
}

int32_t ads1256data;

static uint16_t getSample() {

	ADS1256_WaitDRDY();
	ads1256data = ADS1256_ReadData();
	return (ads1256data >> 8) + 32768;
}


void tapToggle() {
	ToggleRecording = !ToggleRecording;
	if (ToggleRecording == 1)
		TapToneBufferCount = 0;
}

uint16_t newsample;
uint32_t oldsample;
uint16_t upper;
uint16_t lower;
uint16_t mixed;
uint32_t upperlower;

void play_record(){

	drumHandler();
	if(StartApp == 0 ){
		return;
	}

	Dubbing = ToggleDubbing;
	newsample = getSample();
	if(Recording == 1){
			upperlower = (((uint32_t)newsample) << 16);// | ((uint32_t)(newsample > 1));
			BSP_SDRAM_WriteData(SDRAM_DEVICE_ADDR + write_pointer,(uint32_t *) &upperlower, 1);
			SamplesWritten++;
	}

	else if(Playback == 1){
		BSP_SDRAM_ReadData(SDRAM_DEVICE_ADDR + read_pointer,(uint32_t *) &upperlower, 1);
		upper = (upperlower >> 16);
		if(DrumsPlaying == 1)
			upper = mixGuitar(upper,drumHandler() * 0.3,1);
		lower = (upperlower & 0x0000FFFF);
		Write_DAC8552(channel_A,upper);
		Write_DAC8552(channel_B,lower);
		mixed = mixGuitar(newsample,lower,Dubbing);
		upperlower = upper;
		upperlower <<= 16;
		upperlower |= (uint32_t)mixed;
		BSP_SDRAM_WriteData(SDRAM_DEVICE_ADDR + dub_pointer,(uint32_t *) &upperlower, 1);

		SamplesRead++;
		if(SamplesRead == SamplesWritten){
				dub_pointer = 0;
				SamplesRead = 0;
				read_pointer = 0;
				return;
			}

		}

		dub_pointer += 4;
		read_pointer += 4;
		write_pointer += 4;
		if(write_pointer == SDRAM_SIZE)
			write_pointer = 0;
		if(read_pointer == SDRAM_SIZE)
			read_pointer = 0;
		if(dub_pointer == SDRAM_SIZE)
			dub_pointer = 0;
}

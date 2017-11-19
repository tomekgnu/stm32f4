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

uint32_t read_buffer_1[SAMPLE_ARRAY];
uint32_t read_buffer_2[SAMPLE_ARRAY];
uint32_t write_buffer_1[SAMPLE_ARRAY];
uint32_t write_buffer_2[SAMPLE_ARRAY];
uint32_t highest_sample_sum;
extern uint8_t tracksPlaying;
extern uint8_t currentLoop;
extern FMC_SDRAM_CommandTypeDef SDRAMCommandStructure;
uint32_t * rdptr;
uint32_t * wrptr;	// pointer for writinguint32_t SamplesRead = 0;
uint32_t dub_pointer = 0;
uint32_t read_pointer = 0;
extern __IO uint32_t midiDrumClock;
extern __IO uint32_t midiDrumPointer;
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

//extern SPI_HandleTypeDef hspi3;
extern uint8_t Audio_Buffer[];
extern __IO BUFFER_StateTypeDef buffer_offset;

static inline uint16_t mixGuitar(uint16_t a, uint16_t b) {

//	if(a < 0 && b < 0)
//		return (int16_t)((a + b) - ((a * b)/SHRT_MIN));
//	if( a > 0 && b > 0)
//	   return (int16_t)((a + b) - ((a * b)/SHRT_MAX));


	if(a < 32768 && b < 32768)
		return (a * b) / 32768;
	return 2 * (a + b) - ((a * b) /32768) - 65536;
	//return (uint16_t)((a + b) / 2);
	//return (int16_t)((a + b*1.2)*0.5);
}

static inline uint16_t mixMultiple(struct tracks *trck,uint8_t playing){
	return (trck->sum / playing ) ;
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


void play16u(uint16_t newsample){
	BSP_SDRAM_ReadData(SDRAM_DEVICE_ADDR + read_pointer,(uint32_t *) &upperlower, 1);
	upper = (upperlower >> 16);
	lower = (upperlower & 0x0000FFFF);
	//HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,upper);
	//HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,lower);
	Dubbing = ToggleDubbing;
	Write_DAC8552(channel_A,upper);
	if(Dubbing == 1){
		lower = newsample;
	}
	upperlower = upper;
	upperlower <<= 16;
	upperlower |= (uint32_t)lower;
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

void record16u(uint16_t sample){

	if(StartApp == 0 ){
		return;
	}

	upperlower = ((uint32_t)sample) << 16;

	BSP_SDRAM_WriteData(SDRAM_DEVICE_ADDR + write_pointer,(uint32_t *) &upperlower, 1);
	SamplesWritten++;
	write_pointer += 4;
	if(write_pointer == SDRAM_SIZE)
		write_pointer = 0;
}

float gain = 1.0;
uint32_t clipcnt = 0;
int32_t mix32Max = 32767;

void record32s(int32_t sample){

	if(StartApp == 0 ){
		return;
	}
	gain = 1.0;
	mix32Max = 32767;
	BSP_SDRAM_WriteData(SDRAM_DEVICE_ADDR + write_pointer,(uint32_t *) &sample, 1);
	SamplesWritten++;
	write_pointer += 4;
	if(write_pointer == SDRAM_SIZE)
		write_pointer = 0;
}



void play32s(int32_t newsample){
	int32_t mix32s,mix32tmp,read32s;

	BSP_SDRAM_ReadData(SDRAM_DEVICE_ADDR + read_pointer,(uint32_t *) &read32s, 1);
	Write_DAC8552(channel_A,((uint16_t) (((float)read32s * gain) + 32767)));

	mix32tmp = (int32_t)(newsample + read32s);
	if(mix32tmp > mix32Max){
		mix32Max = mix32tmp;
		clipcnt++;
	}

	if(Dubbing == 1){
		//mix32s = (int32_t)((float)mix32tmp * gain);
		BSP_SDRAM_WriteData(SDRAM_DEVICE_ADDR + read_pointer,(uint32_t *) &mix32tmp, 1);
	}
	else
		BSP_SDRAM_WriteData(SDRAM_DEVICE_ADDR + read_pointer,(uint32_t *) &read32s, 1);

	SamplesRead++;
	if(SamplesRead == SamplesWritten){
		Dubbing = ToggleDubbing;
		if(clipcnt > 0)
			gain = 32767.00 / (float)mix32Max;
		else{
			gain = 1.0;
			clipcnt = 0;
			mix32Max = 32767;
		}
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

void recordFloat(float sample){

	if(StartApp == 0 ){
		return;
	}

	BSP_SDRAM_WriteData(SDRAM_DEVICE_ADDR + write_pointer,(uint32_t *) &sample, 1);
	SamplesWritten++;
	write_pointer += 4;
	if(write_pointer == SDRAM_SIZE)
		write_pointer = 0;
}


void playFloat(float newsample){
	float mix32f;
	static float gain = 1.0;
	static float mixFloatMax = 1.00;

	BSP_SDRAM_ReadData(SDRAM_DEVICE_ADDR + read_pointer,(uint32_t *) &mix32f, 1);
	Dubbing = ToggleDubbing;
	Write_DAC8552(channel_A,(uint16_t) ((mix32f  * 32768) + 32767));

	if(Dubbing == 1){
		mix32f = (newsample  + mix32f) * gain;
		if(mix32f > mixFloatMax)
			mixFloatMax = mix32f;
	}

	BSP_SDRAM_WriteData(SDRAM_DEVICE_ADDR + read_pointer,(uint32_t *) &mix32f, 1);
	SamplesRead++;
	if(SamplesRead == SamplesWritten){
		gain = 1.00 / mixFloatMax;
		mixFloatMax = 1.00;
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

void playMultiFromTimer(uint8_t number,uint16_t sampleA,uint16_t sampleB,struct tracks * tr){
	uint16_t mix;
		BSP_SDRAM_ReadData(SDRAM_DEVICE_ADDR + read_pointer,(uint32_t *) tr, 3);
		mix = mixMultiple(tr,tracksPlaying);
		Write_DAC8552(channel_B,(uint16_t)sampleA);
		//HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,mix);
		//HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,mix);

}
void playMulti(uint8_t number,uint16_t sampleA,uint16_t sampleB,struct tracks * tr){
	uint16_t mix;
	BSP_SDRAM_ReadData(SDRAM_DEVICE_ADDR + read_pointer,(uint32_t *) tr, 3);
	Write_DAC8552(channel_A,tr->samples[TRACK1]);

	Dubbing = ToggleDubbing;
	if(Dubbing == 1){
			tr->samples[TRACK1] = (tr->samples[TRACK1] * 1.1 + sampleA) * 0.5; //mixGuitar(sampleA,tr->samples[TRACK1]);
			tr->sum = tr->samples[TRACK1] + tr->samples[TRACK2] + tr->samples[TRACK3] + tr->samples[TRACK4];
			BSP_SDRAM_WriteData(SDRAM_DEVICE_ADDR + read_pointer,(uint32_t *) tr, 3);
		}
	SamplesRead++;
	if(SamplesRead == SamplesWritten){
		midiDrumPointer = 0;
		midiDrumClock = 0;
		if(Dubbing == 1){
			tracksPlaying++;
			currentLoop++;
		}
		if(tracksPlaying == 5)
			tracksPlaying = 4;
		if(currentLoop == 4)
			currentLoop = 0;
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
void recordMulti(uint8_t number,uint16_t sampleA,uint16_t sampleB,struct tracks * tr){
	if(StartApp == 0 )
		return;

	tr->samples[number] = sampleA;
	tr->sum = tr->samples[TRACK1] + tr->samples[TRACK2] + tr->samples[TRACK3] + tr->samples[TRACK4];
	BSP_SDRAM_WriteData(SDRAM_DEVICE_ADDR + write_pointer,(uint32_t *) tr, 3);
	SamplesWritten++;
	write_pointer += 12;
	if(write_pointer == SDRAM_SIZE)
		write_pointer = 0;
}


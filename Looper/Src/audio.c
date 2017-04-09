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
uint32_t SamplesWritten = 0;
uint32_t SamplesRead = 0;
uint32_t DataReady = 0;
__IO uint8_t Recording = 0;
__IO uint8_t Playback = 0;
__IO uint8_t Dubbing = 0;
__IO uint8_t ToggleRhythm = 0;

__IO ButtonStates DubbingPressed = UP;
__IO ButtonStates PlaybackButton = UP;
__IO ButtonStates RecordingButton = UP;
__IO uint32_t BufferCount = 0;
__IO uint32_t TapToneBufferCount = 1500;
__IO uint8_t BufferSwitch = 0;
__IO uint32_t TimeCounter = 0;
__IO uint8_t ToggleRecording = 0;
__IO uint8_t StartApp = 0;
__IO uint32_t CurrentSize;
__IO uint32_t CurrentBytes;
__IO uint8_t DmaTransferReady = 0;
__IO GPIO_PinState GuitarTrigger = 0;

uint16_t sample;
uint32_t read_pointer;
uint32_t write_pointer;
uint8_t first_filled = 0;
uint32_t readADC;

int16_t loop = 0;
//extern DAC_HandleTypeDef hdac;

static inline uint16_t mix(uint16_t a, uint16_t b,int16_t loop) {
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

int32_t ads1256data;

static uint16_t getSample() {

	ADS1256_WaitDRDY();
	ads1256data = ADS1256_ReadData();
	return (ads1256data >> 8) + 32768;
}

inline static void adc_play_dac(){
	uint16_t sample = getSample();
	Write_DAC8552(channel_A,sample);
	Write_DAC8552(channel_B,sample);
}


void play_memory(){
	BufferCount = 0;
	SamplesRead = 0;
	while(Recording == 0){
		dac_adc_memory();
		BufferCount++;
		if(BufferCount == 90000 || BufferCount == SamplesWritten){
			BufferCount = 0;
		}
	}

}


void record_memory(){
	BufferCount = 0;
	SamplesWritten = 0;
	while(Recording == 1){
		dac_adc_memory();
		BufferCount++;
		if(BufferCount == 90000){
			BufferCount = 0;
		}
	}
}



inline void dac_adc(){
	uint16_t newsample = getSample();
	uint16_t oldsample = rdptr[BufferCount];
	uint16_t mixed = mix(newsample,oldsample,1);

	if(Recording == 0){
		Write_DAC8552(channel_A,newsample);
		Write_DAC8552(channel_B,oldsample);
		if(Dubbing == 1)
			wrptr[BufferCount] = mixed;
		SamplesRead++;
	}
	else{
		Write_DAC8552(channel_A,newsample);
		wrptr[BufferCount] = newsample;
		SamplesWritten++;
	}

	BufferCount++;
	if(BufferCount == SAMPLE_ARRAY){
		DataReady = 1;
		BufferCount = 0;
	}
}

void write_sdram(){
	//uint32_t size = (BufferCount * 2 / 4) + (BufferCount % 2);
	if (BufferSwitch == 1) {
			BufferSwitch = 0;
			wrptr = write_buffer_2;
			BSP_SDRAM_WriteData_DMA(SDRAM_DEVICE_ADDR + write_pointer,(uint32_t *) write_buffer_1, MEM_BLOCK);
			if (first_filled == 0) {
				memcpy(read_buffer_1, write_buffer_1, BufferCount * 2);
				first_filled = 1;
			}
			bzero(write_buffer_1, SAMPLE_BYTES);
			write_pointer += (BufferCount * 2);
		}
	else {
			BufferSwitch = 1;
			wrptr = write_buffer_1;
			BSP_SDRAM_WriteData_DMA(SDRAM_DEVICE_ADDR + write_pointer,(uint32_t *) write_buffer_2, MEM_BLOCK);
			if (first_filled == 1) {
				memcpy(read_buffer_2, write_buffer_2, MEM_BLOCK);
				first_filled = 2;
			}
			bzero(write_buffer_2, MEM_BLOCK);
			write_pointer += (BufferCount * 2);
		}

}
void read_sdram(){
	uint32_t size = (BufferCount * 2 / 4) + (BufferCount % 2);
	if (BufferSwitch == 1) {
		BufferSwitch = 0;
		wrptr = write_buffer_2;
		rdptr = read_buffer_2;
		if (Dubbing == 1)
			BSP_SDRAM_WriteData_DMA(SDRAM_DEVICE_ADDR + read_pointer - SAMPLE_BYTES,(uint32_t *) write_buffer_1, size);
		read_pointer += (BufferCount * 2);
		BSP_SDRAM_ReadData_DMA(SDRAM_DEVICE_ADDR + read_pointer,(uint32_t *) read_buffer_1, size);
	}
	else {
			BufferSwitch = 1;
			wrptr = write_buffer_1;
			rdptr = read_buffer_1;
			if (Dubbing == 1)
				BSP_SDRAM_WriteData_DMA(SDRAM_DEVICE_ADDR + read_pointer - SAMPLE_BYTES,(uint32_t *) write_buffer_2, size);
			read_pointer += (BufferCount * 2);
			BSP_SDRAM_ReadData_DMA(SDRAM_DEVICE_ADDR + read_pointer,(uint32_t *) read_buffer_2, size);

		}


}

void eraseMemory(){
	uint32_t pointer = 0;
	bzero(write_buffer_1, SAMPLE_BYTES);
	while(pointer < SDRAM_SIZE){
		BSP_SDRAM_WriteData(SDRAM_DEVICE_ADDR + pointer,(uint32_t *) write_buffer_1, MEM_BLOCK);
		pointer += SAMPLE_BYTES;
	}
}

static int32_t mixbuffer[SAMPLE_ARRAY];
static uint32_t highestmix = 0;
static float multiplier = 1.0;


void recordLoop() {
	loop = 1;
	uint32_t write_pointer = 0;
	uint8_t first_filled = 0;	// first read buffer
	DataReady = 0;
	BufferSwitch = 1;
	BufferCount = 0;
	SamplesWritten = 0;
	DmaTransferReady = 0;
	memset(write_buffer_1, 0, SAMPLE_BYTES);
	memset(write_buffer_2, 0, SAMPLE_BYTES);
	wrptr = write_buffer_1;

	while (Recording == 1) {
		highestmix = 0;
		multiplier = 1.0;
		while (DataReady == 0)
			continue;
		DataReady = 0;
		CurrentBytes = BufferCount * 4;
		BufferCount = 0;
		if (BufferSwitch == 1) {
			BufferSwitch = 0;
			wrptr = write_buffer_2;
			DmaTransferReady = 0;
			BSP_SDRAM_WriteData_DMA(SDRAM_DEVICE_ADDR + write_pointer,(uint32_t *)write_buffer_1, CurrentBytes);
			while(!DmaTransferReady)
				continue;

			if (first_filled == 0) {
				memcpy(read_buffer_1, write_buffer_1, SAMPLE_BYTES);
				first_filled = 1;
			}
			memset(write_buffer_1, 0, SAMPLE_BYTES);
		} else {
			BufferSwitch = 1;
			wrptr = write_buffer_1;
			DmaTransferReady = 0;
			BSP_SDRAM_WriteData_DMA(SDRAM_DEVICE_ADDR + write_pointer, (uint32_t *)write_buffer_2, CurrentBytes);
			while(!DmaTransferReady)
				continue;

			if (first_filled == 1) {
				memcpy(read_buffer_2, write_buffer_2, SAMPLE_BYTES);
				first_filled = 2;
			}
			memset(write_buffer_2, 0, SAMPLE_BYTES);
		}

		write_pointer += CurrentBytes;

		if (write_pointer == SDRAM_SIZE) {
			write_pointer = 0;
			wrptr = write_buffer_1;
			SamplesWritten = 0;
			BufferCount = 0;
		}
	}


}
void playbackLoop() {
	uint32_t read_pointer = SAMPLE_BYTES;
	uint32_t time = 0;
	DataReady = 0;
	BufferSwitch = 1;
	BufferCount = 0;
	SamplesRead = 0;
	rdptr = read_buffer_1;
	wrptr = write_buffer_1;

	while(!DmaTransferReady)
		continue;
	DmaTransferReady = 0;

	while (Playback == 1) {
		highestmix = 0;
		multiplier = 1.0;
		while (SamplesRead == SamplesWritten || DataReady == 0){
			if(SamplesRead == SamplesWritten){
				DataReady = 1;
				SamplesRead = 0;
//				if(HAL_GPIO_ReadPin(Dubbing_GPIO_Port,Dubbing_Pin) == GPIO_PIN_RESET)
//					Dubbing = 1;
//				else
//					Dubbing = 0;
				goto END_PLAYBACK;
			}

			continue;
		}
		DataReady = 0;

		CurrentBytes = BufferCount * 4;
		BufferCount = 0;
		if (BufferSwitch == 1) {
			BufferSwitch = 0;
			rdptr = read_buffer_2;
			wrptr = write_buffer_2;
			if (Dubbing == 1){
				DmaTransferReady = 0;
				//lowerMixedSamples(write_buffer_1);
				BSP_SDRAM_WriteData_DMA(SDRAM_DEVICE_ADDR + read_pointer - SAMPLE_BYTES,(uint32_t *) write_buffer_1, CurrentBytes);
				while(!DmaTransferReady)
					continue;

			}
			read_pointer += CurrentBytes;
			DmaTransferReady = 0;
			BSP_SDRAM_ReadData_DMA(SDRAM_DEVICE_ADDR + read_pointer,(uint32_t *) read_buffer_1, CurrentBytes);
			while(!DmaTransferReady)
				continue;

		} else {
			BufferSwitch = 1;
			rdptr = read_buffer_1;
			wrptr = write_buffer_1;
			if (Dubbing == 1){
				DmaTransferReady = 0;
				//lowerMixedSamples(write_buffer_2);
				BSP_SDRAM_WriteData_DMA(SDRAM_DEVICE_ADDR + read_pointer - SAMPLE_BYTES,(uint32_t *) write_buffer_2, CurrentBytes);
				while(!DmaTransferReady)
					continue;

			}
			read_pointer += CurrentBytes;
			DmaTransferReady = 0;
			BSP_SDRAM_ReadData_DMA(SDRAM_DEVICE_ADDR + read_pointer,(uint32_t *) read_buffer_2, CurrentBytes);
			while(!DmaTransferReady)
				continue;

		}

		time++;

		if (read_pointer == SDRAM_SIZE) {
			END_PLAYBACK:
			read_pointer = SAMPLE_BYTES;
			time = 0;
			DataReady = 0;
			BufferSwitch = 1;
			BufferCount = 0;
			DmaTransferReady = 0;
			BSP_SDRAM_ReadData_DMA(SDRAM_DEVICE_ADDR,(uint32_t *) read_buffer_1, SAMPLE_ARRAY);
			while(!DmaTransferReady)
				continue;

			DmaTransferReady = 0;
			BSP_SDRAM_ReadData_DMA(SDRAM_DEVICE_ADDR + read_pointer,(uint32_t *) read_buffer_2, SAMPLE_ARRAY);
			while(!DmaTransferReady)
				continue;

			rdptr = read_buffer_1;
			wrptr = write_buffer_1;

		}

	}
}


void tapToggle() {
	ToggleRecording = !ToggleRecording;
	if (ToggleRecording == 1)
		TapToneBufferCount = 0;
}

void recordToggle() {
	if(StartApp == 1){
			ToggleRecording = 1;
		}
	if (Recording == 0x00 || Recording == 0x02) {
		Recording = 0x01;

	} else {
		Recording = 0x00;
		Playback = 1;
	}
	if (Recording == 0x01) {
		Dubbing = 0;
		//SamplesRead = 0;
		//DubbingBufferCount = BufferCount;
		BSP_LED_On(LED4);
		BSP_LED_Off(LED3);
	} else {
		BSP_LED_Off(LED4);

	}

}
void dubToggle() {
	if (Recording == 1)
		return;
	DubbingPressed = !DubbingPressed;
	if (DubbingPressed == 1) {
		//DubbingPressed = 0;
		BSP_LED_On(LED3);
		BSP_LED_Off(LED4);
	} else {
		BSP_LED_Off(LED3);
	}
}


void lowerMixedSamples(uint32_t * buf){
	int cnt;

	for(cnt = 0; cnt < SAMPLE_ARRAY; cnt++)
		buf[cnt] = buf[cnt] | (uint32_t)((float)mixbuffer[cnt] * multiplier);
	multiplier = 1.0;
	highestmix = 0;

}
void play_record(){

	uint16_t newsample;
	uint16_t upper;
	uint16_t lower;
	uint16_t mixed;
	uint32_t upperlower;

	if(StartApp == 0){
		return;
	}

	newsample = getSample();
	upper = (uint16_t)(rdptr[BufferCount] >> 16);
	lower = (uint16_t)(rdptr[BufferCount] & 0x0000FFFF);

	if(Playback == 1){
		if(ToggleRhythm == 1)
			Write_DAC8552(channel_A,upper);
			//HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,(upper + 32768) >> 4);
		Write_DAC8552(channel_B,lower);
		//Write_DAC8552_Both((uint16_t)(upper + 32768),(uint16_t)(lower + 32768));
		if(HAL_GPIO_ReadPin(Dubbing_GPIO_Port,Dubbing_Pin) == GPIO_PIN_RESET){
			if(BufferCount % SAMPLE_ARRAY == 0)
				Dubbing = 1;
		}
		else
			Dubbing = 0;
		if(Dubbing == 1){
				mixed = mix(newsample,lower,loop);
				upperlower = upper;
				upperlower <<= 16;
				upperlower |= (uint32_t)mixed;
				wrptr[BufferCount] = upperlower;

			}

			SamplesRead++;
		}
	if(Recording == 1){
			wrptr[BufferCount] = (uint32_t)(newsample << 16 );
			SamplesWritten++;

		}

		BufferCount++;

		if(ToggleRecording == 1){
			DataReady = 1;
			ToggleRecording = 0;
			return;
		}


		if(BufferCount == SAMPLE_ARRAY){
			DataReady = 1;
		}


}

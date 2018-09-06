#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "stm32f429xx.h"

#define	SIGNED16_UNSIGNED12(x)		((x / 4) + 2048)

#define ACTIVE_CHANNEL_1			looper.ch1.Active == TRUE && looper.ch2.Active == FALSE?TRUE:FALSE
#define ACTIVE_CHANNEL_2			looper.ch1.Active == FALSE && looper.ch2.Active == TRUE?TRUE:FALSE
#define ACTIVE_BOTH_CHANNELS		looper.ch1.Active == TRUE && looper.ch2.Active == TRUE?TRUE:FALSE

enum { ONE,TWO,THREE,FOUR };
typedef struct {
	uint8_t Number: 1;
	uint8_t Active: 1;
	uint8_t Monitor: 1;
	uint8_t Clipping: 1;
	uint8_t Overdub: 1;
	uint8_t Fill: 3;
	uint32_t SamplesRead;
	uint32_t SamplesWritten;
	int16_t CurrentSample;
	int32_t mix32Max;
	int16_t mix32tmp;
	float gain;
} CHANNEL;

void resetSamples();
void signed16_unsigned12(int16_t *buf,int32_t start,int32_t stop);
void showMinMaxSamples(int32_t min,int32_t max);
void read_samples(int16_t swrite,__IO CHANNEL *cha,__IO CHANNEL *chb);
void record_samples(int16_t swrite,__IO CHANNEL *cha,__IO CHANNEL *chb);
void play_samples(__IO CHANNEL *cha,__IO CHANNEL *chb);

void read_sample(int16_t swrite,__IO CHANNEL *cha);
void record_sample(int16_t swrite,__IO CHANNEL *cha);
void play_sample(__IO CHANNEL *cha);
void play_sample_dac(__IO CHANNEL *cha);
void resetChannels();

#endif

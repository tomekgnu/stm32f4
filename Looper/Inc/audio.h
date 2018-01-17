#ifndef __AUDIO_H__
#define __AUDIO_H__
#include "main.h"

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

void showMinMaxSamples(int32_t min,int32_t max);
void read_samples(int16_t swrite,__IO CHANNEL *cha,__IO CHANNEL *chb);
void record_samples(int16_t swrite,__IO CHANNEL *cha,__IO CHANNEL *chb);
void play_samples(__IO CHANNEL *cha,__IO CHANNEL *chb);

#endif

#ifndef __AUDIO_H__
#define __AUDIO_H__

struct tracks{
	uint16_t samples[4];
	uint16_t loop: 4;
	uint16_t playing: 8;
	uint16_t empty: 4;
	uint16_t sum;
};

enum{TRACK1=0,TRACK2,TRACK3,TRACK4};

void play16u(uint16_t newsample);
void record16u(uint16_t sample);
void play32s(int32_t newsample);
void record32s(int32_t sample);
void playFloat(float newsample);
void recordFloat(float sample);
void playMulti(uint8_t number,uint16_t sampleA,uint16_t sampleB,struct tracks *);
void playMultiFromTimer(uint8_t number,uint16_t sampleA,uint16_t sampleB,struct tracks *);
void recordMulti(uint8_t number,uint16_t sampleA,uint16_t sampleB,struct tracks *);
#endif

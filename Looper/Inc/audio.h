#ifndef __AUDIO_H__
#define __AUDIO_H__

struct tracks{
	uint16_t samples[4];
	uint16_t sum;
	uint16_t empty;
};

enum{TRACK1=0,TRACK2,TRACK3,TRACK4};

void play(uint16_t newsample);
void record(uint16_t sample);
void playMulti(uint8_t number,uint16_t sample,struct tracks *);
void recordMulti(uint8_t number,uint16_t sample,struct tracks *);
#endif
#ifndef __DRUMS_H
#define __DRUMS_H

#include "stm32f429xx.h"
#include "ff.h"

typedef enum{DRUMS_STOPPED,DRUMS_STARTED,DRUMS_READY}DrumFunction;

typedef struct{
	uint32_t beats;	// number of beats
	uint32_t division; // number of smaller parts in single beat
	uint32_t beattime;	// beat time e.g. 60..120
	uint32_t repeat;
}Pattern;

typedef struct{
	uint32_t numberOfBeats;
	uint32_t barDuration;
	uint32_t remainder;
	uint32_t beatDuration;

}DrumTimes;

void readDrums(FIL *fil);
void resetDrums();
void stopDrums();
void playDrums();
void midiDrumHandler();
void initDrumBeats();
void setBarBeats(uint16_t bar,uint16_t beats);
void setPatternTime(__IO Pattern *p,__IO DrumTimes *t);

#define DRUM_TIM_HDR_SIZE	16	// beats,division,beattime,repeat: each 4 bytes
#define DRUM_INSTR			5	// 4 drums + bass
#define L_HAND	0
#define R_HAND	1
#define L_FOOT	2
#define R_FOOT	3
#define BASS	4

// index constants for drum file header
#define NUM_OF_BYTES		0
#define NUM_OF_PATTERNS		1
#define MAX_RESOLUTION		2

extern uint32_t drumBeatIndex;
extern uint8_t key_to_drum[];
#endif

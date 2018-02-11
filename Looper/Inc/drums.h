#ifndef __DRUMS_H
#define __DRUMS_H

#include "stm32f429xx.h"
#include "ff.h"

void readDrums(FIL *fil);
void resetDrums();
void playDrums();
void midiDrumHandler();
void initDrumBeats();
void setBarBeats(uint16_t bar,uint16_t beats);

#define L_HAND	0
#define R_HAND	1
#define L_FOOT	2
#define R_FOOT	3


typedef struct{
	uint16_t beats;	// number of beats
	uint16_t duration; // bar duration
	uint8_t drumTracks[4][32];	// drum data
}DrumBar;


extern DrumBar bar1;
extern DrumBar bar2;
extern uint8_t (*drumTracksPointer)[32];
extern uint32_t drumBeatIndex;
extern uint8_t key_to_drum[];
#endif

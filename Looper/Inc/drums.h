#ifndef __DRUMS_H
#define __DRUMS_H

#include "stm32f429xx.h"

void resetDrums();
void playDrums();
void midiDrumHandler();
void initDrumBeats();
void setBarDuration(uint16_t dur);
void setNumberOfBeats(uint16_t dur);

#define L_HAND	0
#define R_HAND	1
#define L_FOOT	2
#define R_FOOT	3
#define	DRUM	0
#define	TIME	1

extern uint16_t drumTracks[4][16];
extern uint8_t key_to_drum[];
#endif

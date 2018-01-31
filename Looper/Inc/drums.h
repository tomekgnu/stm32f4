#ifndef __DRUMS_H
#define __DRUMS_H

#include "stm32f429xx.h"

void resetDrums();
void playDrums();
void midiDrumHandler();
void initDrumBeats();

#define L_HAND	0
#define R_HAND	1
#define L_FOOT	2
#define R_FOOT	3
#define	DRUM	0
#define	TIME	1

extern uint16_t drumTracks[4][2][16];

#endif

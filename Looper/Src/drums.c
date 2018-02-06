#include "drums.h"
#include "midi.h"


uint16_t drumTracks[4][16];
static uint16_t barDuration = 0;
static uint16_t numberOfBeats = 0;
static uint16_t beatDuration = 0;
static uint8_t remainder = 0;

__IO BOOL ShowBeat = FALSE;

uint8_t key_to_drum[16] = {
		Acoustic_Bass_Drum,
		Side_Stick,
		Acoustic_Snare,
		Cowbell,
		Low_Floor_Tom,
		High_Floor_Tom,
		Low_Mid_Tom,
		Hi_Mid_Tom,
		High_Tom,
		Closed_Hi_Hat,
		Open_Hi_Hat,
		Pedal_Hi_Hat,
		Crash_Cymbal_1,
		Ride_Cymbal_2,
		Splash_Cymbal,
		Chinese_Cymbal
};

void setBarDuration(uint16_t dur){
	barDuration = dur;
}

void setNumberOfBeats(uint16_t num){
	numberOfBeats = num;
	remainder = barDuration % numberOfBeats;
	beatDuration = barDuration / numberOfBeats;
}

void midiDrumHandler(){

	if(midiDrumClock < barDuration){
		if(midiDrumClock % (remainder > 0?(beatDuration + remainder):beatDuration) == 0){
			if(drumTracks[R_FOOT][midiDrumPointer] != 0)
				playPercussion(NOTEON,drumTracks[R_FOOT][midiDrumPointer]);
			if(drumTracks[L_FOOT][midiDrumPointer] != 0)
				playPercussion(NOTEON,drumTracks[L_FOOT][midiDrumPointer]);
			if(drumTracks[R_HAND][midiDrumPointer] != 0)
				playPercussion(NOTEON,drumTracks[R_HAND][midiDrumPointer]);
			if(drumTracks[L_HAND][midiDrumPointer] != 0)
				playPercussion(NOTEON,drumTracks[L_HAND][midiDrumPointer]);
			midiDrumPointer++;
			ShowBeat = TRUE;
		}
		midiDrumClock++;
	}
	else
	{
		midiDrumClock = 0;
		midiDrumPointer = 0;
	}

}


void initDrumBeats(){

}

void resetDrums(){

	midiDrumClock = 0;
	midiDrumPointer = 0;

}


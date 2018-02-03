#include "drums.h"
#include "midi.h"


uint16_t drumTracks[4][16];
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

void midiDrumHandler(){

	if(midiDrumClock < 4000){
		if(midiDrumClock % 250 == 0){
			if(drumTracks[R_FOOT][midiDrumPointers[R_FOOT]] != 0)
				playPercussion(NOTEON,drumTracks[R_FOOT][midiDrumPointers[R_FOOT]]);
			if(drumTracks[L_FOOT][midiDrumPointers[L_FOOT]] != 0)
				playPercussion(NOTEON,drumTracks[L_FOOT][midiDrumPointers[L_FOOT]]);
			if(drumTracks[R_HAND][midiDrumPointers[R_HAND]] != 0)
				playPercussion(NOTEON,drumTracks[R_HAND][midiDrumPointers[R_HAND]]);
			if(drumTracks[L_HAND][midiDrumPointers[L_HAND]] != 0)
				playPercussion(NOTEON,drumTracks[L_HAND][midiDrumPointers[L_HAND]]);
			midiDrumPointers[R_FOOT]++;
			midiDrumPointers[L_FOOT]++;
			midiDrumPointers[R_HAND]++;
			midiDrumPointers[L_HAND]++;
			ShowBeat = TRUE;
		}
		midiDrumClock++;
	}
	else
	{
		midiDrumClock = 0;
		midiDrumPointers[L_HAND] = 0;
		midiDrumPointers[R_HAND] = 0;
		midiDrumPointers[L_FOOT] = 0;
		midiDrumPointers[R_FOOT] = 0;
	}

}

void playDrums(){

		if(drumTracks[R_FOOT][midiDrumPointers[R_FOOT]] != 0)
			playPercussion(NOTEON,drumTracks[R_FOOT][midiDrumPointers[R_FOOT]]);




		if(drumTracks[L_FOOT][midiDrumPointers[L_FOOT]] != 0)
			playPercussion(NOTEON,drumTracks[L_FOOT][midiDrumPointers[L_FOOT]]);




		if(drumTracks[R_HAND][midiDrumPointers[R_HAND]] != 0)
			playPercussion(NOTEON,drumTracks[R_HAND][midiDrumPointers[R_HAND]]);




		if(drumTracks[L_HAND][midiDrumPointers[L_HAND]] != 0)
			playPercussion(NOTEON,drumTracks[L_HAND][midiDrumPointers[L_HAND]]);



}

void initDrumBeats(){

}

void resetDrums(){

	midiDrumClock = 0;
	midiDrumPointers[L_HAND] = 0;
	midiDrumPointers[R_HAND] = 0;
	midiDrumPointers[L_FOOT] = 0;
	midiDrumPointers[R_FOOT] = 0;

}


#include "drums.h"
#include "midi.h"

static uint8_t xpos = 0;
static uint8_t ypos = 0;

uint16_t drumTracks[4][2][16];


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
			midiDrumPointers[L_HAND]++;
			midiDrumPointers[R_HAND]++;
			midiDrumPointers[L_FOOT]++;
			midiDrumPointers[R_FOOT]++;
		}
		midiDrumClock++;
	}
	else{
			midiDrumClock = 0;
			midiDrumPointers[L_HAND] = 0;
			midiDrumPointers[R_HAND] = 0;
			midiDrumPointers[L_FOOT] = 0;
			midiDrumPointers[R_FOOT] = 0;
	}
}

void playDrums(){
	if(midiDrumClock >= drumTracks[R_FOOT][TIME][midiDrumPointers[R_FOOT]] && drumTracks[R_FOOT][DRUM][midiDrumPointers[R_FOOT]] != 0){
		playPercussion(NOTEON,drumTracks[R_FOOT][DRUM][midiDrumPointers[R_FOOT]]);
	}

	if(midiDrumClock >= drumTracks[L_FOOT][TIME][midiDrumPointers[L_FOOT]] && drumTracks[L_FOOT][DRUM][midiDrumPointers[L_FOOT]] != 0){
		playPercussion(NOTEON,drumTracks[L_FOOT][DRUM][midiDrumPointers[L_FOOT]]);
	}

	if(midiDrumClock >= drumTracks[R_HAND][TIME][midiDrumPointers[R_HAND]] && drumTracks[R_HAND][DRUM][midiDrumPointers[R_HAND]] != 0){
		playPercussion(NOTEON,drumTracks[R_HAND][DRUM][midiDrumPointers[R_HAND]]);
	}

	if(midiDrumClock >= drumTracks[L_HAND][TIME][midiDrumPointers[L_HAND]] && drumTracks[L_HAND][DRUM][midiDrumPointers[L_HAND]] != 0){
		playPercussion(NOTEON,drumTracks[L_HAND][DRUM][midiDrumPointers[L_HAND]]);
	}

}

void initDrumBeats(){
	uint8_t index;
	for(index = 0;index < 16;index++){
		uint16_t tm = index * 250;
		drumTracks[L_FOOT][TIME][index] = tm;
		drumTracks[R_FOOT][TIME][index] = tm;
		drumTracks[L_HAND][TIME][index] = tm;
		drumTracks[L_HAND][TIME][index] = tm;
	}


}

void resetDrums(){

	midiDrumClock = 0;
	midiDrumPointers[L_HAND] = 0;
	midiDrumPointers[R_HAND] = 0;
	midiDrumPointers[L_FOOT] = 0;
	midiDrumPointers[R_FOOT] = 0;

}

void moveUp(){
	if(xpos < 3)
		xpos++;
}

void moveDown(){
	if(xpos > 0)
		xpos--;
}

void moveRight(){
	if(xpos < 15)
		ypos++;
}

void moveLeft(){
	if(ypos > 0)
		xpos--;
}

#include "drums.h"
#include "midi.h"
#include "ff.h"


DrumBar bar1,bar2;
uint8_t (*drumTracksPointer)[32];
uint32_t drumBeatIndex;
static uint16_t barDuration = 0;
static uint16_t numberOfBeats = 0;
static uint16_t beatDuration = 0;
static uint8_t remainder = 0;
static __IO BOOL switch_buff;
static __IO BOOL first_beat;
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

void readDrums(FIL *fil){

	uint32_t bytesRead;

	switch_buff = TRUE;
	first_beat = FALSE;
	if(f_eof(fil))
		return;
	f_read(fil,&bar1,sizeof(DrumBar),(UINT *)&bytesRead);
	f_read(fil,&bar2,sizeof(DrumBar),(UINT *)&bytesRead);
	resetDrums();
	setBarBeats(bar1.duration,bar1.beats);
	drumTracksPointer = bar1.drumTracks;
	DrumState = DRUM_START;

	while(bytesRead > 0 && DrumState == DRUM_START){
		while(first_beat == FALSE && DrumState == DRUM_START)
			continue;
		first_beat = FALSE;
		if(switch_buff == FALSE){
			setBarBeats(bar1.duration,bar1.beats);
			drumTracksPointer = bar1.drumTracks;
			if(f_eof(fil))
				f_lseek(fil,SEEK_SET);
			f_read(fil,&bar2,sizeof(DrumBar),(UINT *)&bytesRead);
			switch_buff = TRUE;
		}
		else{
			setBarBeats(bar2.duration,bar2.beats);
			drumTracksPointer = bar2.drumTracks;
			if(f_eof(fil))
				f_lseek(fil,SEEK_SET);
			f_read(fil,&bar1,sizeof(DrumBar),(UINT *)&bytesRead);
			switch_buff = FALSE;

		}

	}

}


void setBarBeats(uint16_t bar,uint16_t beats){
	numberOfBeats = beats;
	barDuration = bar;
	remainder = barDuration % numberOfBeats;
	beatDuration = barDuration / numberOfBeats;
}

void midiDrumHandler(){

	if(midiDrumClock < barDuration){
		if(midiDrumClock % ((remainder > 0 && drumBeatIndex == 0)?(beatDuration + remainder):beatDuration) == 0){
			if(drumTracksPointer[R_FOOT][drumBeatIndex] != 0)
				playPercussion(NOTEON,drumTracksPointer[R_FOOT][drumBeatIndex]);
			if(drumTracksPointer[L_FOOT][drumBeatIndex] != 0)
				playPercussion(NOTEON,drumTracksPointer[L_FOOT][drumBeatIndex]);
			if(drumTracksPointer[R_HAND][drumBeatIndex] != 0)
				playPercussion(NOTEON,drumTracksPointer[R_HAND][drumBeatIndex]);
			if(drumTracksPointer[L_HAND][drumBeatIndex] != 0)
				playPercussion(NOTEON,drumTracksPointer[L_HAND][drumBeatIndex]);
			drumBeatIndex++;
			ShowBeat = TRUE;
		}
		midiDrumClock++;
	}
	else
	{
		first_beat = TRUE;
		midiDrumClock = 0;
		drumBeatIndex = 0;
	}

}


void initDrumBeats(){

}

void resetDrums(){

	midiDrumClock = 0;
	drumBeatIndex = 0;

}


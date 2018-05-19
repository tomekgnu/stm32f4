#include "drums.h"
#include "midi.h"
#include "ff.h"
#include "stdlib.h"

uint32_t drumBeatIndex;
static __IO BOOL switch_buff;
static __IO BOOL first_beat;

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

uint8_t *drumbuf1;
uint8_t *drumbuf2;
uint8_t * drumpointer;
Pattern pat1,pat2;
DrumTimes times1,times2,*timepointer;

void readDrums(FIL *fil){
	uint32_t bytesRead;
	switch_buff = TRUE;
	first_beat = FALSE;

	drumbuf1 = (uint8_t *)malloc(12 + 5 * 120);
	drumbuf2 = (uint8_t *)malloc(12 + 5 * 120);

	if(f_eof(fil))
		return;

	f_read(fil,&pat1,sizeof(Pattern),(UINT *)&bytesRead);
	setPatternTime(&pat1,&times1);
	f_read(fil,drumbuf1,times1.numberOfBeats * 5,(UINT *)&bytesRead);

	f_read(fil,&pat2,sizeof(Pattern),(UINT *)&bytesRead);
	setPatternTime(&pat2,&times2);
	f_read(fil,drumbuf2,times2.numberOfBeats * 5,(UINT *)&bytesRead);

	resetDrums();
	drumpointer = drumbuf1;
	timepointer = &times1;
	DrumState = DRUM_START;

	while(bytesRead > 0 && DrumState == DRUM_START){
		while(first_beat == FALSE && DrumState == DRUM_START)
			continue;
		first_beat = FALSE;
		if(switch_buff == FALSE){
			drumpointer = drumbuf1;
			timepointer = &times1;
			if(f_eof(fil))
				f_lseek(fil,SEEK_SET);
			f_read(fil,&pat2,sizeof(Pattern),(UINT *)&bytesRead);
			setPatternTime(&pat2,&times2);
			f_read(fil,drumbuf2,times2.numberOfBeats * 5,(UINT *)&bytesRead);
			switch_buff = TRUE;
		}
		else{
			drumpointer = drumbuf2;
			timepointer = &times2;
			if(f_eof(fil))
				f_lseek(fil,SEEK_SET);
			f_read(fil,&pat1,sizeof(Pattern),(UINT *)&bytesRead);
			setPatternTime(&pat1,&times1);
			f_read(fil,drumbuf1,times1.numberOfBeats * 5,(UINT *)&bytesRead);
			switch_buff = FALSE;
		}

	}

	while(DrumState == DRUM_START)
		continue;
	free(drumbuf1);
	free(drumbuf2);

}

void setPatternTime(Pattern *p,DrumTimes *t){
	int beatTimeMillis = (60000 / p->beattime) / p->division;
	t->numberOfBeats = p->beats * p->division;
	t->barDuration = t->numberOfBeats * beatTimeMillis;
	t->remainder = t->barDuration % t->numberOfBeats;
	t->beatDuration = t->barDuration / t->numberOfBeats;
}


void midiDrumHandler(){
	uint32_t i;
	if(midiDrumClock < timepointer->barDuration){
		if(midiDrumClock % ((timepointer->remainder > 0 && drumBeatIndex == 0)?(timepointer->beatDuration + timepointer->remainder):timepointer->beatDuration) == 0){
			for(i = drumBeatIndex; i < drumBeatIndex + 4; i++){
				if(drumpointer[i] != 0)
					playPercussion(NOTEON,drumpointer[i]);
			}

			if(drumpointer[i] != 0)
				playBass(NOTEON,drumpointer[i]);

			drumBeatIndex += 5;

		}

		midiDrumClock++;
	}
	else{
			first_beat = TRUE;
			midiDrumClock = 0;
			drumBeatIndex = 0;
	}

}

void resetDrums(){
	midiDrumClock = 0;
	drumBeatIndex = 0;
}


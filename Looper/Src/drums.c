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

uint8_t *dbufa;
uint8_t *dbufb;
uint8_t * dbufp;
Pattern pa,pb;
DrumTimes ta,tb,*tptr;

void readDrums(FIL *fil){
	uint32_t bytesRead;
	switch_buff = TRUE;
	first_beat = FALSE;

	dbufa = (uint8_t *)malloc(12 + 5 * 120);
	dbufb = (uint8_t *)malloc(12 + 5 * 120);

	if(f_eof(fil))
		return;

	f_read(fil,&pa,sizeof(Pattern),(UINT *)&bytesRead);
	setPatternTime(&pa,&ta);
	f_read(fil,dbufa,ta.numberOfBeats * 5,(UINT *)&bytesRead);

	f_read(fil,&pb,sizeof(Pattern),(UINT *)&bytesRead);
	setPatternTime(&pb,&tb);
	f_read(fil,dbufb,tb.numberOfBeats * 5,(UINT *)&bytesRead);

	resetDrums();
	dbufp = dbufa;
	tptr = &ta;
	DrumState = DRUM_START;

	while(bytesRead > 0 && DrumState == DRUM_START){
		while(first_beat == FALSE && DrumState == DRUM_START)
			continue;
		first_beat = FALSE;
		if(switch_buff == FALSE){
			dbufp = dbufa;
			tptr = &ta;
			if(f_eof(fil))
				f_lseek(fil,SEEK_SET);
			f_read(fil,&pb,sizeof(Pattern),(UINT *)&bytesRead);
			setPatternTime(&pb,&tb);
			f_read(fil,dbufb,tb.numberOfBeats * 5,(UINT *)&bytesRead);
			switch_buff = TRUE;
		}
		else{
			dbufp = dbufb;
			tptr = &tb;
			if(f_eof(fil))
				f_lseek(fil,SEEK_SET);
			f_read(fil,&pa,sizeof(Pattern),(UINT *)&bytesRead);
			setPatternTime(&pa,&ta);
			f_read(fil,dbufa,ta.numberOfBeats * 5,(UINT *)&bytesRead);
			switch_buff = FALSE;
		}

	}

	while(DrumState == DRUM_START)
		continue;
	free(dbufa);
	free(dbufb);

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
	if(midiDrumClock < tptr->barDuration){
		if(midiDrumClock % ((tptr->remainder > 0 && drumBeatIndex == 0)?(tptr->beatDuration + tptr->remainder):tptr->beatDuration) == 0){
			for(i = drumBeatIndex; i < drumBeatIndex + 4; i++){
				if(dbufp[i] != 0)
					playPercussion(NOTEON,dbufp[i]);
			}

			if(dbufp[i] != 0)
				playBass(NOTEON,dbufp[i]);

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


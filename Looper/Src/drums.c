#include "drums.h"
#include "midi.h"
#include "ff.h"
#include "stdlib.h"
#include "ads1256_test.h"
#include "tim.h"
#include "string.h"
uint32_t drumBeatIndex;
__IO BOOL switch_buff;
__IO BOOL first_beat;

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

uint8_t *drumBuffA;
uint8_t *drumBuffB;
uint8_t * drumBuffPtr;

__IO Pattern pat1;
__IO Pattern pat2;
__IO DrumTimes tim1;
__IO DrumTimes tim2;
__IO DrumTimes *timptr;
__IO Pattern *patptr;


void readDrums(FIL *fil){

	uint32_t bytesRead;
	switch_buff = FALSE;
	first_beat = FALSE;


	drumBuffA = (uint8_t *)malloc(DRUM_TIM_HDR_SIZE + DRUM_INSTR * 120);
	drumBuffB = (uint8_t *)malloc(DRUM_TIM_HDR_SIZE + DRUM_INSTR * 120);

	if(f_eof(fil))
		return;

	f_read(fil,&pat1,sizeof(Pattern),(UINT *)&bytesRead);
	setPatternTime(&pat1,&tim1);
	f_read(fil,drumBuffA,tim1.numberOfBeats * 5,(UINT *)&bytesRead);

	//f_read(fil,&pat2,sizeof(Pattern),(UINT *)&bytesRead);
	//setPatternTime(&pat2,&tim2);
	//f_read(fil,drumBuffB,tim2.numberOfBeats * 5,(UINT *)&bytesRead);

	resetDrums();
	HAL_TIM_Base_Start_IT(&htim2);
	while(DrumState == DRUMS_READY){
		continue;
	}

	do{


		if(switch_buff == FALSE){
				timptr = &tim1;
				patptr = &pat1;
				drumBuffPtr = drumBuffA;
				f_read(fil,&pat2,sizeof(Pattern),(UINT *)&bytesRead);
				setPatternTime(&pat2,&tim2);
				f_read(fil,drumBuffB,tim2.numberOfBeats * 5,(UINT *)&bytesRead);
				switch_buff = TRUE;
			}
			else{
				timptr = &tim2;
				patptr = &pat2;
				drumBuffPtr = drumBuffB;
				f_read(fil,&pat1,sizeof(Pattern),(UINT *)&bytesRead);
				setPatternTime(&pat1,&tim1);
				f_read(fil,drumBuffA,tim1.numberOfBeats * 5,(UINT *)&bytesRead);
				switch_buff = FALSE;
			}

			while(first_beat == FALSE)
				continue;
			first_beat = FALSE;
			if(bytesRead == 0){
				HAL_TIM_Base_Stop_IT(&htim2);
				HAL_Delay(100);
			}

	}while(bytesRead > 0);


	DrumState = DRUMS_STOPPED;
	StartLooper = FALSE;
	Playback = FALSE;
	Recording = FALSE;

	free(drumBuffA);
	free(drumBuffB);

}

void setPatternTime(__IO Pattern *p,__IO DrumTimes *t){
	int beatTimeMillis = (60000 / p->beattime) / p->division;
	t->numberOfBeats = p->beats * p->division;
	t->barDuration = t->numberOfBeats * beatTimeMillis;
	t->remainder = t->barDuration % t->numberOfBeats;
	t->beatDuration = t->barDuration / t->numberOfBeats;
}


void midiDrumHandler(){
	uint32_t i;
	if(DrumState != DRUMS_STARTED)
		return;
	if(midiDrumClock < timptr->barDuration){
		if(midiDrumClock % ((timptr->remainder > 0 && drumBeatIndex == 0)?(timptr->beatDuration + timptr->remainder):timptr->beatDuration) == 0){
			for(i = drumBeatIndex; i < drumBeatIndex + 4; i++){
				if(drumBuffPtr[i] != 0)
					playPercussion(NOTEON,drumBuffPtr[i]);
			}

			if(drumBuffPtr[i] != 0)
				playBass(NOTEON,drumBuffPtr[i]);

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


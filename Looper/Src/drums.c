#include "drums.h"
#include "main.h"
#include "midi.h"
#include "ff.h"
#include "stdlib.h"
#include "ads1256_test.h"
#include "tim.h"
#include "string.h"
#include "audio.h"
#include "SRAMDriver.h"
#include "tm_stm32f4_ili9341.h"
#include "joystick.h"
#include "menu.h"

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

uint32_t pattern_audio_map[MAX_PATTERNS + 1][2];
uint8_t drumBuffA[MAX_SUBBEATS * NUM_ALL_TRACKS];
uint8_t drumBuffB[MAX_SUBBEATS * NUM_ALL_TRACKS];
uint8_t * drumBuffPtr;

__IO Pattern pat1;
__IO Pattern pat2;
__IO DrumTimes tim1;
__IO DrumTimes tim2;
__IO DrumTimes *timptr;
__IO Pattern *patptr;

static void seekPattern(uint32_t (*pattern_audio_map)[2],uint32_t ind){
	switch_buff = FALSE;
	SRAM_seekRead(pattern_audio_map[ind][0],SRAM_SET);
	readSRAM((uint8_t *)&pat1,sizeof(Pattern));
	setPatternTime(&pat1,&tim1);
	readSRAM((uint8_t *)drumBuffA,tim1.subbeats * 5);

}

void drumAudioSync(){
	looper.ch1.SamplesRead = pattern_audio_map[looper.startPattern][1];
	sdram_pointer = looper.ch1.SamplesRead * 2;
	looper.ch1.SamplesWritten = pattern_audio_map[looper.endPattern + 1][1];
}

void drumLoop(){
	uint32_t tmp = looper.startPattern;
	switch_buff = FALSE;
	first_beat = FALSE;
	seekPattern(pattern_audio_map,looper.startPattern);
	resetDrums();
	HAL_TIM_Base_Start_IT(&htim2);
	looper.Function = AUDIO_DRUMS;
	looper.DrumState = DRUMS_STARTED;
	setStartEndPatterns(looper.startPattern,looper.endPattern);

	while(looper.DrumState == DRUMS_STARTED && looper.startPattern < (looper.startPattern + 1)){

			looper.StartLooper = TRUE;

			if(switch_buff == FALSE){
					updatePatternTime(&pat1,&tim1);
					timptr = &tim1;
					patptr = &pat1;
					drumBuffPtr = drumBuffA;
					menuShowStatus();
					if(looper.startPattern == looper.endPattern)
						goto wait_first_beat;
					readSRAM((uint8_t *)&pat2,sizeof(Pattern));
					setPatternTime(&pat2,&tim2);
					readSRAM((uint8_t *)drumBuffB,tim2.subbeats * 5);
					switch_buff = TRUE;
				}
				else{
					updatePatternTime(&pat2,&tim2);
					timptr = &tim2;
					patptr = &pat2;
					drumBuffPtr = drumBuffB;
					if(looper.startPattern == looper.endPattern)
						goto wait_first_beat;
					readSRAM((uint8_t *)&pat1,sizeof(Pattern));
					setPatternTime(&pat1,&tim1);
					readSRAM((uint8_t *)drumBuffA,tim1.subbeats * 5);
					switch_buff = FALSE;
				}

				wait_first_beat:
				sprintf(lcdline," Playing bar: %-4u",(unsigned int)(looper.startPattern + 1));
				TM_ILI9341_Puts(10, 150, lcdline, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);

				while(first_beat == FALSE){
					if(looper.DrumState == DRUMS_PAUSED || looper.DrumState == DRUMS_STOPPED)
						goto end_drum_loop;
					continue;
				}

				first_beat = FALSE;

				looper.startPattern++;

				if(looper.startPattern == (looper.endPattern + 1)){
					if(looper.Recording == TRUE){
						looper.Recording = FALSE;
						goto end_drum_loop;
					}

					looper.startPattern = tmp;	// restore original start pattern number
					// setStartEndPatterns in read_sample and read_samples
					switch_buff = FALSE;
					first_beat = FALSE;
					seekPattern(pattern_audio_map,looper.startPattern);

				}

		}


		end_drum_loop:
		stopDrums();
		looper.startPattern = tmp;
		looper.StartLooper = FALSE;
		looper.Playback = FALSE;
		looper.Recording = FALSE;

}

void readDrums(uint32_t *numOfPatterns,uint32_t *numOfBytes,uint32_t *maxResolution){
	Pattern tmp;
	uint32_t header[3];		// number of patterns, number of bytes, max. resolution
	//uint32_t (*map)[2] = NULL;
	uint32_t currPat = 0;
	switch_buff = FALSE;
	first_beat = FALSE;

	//f_read(fil,header,sizeof(header),&bytesRead);
	SRAM_seekRead(0,SRAM_SET);
	readSRAM((uint8_t *)header,sizeof(header));
	*numOfBytes = header[HEADER_NUM_BYTES];
	*numOfPatterns = header[HEADER_NUM_PATTS];
	*maxResolution = header[HEADER_MAX_BEATS];

	if(*numOfPatterns == 0 || *maxResolution > MAX_SUBBEATS)
		return;

	// create memory map
	for(currPat = 0; currPat < *numOfPatterns; currPat++){
		pattern_audio_map[currPat][0] = SRAM_readerPosition();
		readSRAM((uint8_t *)&tmp,sizeof(Pattern));
		readSRAM((uint8_t *)drumBuffA,tmp.beats * tmp.division * 5);
		if(currPat < *numOfPatterns){
			uint32_t millis = BEAT_MILLIS(tmp.beattime);
			pattern_audio_map[currPat + 1][1] = pattern_audio_map[currPat][1] + (millis * tmp.beats * 15);
		}
	}

	//return (uint32_t *)map;

}


void stopDrums(){
	looper.DrumState = DRUMS_STOPPED;
	HAL_TIM_Base_Stop_IT(&htim2);
	HAL_Delay(100);

}

void setPatternTime(__IO Pattern *p,__IO DrumTimes *t){
	uint32_t millis = BEAT_MILLIS(p->beattime);
	t->subbeats = p->beats * p->division;
	t->barDuration = p->beats * millis;
	t->remainder = t->barDuration % t->subbeats;
	t->subBeatDuration = t->barDuration / t->subbeats;
}

void updatePatternTime(__IO Pattern *p,__IO DrumTimes *t){
	uint32_t millis = BEAT_MILLIS(p->beattime + looper.timeIncrement) ;
	t->subbeats = p->beats * p->division;
	t->barDuration = p->beats * millis;
	t->remainder = t->barDuration % t->subbeats;
	t->subBeatDuration = t->barDuration / t->subbeats;
}



void midiDrumHandler(){
	uint32_t i;

	if(looper.DrumState != DRUMS_STARTED){
		return;
	}

	if(midiDrumClock < timptr->barDuration){
		if(midiDrumClock % ((timptr->remainder > 0 && drumBeatIndex == NUM_ALL_TRACKS)?(timptr->subBeatDuration + timptr->remainder):timptr->subBeatDuration) == 0){
			for(i = drumBeatIndex; i < drumBeatIndex + NUM_DRUM_TRACKS; i++){
				if(drumBuffPtr[i] != 0 && looper.DrumState == DRUMS_STARTED)
					playPercussion(NOTEON,drumBuffPtr[i]);
			}

			if(drumBuffPtr[i] != 0 && looper.DrumState == DRUMS_STARTED)
				playBass(NOTEON,drumBuffPtr[i]);

			drumBeatIndex += NUM_ALL_TRACKS;

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


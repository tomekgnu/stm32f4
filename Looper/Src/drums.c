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
#include "stm32f429i_discovery.h"
#include "keyclick.h"
#include "string.h"

extern BOOL Skip_Read_Button;

static BOOL metronomeUpdated = FALSE;
static __IO uint32_t drumBeatIndex = 0;
static __IO uint32_t drumBufferIndex = 0;
static __IO uint16_t midiDrumClock;
static __IO BOOL switch_buff;
static __IO BOOL first_beat;

uint8_t key_to_drum_part[16][2] = {
		{Acoustic_Bass_Drum,R_FOOT},
		{Side_Stick,R_HAND},
		{Acoustic_Snare,R_HAND},
		{Cowbell,L_HAND},
		{Low_Floor_Tom,L_HAND},
		{High_Floor_Tom,L_HAND},
		{Low_Mid_Tom,L_HAND},
		{Hi_Mid_Tom,L_HAND},
		{High_Tom,L_HAND},
		{Closed_Hi_Hat,R_HAND},
		{Open_Hi_Hat,R_HAND},
		{Pedal_Hi_Hat,L_FOOT},
		{Crash_Cymbal_1,L_HAND},
		{Ride_Cymbal_2,R_HAND},
		{Splash_Cymbal,L_HAND},
		{Chinese_Cymbal,R_HAND}
};


PatternData pattern_audio_map[MAX_PATTERNS + 1];
uint8_t drumBuffA[MAX_SUBBEATS * NUM_ALL_TRACKS];
uint8_t drumBuffB[MAX_SUBBEATS * NUM_ALL_TRACKS];
uint32_t drumEventTimes[MAX_SUBBEATS];
uint8_t * drumBuffReadPtr;
uint8_t * drumBuffWritePtr;

__IO PatternBeats pat1;
__IO PatternBeats pat2;
__IO PatternTimes tim1;
__IO PatternTimes tim2;
__IO PatternTimes *timptr;
__IO PatternBeats *patptr;

static inline uint8_t key_index(uint16_t code){
	switch(code){
	case 1:	return 0;
	case 2: return 1;
	case 4: return 2;
	case 8:	return 3;
	case 16: return 4;
	case 32: return 5;
	case 64: return 6;
	case 128: return 7;
	case 256: return 8;
	case 512: return 9;
	case 1024: return 10;
	case 2048: return 11;
	case 32768: return 12;
	case 4096: return 13;
	case 8192: return 14;
	case 16384: return 15;

	}

}

static void seekPattern(PatternData *pattern_audio_map,uint32_t ind){
	switch_buff = FALSE;
	SRAM_seekRead(pattern_audio_map[ind].sram_position,SRAM_SET);
	readSRAM((uint8_t *)&pat1,sizeof(PatternBeats));
	setPatternTime(&pat1,&tim1);
	readSRAM((uint8_t *)drumBuffA,tim1.subbeats * 5);

}

void updateMetronome(){
	metronomeUpdated = TRUE;
}
void drumAudioSync(){
	//looper.SampleOffset = pattern_audio_map[looper.StartPattern].sample_position;
	//sdram_pointer = looper.SampleOffset * 2;
	//looper.SampleCount = pattern_audio_map[looper.EndPattern + 1].sample_position;
}

void drumLoop(){
	uint32_t tmp = looper.StartPattern;
	switch_buff = FALSE;
	first_beat = FALSE;
	seekPattern(pattern_audio_map,looper.StartPattern);
	resetDrums();
	HAL_TIM_Base_Start_IT(&htim2);
	looper.Function = AUDIO_DRUMS;
	looper.DrumState = DRUMS_STARTED;
	//setStartEndPatterns(looper.StartPattern,looper.EndPattern);

	while(looper.DrumState == DRUMS_STARTED && looper.StartPattern < (looper.EndPattern + 1)){

			looper.StartLooper = TRUE;

			if(switch_buff == FALSE){
					updatePatternTime(&pat1,&tim1);
					timptr = &tim1;
					patptr = &pat1;
					drumBuffReadPtr = drumBuffA;
					menuShowStatus();
					if(looper.StartPattern == looper.EndPattern)
						goto wait_first_beat;
					readSRAM((uint8_t *)&pat2,sizeof(PatternBeats));
					setPatternTime(&pat2,&tim2);
					readSRAM((uint8_t *)drumBuffB,tim2.subbeats * NUM_ALL_TRACKS);
					switch_buff = TRUE;
				}
				else{
					updatePatternTime(&pat2,&tim2);
					timptr = &tim2;
					patptr = &pat2;
					drumBuffReadPtr = drumBuffB;
					if(looper.StartPattern == looper.EndPattern)
						goto wait_first_beat;
					readSRAM((uint8_t *)&pat1,sizeof(PatternBeats));
					setPatternTime(&pat1,&tim1);
					readSRAM((uint8_t *)drumBuffA,tim1.subbeats * NUM_ALL_TRACKS);
					switch_buff = FALSE;
				}

				wait_first_beat:
				sprintf(lcdline," Playing bar: %-4u",(unsigned int)(looper.StartPattern + 1));
				TM_ILI9341_Puts(10, 150, lcdline, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);

				while(first_beat == FALSE){
					if(looper.DrumState == DRUMS_PAUSED || looper.DrumState == DRUMS_STOPPED)
						goto end_drum_loop;
					continue;
				}

				first_beat = FALSE;
				if(looper.Recording == TRUE)
					pattern_audio_map[looper.StartPattern].channel_recorded[ACTIVE_CHANNEL_INDEX] = TRUE;
				looper.StartPattern++;

				if(looper.StartPattern == (looper.EndPattern + 1)){
					if(looper.Recording == TRUE){
						looper.Recording = FALSE;
						goto end_drum_loop;
					}

					looper.StartPattern = tmp;	// restore original start pattern number
					// setStartEndPatterns in read_sample and read_samples
					switch_buff = FALSE;
					first_beat = FALSE;
					seekPattern(pattern_audio_map,looper.StartPattern);

				}

		}


		end_drum_loop:
		stopDrums();
		looper.StartPattern = tmp;
		looper.StartLooper = FALSE;
		looper.Playback = FALSE;
		looper.Recording = FALSE;
		BSP_LED_Off(LED_GREEN);
		BSP_LED_Off(LED_RED);
}

void save_first(){
	uint32_t header[3];
	SRAM_seekWrite(0,SRAM_SET);
	header[HEADER_NUM_BYTES] = sizeof(header) + sizeof(PatternBeats) + pat1.beats * pat1.division * NUM_ALL_TRACKS;
	header[HEADER_NUM_PATTS] = 1;
	header[HEADER_MAX_BEATS] = tim1.subbeats;
	writeSRAM((uint8_t *)header,sizeof(header));
	writeSRAM((uint8_t *)&pat1,sizeof(PatternBeats));
	writeSRAM((uint8_t *)drumBuffA,pat1.beats * pat1.division * NUM_ALL_TRACKS);
}

void save_next(){


}

void readDrums(uint32_t *numOfPatterns,uint32_t *numOfBytes,uint32_t *maxResolution){
	PatternBeats tmp;
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
	pattern_audio_map[0].sample_position = 0;
	pattern_audio_map[0].channel_recorded[_CH1] = FALSE;
	pattern_audio_map[0].channel_recorded[_CH2] = FALSE;
	for(currPat = 0; currPat < *numOfPatterns; currPat++){
		pattern_audio_map[currPat].channel_recorded[_CH1] = FALSE;
		pattern_audio_map[currPat].channel_recorded[_CH2] = FALSE;
		pattern_audio_map[currPat].sram_position = SRAM_readerPosition();
		readSRAM((uint8_t *)&tmp,sizeof(PatternBeats));
		readSRAM((uint8_t *)drumBuffA,tmp.beats * tmp.division * NUM_ALL_TRACKS);
		if(currPat < *numOfPatterns){
			uint32_t millis = BEAT_MILLIS(tmp.beattime);
			pattern_audio_map[currPat + 1].sample_position = pattern_audio_map[currPat].sample_position + (millis * tmp.beats * 15);
		}
	}

	//return (uint32_t *)map;

}

void startDrums(){
	HAL_TIM_Base_Start_IT(&htim2);
	looper.DrumState = DRUMS_STARTED;
}

void stopDrums(){
	looper.DrumState = DRUMS_STOPPED;
	HAL_TIM_Base_Stop_IT(&htim2);
	HAL_Delay(100);

}

void setPatternTime(__IO PatternBeats *p,__IO PatternTimes *t){
	uint32_t millis = BEAT_MILLIS(p->beattime);
	t->subbeats = p->beats * p->division;
	t->barDuration = p->beats * millis;
	t->remainder = t->barDuration % t->subbeats;
	t->subBeatDuration = t->barDuration / t->subbeats;
}

void updatePatternTime(__IO PatternBeats *p,__IO PatternTimes *t){
	uint32_t millis = BEAT_MILLIS(p->beattime + looper.timeIncrement) ;
	t->subbeats = p->beats * p->division;
	t->barDuration = p->beats * millis;
	t->remainder = t->barDuration % t->subbeats;
	t->subBeatDuration = t->barDuration / t->subbeats;
}


void readDrumKeyboard(BOOL record){
	//static uint16_t lastKeys = 0;
	//uint16_t currentKeys = read_shift_regs();
//	if(currentKeys != lastKeys && currentKeys != 0){
//		if(looper.DrumState != DRUMS_STARTED){
//			midiDrumClock = 0;
//			drumBeatIndex = 0;
//			drumBufferIndex = 0;
//			looper.DrumState = DRUMS_STARTED;
//		}
//
//		drumBuffWritePtr[drumBufferIndex] = key_index(currentKeys);	// numbers are resolved to drums and parts using key_to_drum_part array
//		playPercussion(NOTEON,key_to_drum_part[drumBuffWritePtr[drumBufferIndex]][0]);
//		drumEventTimes[drumBufferIndex] = midiDrumClock;
//		drumBufferIndex++;
//
//	}

	//lastKeys = currentKeys;
	TM_KEYPAD_Button_t key = TM_KEYPAD_Read();
	if(key != TM_KEYPAD_Button_NOPRESSED){
		if(record == FALSE){
			playPercussion(NOTEON,key_to_drum_part[key][0]);
			return;
		}
		if(looper.DrumState != DRUMS_STARTED){
					resetDrums();
					looper.DrumState = DRUMS_STARTED;
				}

				drumBuffWritePtr[drumBufferIndex] = key;	// numbers are resolved to drums and parts using key_to_drum_part array
				playPercussion(NOTEON,key_to_drum_part[drumBuffWritePtr[drumBufferIndex]][0]);
				drumEventTimes[drumBufferIndex] = midiDrumClock;
				drumBufferIndex++;
				BSP_LED_On(LED_RED);
	}

}

void midiDrumHandler(){
	uint32_t i;

	if(looper.DrumState != DRUMS_STARTED && looper.DrumState != DRUMS_PAUSED){
		return;
	}

	readDrumKeyboard(TRUE);

	if(midiDrumClock < timptr->barDuration){
		if(midiDrumClock % ((timptr->remainder > 0 && drumBeatIndex == NUM_ALL_TRACKS)?(timptr->subBeatDuration + timptr->remainder):timptr->subBeatDuration) == 0){
			if(looper.Metronome == TRUE && drumBeatIndex % patptr->beats == 0)
				playPercussion(NOTEON,Metronome_Click);
			for(i = drumBeatIndex; i < drumBeatIndex + NUM_DRUM_TRACKS; i++){
				if(drumBuffReadPtr[i] != 0 && looper.DrumState == DRUMS_STARTED)
					playPercussion(NOTEON,drumBuffReadPtr[i]);
			}

			if(drumBuffReadPtr[i] != 0 && looper.DrumState == DRUMS_STARTED)
				playBass(NOTEON,drumBuffReadPtr[i]);

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

void clear_drums(){
	memset(drumBuffA,0,MAX_SUBBEATS * NUM_ALL_TRACKS);
	memset(drumBuffB,0,MAX_SUBBEATS * NUM_ALL_TRACKS);
	memset(drumEventTimes,0,MAX_SUBBEATS * 4);
}

void preview_drums(){
	menuClearLines(1,4);
	menuMultiLine(5,30,"Press keys to hear drums.","Use joystick to change","drum sound assignment","to specific keys","Press blue button to finish.");
	looper.DrumState = DRUMS_PAUSED;
	while(looper.DrumState == DRUMS_PAUSED){
		readDrumKeyboard(FALSE);
	}

}
void play_drums(){
	looper.Metronome = FALSE;
	HAL_TIM_Base_Start_IT(&htim2);
	resetDrums();
	looper.DrumState = DRUMS_STARTED;
	while(looper.DrumState == DRUMS_STARTED)
		continue;
	stopDrums();

}

void record_drums(){
		uint32_t barMillis;
		uint8_t drum,part;
		static uint32_t beattime = 60;
		drumBeatIndex = 0;
		midiDrumClock = 0;
		drumBufferIndex = 0;
		pat1.beattime = beattime;
		pat1.division = 4;
		pat1.beats = 4;
		drumBuffReadPtr = drumBuffA;
		drumBuffWritePtr = drumBuffB;
		timptr = &tim1;
		patptr = &pat1;
		looper.Metronome = TRUE;
		setPatternTime(&pat1,&tim1);

		HAL_TIM_Base_Start_IT(&htim2);

		while(TRUE){
			// metronome is ticking
			// pressing drum keyboard starts recording and sets clocks to 0
			while(looper.DrumState != DRUMS_STARTED){
				  if(metronomeUpdated == TRUE){
					  updatePatternTime(&pat1,&tim1);
					  metronomeUpdated = FALSE;
				  }
				  if(looper.DrumState == DRUMS_STOPPED)	// blue button pressed
					  goto end_drum_record;
			}
		// recording loop and playback: drum events added to buffer in readDrumKeyboard(), called in midiDrumHandler()
		  while(midiDrumClock < tim1.barDuration){
			  if(metronomeUpdated == TRUE){
				  updatePatternTime(&pat1,&tim1);
				  metronomeUpdated = FALSE;
			  }
			  if(looper.DrumState == DRUMS_STOPPED)	// blue button pressed
				  goto end_drum_record;
			  continue;
		  }

		  drumBuffB[drumBufferIndex] = No_Event;
		  looper.DrumState = DRUMS_STOPPED;
		  resetDrums();
		  BSP_LED_Off(LED_RED);
		  BSP_LED_On(LED_GREEN);
		  // add new drum events if any
		  while(drumBuffB[drumBufferIndex] != No_Event){
			  for(barMillis = 0; barMillis < tim1.barDuration; barMillis += tim1.subBeatDuration){
				  if(drumEventTimes[drumBufferIndex] >= barMillis && drumEventTimes[drumBufferIndex] < (barMillis + tim1.subBeatDuration)){
					  drumBeatIndex = barMillis / tim1.subBeatDuration;
					  drum = key_to_drum_part[drumBuffB[drumBufferIndex]][0];
					  part = key_to_drum_part[drumBuffB[drumBufferIndex]][1];
					  if(drumEventTimes[drumBufferIndex] < (barMillis + tim1.subBeatDuration / 2))
						  drumBuffA[drumBeatIndex * 5 + part] = drum;
					  else
						  drumBuffA[drumBeatIndex * 5 + 5 + part] = drum;
				  }
			  }

			  drumBufferIndex++;
		  }

		 resetDrums();
		 looper.DrumState = DRUMS_STARTED;

	  }

	  end_drum_record:
	  stopDrums();
	  beattime += looper.timeIncrement;
	  looper.timeIncrement = 0;
	  Skip_Read_Button = TRUE;
	  return;
}

void resetDrums(){
	midiDrumClock = 0;
	drumBeatIndex = 0;
	drumBufferIndex = 0;
}


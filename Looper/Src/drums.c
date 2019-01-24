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
static uint32_t beattime = 60;
static BOOL metronomeUpdated = FALSE;
static __IO uint32_t drumBeatIndex = 0;
static __IO uint32_t drumBufferIndex = 0;
static __IO uint16_t midiDrumClock;
static __IO BOOL switch_buff;
static __IO BOOL first_beat;

char *drum_names[] = {"Bass drum","Side stick","Snare",
		"Cowbell","Low floor tom","High floor tom","Low mid tom","Hi mid tom","High tom","Closed hi-hat","Open hi-hat","Pedal hi-hat","Crash","Ride","Splash","Chinese",
};

char *part_names[] = {"[left hand]","[right hand]","[left foot]","[right foot]"};
uint8_t drum_midi_values[] = {Acoustic_Bass_Drum,Side_Stick,Acoustic_Snare,Cowbell,Low_Floor_Tom,High_Floor_Tom,
		Low_Mid_Tom,Hi_Mid_Tom,High_Tom,Closed_Hi_Hat,Open_Hi_Hat,Pedal_Hi_Hat,Crash_Cymbal_1,Ride_Cymbal_2,Splash_Cymbal,Chinese_Cymbal};

uint8_t drum_parts[16][2] = {
		{L_FOOT,R_FOOT},{L_HAND,R_HAND},{L_HAND,R_HAND},{L_HAND,R_HAND},
		{L_HAND,R_HAND},{L_HAND,R_HAND},{L_HAND,R_HAND},{L_HAND,R_HAND},
		{L_HAND,R_HAND},{L_HAND,R_HAND},{L_HAND,R_HAND},{L_FOOT,R_FOOT},
		{L_HAND,R_HAND},{L_HAND,R_HAND},{L_HAND,R_HAND},{L_HAND,R_HAND},

};


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
	uint32_t currentBytes;
	uint32_t header[3];		// number of patterns, number of bytes, max. resolution
	SRAM_seekRead(0,SRAM_SET);
	SRAM_seekWrite(0,SRAM_SET);
	readSRAM((uint8_t *)header,sizeof(header));
	currentBytes = header[HEADER_NUM_BYTES];
	if(currentBytes == 0){
		save_first();
		return;
	}
	header[HEADER_NUM_BYTES] += (sizeof(PatternBeats) + pat1.beats * pat1.division * NUM_ALL_TRACKS);
	header[HEADER_NUM_PATTS]++;
	if(tim1.subbeats > header[HEADER_MAX_BEATS])
		header[HEADER_MAX_BEATS] = tim1.subbeats;
	writeSRAM((uint8_t *)header,sizeof(header));
	SRAM_seekWrite(currentBytes,SRAM_SET);
	writeSRAM((uint8_t *)&pat1,sizeof(PatternBeats));
	writeSRAM((uint8_t *)drumBuffA,pat1.beats * pat1.division * NUM_ALL_TRACKS);
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


TM_KEYPAD_Button_t readDrumKeyboard(BOOL record){

	TM_KEYPAD_Button_t key = TM_KEYPAD_Read();
	if(key != TM_KEYPAD_Button_NOPRESSED){
		if(record == TRUE){
			if(looper.DrumState != DRUMS_STARTED){
					resetDrums();
					looper.DrumState = DRUMS_STARTED;
			}

			drumBuffWritePtr[drumBufferIndex] = key;	// numbers are resolved to drums and parts using key_to_drum_part array
			drumEventTimes[drumBufferIndex] = midiDrumClock;
			drumBufferIndex++;
			BSP_LED_On(LED_RED);
		}

		if(looper.PlayBass == FALSE)
			playPercussion(NOTEON,key_to_drum_part[key][0]);
		else
			playBass(NOTEON,28 + key);	// Bass lowest E midi code = 28, each key adds half-tone
	}

	return key;

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

void preview_drums() {
	uint16_t x, y, i;
	JOYSTICK js;
	TM_KEYPAD_Button_t currentkey = TM_KEYPAD_Button_NOPRESSED, tmpkey =
			TM_KEYPAD_Button_NOPRESSED;
	uint8_t joydrumkey = 0, joypartkey = 0;
	BOOL pressed = FALSE, changed = FALSE;
	//menuClearLines(2,4,6);

	TM_ILI9341_Fill(ILI9341_COLOR_MAGENTA);
	menuMultiLine(3, 180, "Press keys to hear drums.", "Change with joystick",
			"Blue button to finish.");

	for (x = 10; x < 160; x += 42) {
		for (y = 10; y < 160; y += 42) {
			TM_ILI9341_DrawFilledRectangle(x, y, x + 40, y + 40,ILI9341_COLOR_BLUE2);
		}
	}

	looper.DrumState = DRUMS_PAUSED;
	while (looper.DrumState == DRUMS_PAUSED) {
		if ((currentkey = readDrumKeyboard(FALSE))
				!= TM_KEYPAD_Button_NOPRESSED) {
			switch (currentkey) {
			case TM_KEYPAD_Button_0:
				y = 136;
				x = 52;
				break;
			case TM_KEYPAD_Button_1:
				y = 10;
				x = 10;
				break;
			case TM_KEYPAD_Button_2:
				y = 10;
				x = 52;
				break;
			case TM_KEYPAD_Button_3:
				y = 10;
				x = 94;
				break;
			case TM_KEYPAD_Button_4:
				y = 52;
				x = 10;
				break;
			case TM_KEYPAD_Button_5:
				y = 52;
				x = 52;
				break;
			case TM_KEYPAD_Button_6:
				y = 52;
				x = 94;
				break;
			case TM_KEYPAD_Button_7:
				y = 94;
				x = 10;
				break;
			case TM_KEYPAD_Button_8:
				y = 94;
				x = 52;
				break;
			case TM_KEYPAD_Button_9:
				y = 94;
				x = 94;
				break;
			case TM_KEYPAD_Button_A:
				y = 10;
				x = 136;
				break;
			case TM_KEYPAD_Button_B:
				y = 52;
				x = 136;
				break;
			case TM_KEYPAD_Button_C:
				y = 94;
				x = 136;
				break;
			case TM_KEYPAD_Button_D:
				y = 136;
				x = 136;
				break;
			case TM_KEYPAD_Button_STAR:
				y = 136;
				x = 10;
				break;
			case TM_KEYPAD_Button_HASH:
				y = 136;
				x = 94;
				break;

			}
			changed = FALSE;
			pressed = TRUE;
			tmpkey = currentkey;
			for (i = 0; key_to_drum_part[currentkey][0] != drum_midi_values[i];i++)
				continue;
			sprintf(lcdline,"%s ",drum_names[i]);
			joydrumkey = i;
			if (key_to_drum_part[currentkey][1] == drum_parts[currentkey][0])
				joypartkey = 0;
			else
				joypartkey = 1;
			strcat(lcdline, part_names[key_to_drum_part[currentkey][1]]);
			TM_ILI9341_DrawFilledRectangle(x, y, x + 40, y + 40,
					ILI9341_COLOR_RED);	// square on keyboard
			TM_ILI9341_DrawFilledRectangle(10, 180, 320, 240,
					ILI9341_COLOR_MAGENTA);	// clear text field
			TM_ILI9341_Puts(10, 180, lcdline, &TM_Font_11x18,
					ILI9341_COLOR_WHITE, ILI9341_COLOR_MAGENTA);	// put text
		}

		else {
			if (pressed == TRUE) {
				TM_ILI9341_DrawFilledRectangle(x, y, x + 40, y + 40,ILI9341_COLOR_BLUE2);
				pressed = FALSE;
			}
			if (Active_Joystick() == TRUE) {
				js = Read_Joystick();
				if (js.xpos == MAX_JOY && joydrumkey < 15) {
					joydrumkey++;
					changed = TRUE;
				}
				if (js.xpos == MIN_JOY && joydrumkey > 0) {
					joydrumkey--;
					changed = TRUE;
				}

				if (js.ypos == MAX_JOY) {
					joypartkey = 1;
					changed = TRUE;
				} else if (js.ypos == MIN_JOY) {
					joypartkey = 0;
					changed = TRUE;
				}

				// assign new values in key_to_drum_part[]
				if (changed == TRUE) {
					sprintf(lcdline, "%s ", drum_names[joydrumkey]);
					strcat(lcdline,part_names[drum_parts[joydrumkey][joypartkey]]);
					TM_ILI9341_DrawFilledRectangle(10, 180, 320, 240,ILI9341_COLOR_MAGENTA);	// clear text field
					TM_ILI9341_Puts(10, 180, lcdline, &TM_Font_11x18,ILI9341_COLOR_WHITE, ILI9341_COLOR_MAGENTA);// put text
					key_to_drum_part[tmpkey][0] = drum_midi_values[joydrumkey];
					key_to_drum_part[tmpkey][1] = drum_parts[joydrumkey][joypartkey];
					changed = FALSE;
					HAL_Delay(100);
				}
			}

		}

	}

}

void play_drums() {
	looper.Metronome = FALSE;
	HAL_TIM_Base_Start_IT(&htim2);
	resetDrums();
	pat1.beattime = beattime;
	updatePatternTime(&pat1, &tim1);
	looper.DrumState = DRUMS_STARTED;
	while(TRUE) {
		if (metronomeUpdated == TRUE) {
			updatePatternTime(&pat1, &tim1);
			metronomeUpdated = FALSE;
		}
		if(looper.DrumState == DRUMS_STOPPED)
			break;
	}

	stopDrums();
	beattime += looper.timeIncrement;
}



void record_drums(){
		uint32_t barMillis;
		uint8_t drum,part;
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
					  if(looper.PlayBass == FALSE){
						  drum = key_to_drum_part[drumBuffB[drumBufferIndex]][0];
						  part = key_to_drum_part[drumBuffB[drumBufferIndex]][1];
					  }
					  else{
						  drum = 28 + drumBuffB[drumBufferIndex];
						  part = BASS;
					  }
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


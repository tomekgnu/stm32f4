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

extern TM_KEYPAD_Button_t Keypad_Button;


void readDrums(FIL *fil){

	uint32_t numOfPatterns;
	uint32_t numOfBytes;
	uint32_t maxResolution;
	uint32_t header[3];		// number of patterns, number of bytes, max. resolution
	uint32_t currPat;
	uint32_t (*map)[2];
	BOOL start = FALSE;
	switch_buff = FALSE;
	first_beat = FALSE;

	//f_read(fil,header,sizeof(header),&bytesRead);
	SRAM_seekRead(0,SRAM_SET);
	readSRAM((uint8_t *)header,sizeof(header));
	numOfBytes = header[NUM_OF_BYTES];
	numOfPatterns = header[NUM_OF_PATTERNS];
	maxResolution = header[MAX_RESOLUTION];

	map = malloc(numOfPatterns * 2);

	// create memory map
	for(currPat = 0; currPat < numOfPatterns; currPat++){
		map[currPat][0] = SRAM_readerPosition();
		readSRAM((uint8_t *)&pat1,sizeof(Pattern));
		readSRAM((uint8_t *)drumBuffA,pat1.beats * pat1.division * 5);
	}

	// buffers to store data read from memory
	drumBuffA = (uint8_t *)malloc(DRUM_TIM_HDR_SIZE + DRUM_INSTR * maxResolution);
	drumBuffB = (uint8_t *)malloc(DRUM_TIM_HDR_SIZE + DRUM_INSTR * maxResolution);

	//if(f_eof(fil))
		//return;
	currPat = 0;
	SRAM_seekRead(map[currPat][0],SRAM_SET);


	while (start == FALSE){
		Keypad_Button = TM_KEYPAD_Read();
		switch(Keypad_Button){
			case TM_KEYPAD_Button_5:	start = TRUE; break;
			case TM_KEYPAD_Button_0:	return;
			case TM_KEYPAD_Button_6:	if(currPat < (numOfPatterns - 1))
											currPat++;
										SRAM_seekRead(map[currPat][0],SRAM_SET);
										sprintf(lcdline,"%u bar",(unsigned int)(currPat + 1));
										TM_ILI9341_Puts(10, 100, lcdline, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
										break;
			case TM_KEYPAD_Button_4:	if(currPat > 0)
											currPat--;
										SRAM_seekRead(map[currPat][0],SRAM_SET);
										sprintf(lcdline,"%u bar",(unsigned int)(currPat + 1));
										TM_ILI9341_Puts(10, 100, lcdline, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
										break;

		}
	}
	readSRAM((uint8_t *)&pat1,sizeof(Pattern));
	//f_read(fil,&pat1,sizeof(Pattern),(UINT *)&bytesRead);
	setPatternTime(&pat1,&tim1);
	readSRAM((uint8_t *)drumBuffA,tim1.numberOfBeats * 5);
	//f_read(fil,drumBuffA,tim1.numberOfBeats * 5,(UINT *)&bytesRead);

	resetDrums();
	HAL_TIM_Base_Start_IT(&htim2);

	TM_ILI9341_DrawFilledRectangle(10,10,480,48,ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(10, 10,"[User button] Stop", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	looper.DrumState = DRUMS_STARTED;


	while((looper.DrumState == DRUMS_STARTED || looper.DrumState == DRUMS_PAUSED) && currPat < numOfPatterns){
		while(looper.DrumState == DRUMS_PAUSED)
			continue;
		//currByte = f_tell(fil);
		++currPat;
		//if(currPat < numOfPatterns)
			//map[currPat][0] = currByte;

		if(switch_buff == FALSE){
				updatePatternTime(&pat1,&tim1);
				timptr = &tim1;
				patptr = &pat1;
				drumBuffPtr = drumBuffA;
				if(currPat == numOfPatterns)
					goto wait_first_beat;
				readSRAM((uint8_t *)&pat2,sizeof(Pattern));
				//f_read(fil,&pat2,sizeof(Pattern),(UINT *)&bytesRead);
				setPatternTime(&pat2,&tim2);
				readSRAM((uint8_t *)drumBuffB,tim2.numberOfBeats * 5);
				//f_read(fil,drumBuffB,tim2.numberOfBeats * 5,(UINT *)&bytesRead);
				switch_buff = TRUE;
			}
			else{
				updatePatternTime(&pat2,&tim2);
				timptr = &tim2;
				patptr = &pat2;
				drumBuffPtr = drumBuffB;
				if(currPat == numOfPatterns)
					goto wait_first_beat;
				readSRAM((uint8_t *)&pat1,sizeof(Pattern));
				//f_read(fil,&pat1,sizeof(Pattern),(UINT *)&bytesRead);
				setPatternTime(&pat1,&tim1);
				readSRAM((uint8_t *)drumBuffA,tim1.numberOfBeats * 5);
				//f_read(fil,drumBuffA,tim1.numberOfBeats * 5,(UINT *)&bytesRead);
				switch_buff = FALSE;
			}


			wait_first_beat:
			sprintf(lcdline,"%u bar",(unsigned int)currPat);
			TM_ILI9341_Puts(10, 100, lcdline, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
			while((looper.DrumState == DRUMS_STARTED || looper.DrumState == DRUMS_PAUSED) && first_beat == FALSE){
				continue;
			}
			first_beat = FALSE;
			//if(currPat < numOfPatterns)
				//map[currPat][1] = ch1.SamplesWritten;

			if(currPat == numOfPatterns){
				currPat = 0;
				switch_buff = FALSE;
				first_beat = FALSE;
				SRAM_seekRead(map[currPat][0],SRAM_SET);
				readSRAM((uint8_t *)&pat1,sizeof(Pattern));
				//f_read(fil,&pat1,sizeof(Pattern),(UINT *)&bytesRead);
				setPatternTime(&pat1,&tim1);
				readSRAM((uint8_t *)drumBuffA,tim1.numberOfBeats * 5);
			}

	}

	stopDrums();
	looper.StartLooper = FALSE;
	looper.Playback = FALSE;
	looper.Recording = FALSE;

	free(drumBuffA);
	free(drumBuffB);
	free(map);
}

void stopDrums(){
	looper.DrumState = DRUMS_STOPPED;
	HAL_TIM_Base_Stop_IT(&htim2);
	HAL_Delay(100);

}

void setPatternTime(__IO Pattern *p,__IO DrumTimes *t){
	int beatTimeMillis = (60000 / p->beattime) / p->division;
	t->numberOfBeats = p->beats * p->division;
	t->barDuration = t->numberOfBeats * beatTimeMillis;
	t->remainder = t->barDuration % t->numberOfBeats;
	t->beatDuration = t->barDuration / t->numberOfBeats;
}

void updatePatternTime(__IO Pattern *p,__IO DrumTimes *t){
	int beatTimeMillis = ((60000 / p->beattime) / p->division) + looper.timeIncrement;
	t->numberOfBeats = p->beats * p->division;
	t->barDuration = t->numberOfBeats * beatTimeMillis;
	t->remainder = t->barDuration % t->numberOfBeats;
	t->beatDuration = t->barDuration / t->numberOfBeats;
}

void midiDrumHandler(){
	uint32_t i;
	if(looper.DrumState != DRUMS_STARTED)
		return;

	if(midiDrumClock < timptr->barDuration){
		if(midiDrumClock % ((timptr->remainder > 0 && drumBeatIndex == NUM_ALL_TRACKS)?(timptr->beatDuration + timptr->remainder):timptr->beatDuration) == 0){
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


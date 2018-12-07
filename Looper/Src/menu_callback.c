/*
 * menu_callback.c
 *
 *  Created on: 29.07.2018
 *      Author: Tomek
 */
#include "stdio.h"
#include "ff.h"
#include "main.h"
#include "menu.h"
#include "memops.h"
#include "drums.h"
#include "tm_stm32f4_keypad.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32_hd44780.h"
#include "menu_callback.h"
#include "stdlib.h"
#include "ads1256_test.h"
#include "audio.h"
#include "stm32f429i_discovery.h"

extern TM_KEYPAD_Button_t Keypad_Button;
extern BOOL Skip_Read_Button;

void audio_rhythm(){
	looper.Function = AUDIO_DRUMS;

}

void audio_only(){
	looper.Function = AUDIO_ONLY;

}

void drums_only(void){
	looper.Function = DRUMS_ONLY;
}

void audio_drums(void){
	looper.Function = AUDIO_DRUMS;
}
void print_letters(void) {

	int c = '0';
	sprintf(lcdline, "%c", c);
	TM_HD44780_Puts(0, 1, lcdline);
	while ((Keypad_Button = TM_KEYPAD_Read()) != TM_KEYPAD_Button_0) {
		if (Keypad_Button == TM_KEYPAD_Button_5) {
			c++;
			sprintf(lcdline, "%c", c);
			TM_HD44780_Puts(0, 1, lcdline);
		}

	}

	Skip_Read_Button = TRUE;

}

void download_rhythm() {
	looper.Function = DOWNLOAD_SRAM;
	menuMultiLine(2,110,"Press [Send via USB] button","in Rhythm application.");
	SRAM_download_rhythm();
	looper.Function = NONE;
	Skip_Read_Button = TRUE;
}


void select_loops(){
	looper.Function = AUDIO_ONLY;
	looper.StartPattern = 0;
	looper.EndPattern = 0;

	while(TM_KEYPAD_Read() == TM_KEYPAD_Button_2)
		continue;
	show_status_line = TRUE;
	SHOW_STATUS_LINE();

	menuMultiLine(3,30,"[1] Skip loop backward","[2] Skip loop forward","[AB] Select channels");
	sprintf(lcdline, "Current loop: %u", (unsigned int)(looper.StartPattern + 1));
	menuMultiLine(1,130,lcdline);

	while(TRUE){
		Keypad_Button = TM_KEYPAD_Read();

		switch(Keypad_Button){
			case TM_KEYPAD_Button_0: goto end_select_loop;
			case TM_KEYPAD_Button_1:
				 looper.StartLooper = FALSE;
				 looper.Playback = FALSE;
				 looper.Recording = FALSE;
				 if(pattern_audio_map[looper.StartPattern].sample_position > 0){
					 looper.StartPattern--;
					 looper.EndPattern--;
					 //looper.SamplesWritten = pattern_audio_map[looper.EndPattern + 1].sample_position;
					 //looper.SamplesWritten = 0;
				 }

				 break;
			case TM_KEYPAD_Button_2:
				looper.StartLooper = FALSE;
				looper.Playback = FALSE;
				looper.Recording = FALSE;
				if(pattern_audio_map[looper.EndPattern + 1].sample_position > 0){
					 looper.StartPattern++;
					 looper.EndPattern++;
					 //looper.SamplesWritten = pattern_audio_map[looper.EndPattern + 1].sample_position;
					 //looper.SamplesWritten = 0;
				 }

				break;
			case TM_KEYPAD_Button_A:
			case TM_KEYPAD_Button_B:	select_channel(Keypad_Button);
										break;

			case TM_KEYPAD_Button_3:	pauseLoop();
										break;

		}

		if(Keypad_Button != TM_KEYPAD_Button_NOPRESSED){
			menuMultiLine(3,30,"[1] Skip loop backward","[2] Skip loop forward","[AB] Select channels");
			sprintf(lcdline, "Current loop: %u", (unsigned int)(looper.StartPattern + 1));
			menuMultiLine(1,130,lcdline);
			sprintf(lcdline, "Current loop: %u", (unsigned int)(looper.StartPattern + 1));
			menuMultiLine(1,130,lcdline);
			show_status_line = TRUE;
		}

		SHOW_STATUS_LINE();
	}


	end_select_loop:
	Skip_Read_Button = TRUE;
}


void select_channel(TM_KEYPAD_Button_t key){
	if(looper.Playback == TRUE || looper.Recording == TRUE)
		pauseLoop();
	switch(key){
		case TM_KEYPAD_Button_A:
			if(ACTIVE_CHANNEL_2){
				setActiveChannelOne();
				break;
			}
			if(ACTIVE_CHANNEL_1){
				 if(looper.TwoChannels == FALSE)
					 toggleActiveBothChannels(TRUE);
				 else
					 toggleActiveBothChannels(FALSE);
			}
			break;

		case TM_KEYPAD_Button_B:
			if(ACTIVE_CHANNEL_1){
				setActiveChannelTwo();
				break;
			}
			if(ACTIVE_CHANNEL_2){
				 if(looper.TwoChannels == FALSE)
					 toggleActiveBothChannels(TRUE);
				 else
					 toggleActiveBothChannels(FALSE);
			}
			break;
		}

}


void select_bars() {
	uint32_t numOfPatterns;
	uint32_t numOfBytes;
	uint32_t maxResolution;
	BOOL play = FALSE;
	//looper.startPattern = 0;
	sdram_pointer = 0;
	looper.Function = AUDIO_DRUMS;
	//map = (uint32_t (*)[])
	memset(pattern_audio_map,0,sizeof(pattern_audio_map));
	readDrums(&numOfPatterns,&numOfBytes,&maxResolution);
	// restore previous values of start/end bars
	getStartEndPatterns(&looper.StartPattern,&looper.EndPattern);
	//looper.endPattern = numOfPatterns - 1;

	if(numOfPatterns == 0){
		menuMultiLine(1,130,messages[NO_PATTS]);
		menuWaitReturn();
		goto end_play_rhythm;
	}
	if(numOfPatterns > MAX_PATTERNS){
		menuMultiLine(1,150,messages[TOO_MANY_PATTS]);
		menuWaitReturn();
		goto end_play_rhythm;
	}
	if(maxResolution > MAX_SUBBEATS){
		menuMultiLine(1,150,messages[TOO_MANY_SUBB]);
		menuWaitReturn();
		goto end_play_rhythm;
	}

	do{
		looper.DrumState = DRUMS_READY;
		// return star and end patterns and use them as parameters to drum loop
		// waits until play == TRUE (button "3" or joystick)
		menuShowStatus();
		drumMenuInput(numOfPatterns,&play);
		if(play == FALSE)
			goto end_play_rhythm;
		drumLoop();
		// end playing on pressing user button or joystick


	}while(play != FALSE);

	end_play_rhythm:
	//free(map);
	looper.DrumState = DRUMS_STOPPED;
	Skip_Read_Button = TRUE;
}

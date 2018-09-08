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

extern TM_KEYPAD_Button_t Keypad_Button;
extern BOOL Skip_Read_Button;


void audio_only(void){
	looper.Function = AUDIO_ONLY;
}

void drums_only(void){
	looper.Function = DRUMS_ONLY;
}

void audio_drums(){
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

void download_rhythm(void) {
	looper.Function = DOWNLOAD_SRAM;
	menuMultiLine(2,110,"Press [Send via USB] button","in Rhythm application.");
	SRAM_download_rhythm();
	looper.Function = NONE;
	Skip_Read_Button = TRUE;
}

void select_channel(void){
	TM_KEYPAD_Button_t key;
	if(ACTIVE_CHANNEL_1)
		sprintf(lcdline,"%s","Active channel: 1");
	else if(ACTIVE_CHANNEL_2)
		sprintf(lcdline,"%s","Active channel: 2");
	else if(ACTIVE_BOTH_CHANNELS)
		sprintf(lcdline,"%s","Active: Both     ");

	menuMultiLine(1,110,lcdline);
	menuMultiLine(3,130,"[A] Channel 1","[B] Channel 2","[C] Both");

	while(TRUE){
		key = TM_KEYPAD_Read();
		switch(key){
			case TM_KEYPAD_Button_0: return;
			case TM_KEYPAD_Button_A: looper.StartLooper = FALSE;
									 looper.ch1.Active = TRUE;
									 looper.ch2.Active = FALSE;
									 looper.ch1.Monitor = TRUE;
									 looper.ch2.Monitor = FALSE;
									 ADS1256_SetDiffChannel(0);
									 menuMultiLine(1,110,"Active channel: 1");
									 ADS1256_WriteCmd(CMD_SELFCAL);
									 ADS1256_WriteCmd(CMD_SELFOCAL);
									 break;


			case TM_KEYPAD_Button_B:looper.StartLooper = FALSE;
									looper.ch1.Active = FALSE;
									looper.ch2.Active = TRUE;
									looper.ch1.Monitor = FALSE;
									looper.ch2.Monitor = TRUE;
									ADS1256_SetDiffChannel(1);
									menuMultiLine(1,110,"Active channel: 2");
									ADS1256_WriteCmd(CMD_SELFCAL);
									ADS1256_WriteCmd(CMD_SELFOCAL);
									break;
			case TM_KEYPAD_Button_C:menuMultiLine(1,110,"Active: Both     ");
									break;

		}

	}

	Skip_Read_Button = TRUE;
}

void play_rhythm(void) {
	uint32_t map[MAX_PATTERNS][2];
	uint32_t numOfPatterns;
	uint32_t numOfBytes;
	uint32_t maxResolution;
	BOOL play = FALSE;
	looper.startPattern = 0;
	sdram_pointer = 0;
	//map = (uint32_t (*)[])
	memset(map,0,sizeof(map));
	readDrums(map,&numOfPatterns,&numOfBytes,&maxResolution);
	looper.endPattern = numOfPatterns - 1;

	if(numOfPatterns == 0){
		menuMultiLine(1,130,messages[NO_PATTS]);
		menuWaitReturn();
		goto end_play_rhythm;
	}
	if(numOfPatterns > MAX_PATTERNS){
		menuMultiLine(1,130,messages[TOO_MANY_PATTS]);
		menuWaitReturn();
		goto end_play_rhythm;
	}
	if(maxResolution > MAX_SUBBEATS){
		menuMultiLine(1,130,messages[TOO_MANY_SUBB]);
		menuWaitReturn();
		goto end_play_rhythm;
	}

	do{
		looper.DrumState = DRUMS_READY;
		// return star and end patterns and use them as parameters to drum loop
		// waits until play == TRUE (button "3" or joystick)
		menuShowStatus();
		drumMenuInput(map,numOfPatterns,&play);
		drumLoop(map);
		// end playing on pressing user button or joystick


	}while(play != FALSE);

	end_play_rhythm:
	//free(map);
	looper.DrumState = DRUMS_STOPPED;
	Keypad_Button = TM_KEYPAD_Button_0;
	Skip_Read_Button = TRUE;
}

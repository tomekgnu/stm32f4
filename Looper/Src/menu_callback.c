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

extern TM_KEYPAD_Button_t Keypad_Button;
extern BOOL Skip_Read_Button;



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
	menuMultiLine(2,100,"Press [Send via USB] button","in Rhythm application.");
	SRAM_download_rhythm();
	looper.Function = NONE;
	Skip_Read_Button = TRUE;
}

void play_rhythm(void) {
	uint32_t (*map)[2];
	uint32_t currPat = 0;
	uint32_t numOfPatterns;
	uint32_t numOfBytes;
	uint32_t maxResolution;
	uint8_t choice;
	looper.DrumState = DRUMS_READY;
	map = (uint32_t (*)[])readDrums(&numOfPatterns,&numOfBytes,&maxResolution);

	if(numOfPatterns == 0){
		menuMultiLine(1,100,messages[NO_PATTS]);
		menuWaitReturn();
		return;
	}
	if(maxResolution > MAX_SUBBEATS){
		menuMultiLine(1,100,messages[NO_PATTS]);
		menuWaitReturn();
		return;
	}

	do{
			// return pattern from which to play and use it as parameter to drum loop
			currPat = drumMenuInput(map,currPat,numOfPatterns,&choice);
			switch(choice){
				case TM_KEYPAD_Button_0: break;

				case TM_KEYPAD_Button_3: // return last played pattern and use it as parameter to menu
										 currPat = drumLoop(map,currPat,numOfPatterns);
								 	 	 break;
				default:				 break;
			}
		}
		while(choice != TM_KEYPAD_Button_0);

	free(map);
	looper.DrumState = DRUMS_STOPPED;
	Keypad_Button = TM_KEYPAD_Button_0;
	Skip_Read_Button = TRUE;
}

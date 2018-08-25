/*
 * menu_callback.c
 *
 *  Created on: 29.07.2018
 *      Author: Tomek
 */
#include "stdio.h"
#include "ff.h"
#include "main.h"
#include "memops.h"
#include "drums.h"
#include "tm_stm32f4_keypad.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32_hd44780.h"
#include "menu_callback.h"

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
	SRAM_download_rhythm();
	looper.Function = NONE;
	Skip_Read_Button = TRUE;
}

void play_rhythm(void) {
	FIL fil;
	looper.DrumState = DRUMS_READY;
	TM_ILI9341_Puts(10, 10,"[4] One bar back", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	TM_ILI9341_Puts(10, 30,"[5] Start", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	TM_ILI9341_Puts(10, 50,"[6] One bar forward", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	TM_ILI9341_Puts(10, 70,"[0] Return", &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	playDrums(&fil);
	looper.DrumState = DRUMS_STOPPED;
	Keypad_Button = TM_KEYPAD_Button_0;
	Skip_Read_Button = TRUE;
}

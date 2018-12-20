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
#include "tm_stm32f4_fatfs.h"

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


static void display_list(FileEntry *head,uint32_t startIndex,uint32_t size){
	FileEntry *current;
	uint32_t index = 0;
	TM_ILI9341_Fill(ILI9341_COLOR_MAGENTA);
	for(current = head; current != NULL && current->number < startIndex; current = current->next)
		continue;
	for(;current != NULL && current->number < (startIndex + size); current = current->next,index++)
		TM_ILI9341_Puts(10, 10 + (index * 10), current->filename, &TM_Font_7x10, ILI9341_COLOR_YELLOW, ILI9341_COLOR_MAGENTA);

}



void get_file(char *outstr){
	uint16_t idx = 0;
	TCHAR path[8];
	FILINFO fno;
	DIR dir;

	FileEntry *current, *head, *node, *tail;
	current = NULL;
	head = NULL;
	FRESULT res = f_getcwd(path,8);

	if(res == FR_OK){
		f_opendir(&dir,path);
		while((res = f_readdir(&dir, &fno)) == FR_OK && fno.fname[0] != 0){
			if(fno.fattrib & AM_DIR)	// directory
				continue;
			 node = (FileEntry *)malloc(sizeof(FileEntry));
			 strcpy(node->filename,fno.fname);
			 node->number = idx++;
			 node->next = node->prev = NULL;
			 if(head == NULL){
			    current = head = tail = node;
			 }
			 else{
			     current = current->next = node;
				 current->prev = tail;
				 tail = current;
			 }

		}

		f_closedir(&dir);

		display_list(head,0,10);
		display_list(head,10,10);
		display_list(head,20,10);
		display_list(head,30,10);

		// free linked list memory
		for(current = head; current ; current=current->next)
			free(current);

	}
	else
		TM_ILI9341_Puts(10, 10, "Error reading SD card", &TM_Font_7x10,	ILI9341_COLOR_YELLOW, ILI9341_COLOR_MAGENTA);

	return;
}

void get_string(char *outstr) {
	uint8_t nameIndex = 0;
	TM_KEYPAD_Button_t tmpKey = TM_KEYPAD_Button_NOPRESSED;
	uint8_t charIndex = 0;
	BOOL keyChar = FALSE;
	BOOL keyBack = FALSE;
	BOOL keyForw = FALSE;
	BOOL keyDel = FALSE;

	// array indexed by TM_KEYPAD_Button_t
	static char* keychars[10] = { "0", "1", "2ABCabc", "3DEFdef", "4GHIghi",
			"5JKLjkl", "6MNOmno", "7PRSprs", "8TUVtuv", "9WXYZwxyz" };
	memset(outstr, '\0', 26);
	TM_ILI9341_Fill(ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(10, 10, "Enter filename:", &TM_Font_7x10,
			ILI9341_COLOR_YELLOW, ILI9341_COLOR_MAGENTA);
	while (TM_KEYPAD_Read() == TM_KEYPAD_Button_4)
		continue;
	TM_ILI9341_Putc(10 + (nameIndex * 11), 30, ' ', &TM_Font_11x18,
			ILI9341_COLOR_BLACK, ILI9341_COLOR_YELLOW);
	while ((Keypad_Button = TM_KEYPAD_Read()) != TM_KEYPAD_Button_HASH) {
		switch (Keypad_Button) {
			case TM_KEYPAD_Button_0:
			case TM_KEYPAD_Button_1:
			case TM_KEYPAD_Button_2:
			case TM_KEYPAD_Button_3:
			case TM_KEYPAD_Button_4:
			case TM_KEYPAD_Button_5:
			case TM_KEYPAD_Button_6:
			case TM_KEYPAD_Button_7:
			case TM_KEYPAD_Button_8:
			case TM_KEYPAD_Button_9:
				keyChar = TRUE;
				if (Keypad_Button != tmpKey) {
					tmpKey = Keypad_Button;
					charIndex = 0;
				} else if (keychars[Keypad_Button][charIndex + 1] != '\0')
					charIndex++;
				else
					charIndex = 0;

				outstr[nameIndex] = keychars[Keypad_Button][charIndex];
				break;
			case TM_KEYPAD_Button_A:
				keyForw = TRUE;
				break;
			case TM_KEYPAD_Button_B:
				keyBack = TRUE;
				break;
			case TM_KEYPAD_Button_C:
				keyDel = TRUE;
				break;
		}
		if (Keypad_Button != TM_KEYPAD_Button_NOPRESSED) {
			TM_ILI9341_Puts(10, 30, outstr, &TM_Font_11x18,
					ILI9341_COLOR_BLACK, ILI9341_COLOR_CYAN);
			if (keyChar == TRUE) {
				keyChar = FALSE;
				TM_ILI9341_Putc(10 + (nameIndex * 11), 30, outstr[nameIndex],
						&TM_Font_11x18, ILI9341_COLOR_BLACK,
						ILI9341_COLOR_YELLOW);
			} else if (keyForw == TRUE) {
				keyForw = FALSE;
				if (nameIndex < 24 && outstr[nameIndex] != '\0')
					nameIndex++;
				else
					TM_ILI9341_Putc(10 + (nameIndex * 11), 30, ' ',
							&TM_Font_11x18, ILI9341_COLOR_BLACK,
							ILI9341_COLOR_YELLOW);
			} else if (keyBack == TRUE) {
				keyBack = FALSE;
				if (outstr[nameIndex] == '\0')
					TM_ILI9341_Putc(10 + (nameIndex * 11), 30, ' ',
							&TM_Font_11x18, ILI9341_COLOR_MAGENTA,
							ILI9341_COLOR_MAGENTA);

				if (nameIndex > 0)
					nameIndex--;

				TM_ILI9341_Putc(10 + (nameIndex * 11), 30, outstr[nameIndex],
						&TM_Font_11x18, ILI9341_COLOR_BLACK,
						ILI9341_COLOR_YELLOW);
			} else if (keyDel == TRUE) {
				keyDel = FALSE;
				strcpy(outstr + nameIndex, outstr + nameIndex + 1);
				TM_ILI9341_Puts(10, 30, outstr, &TM_Font_11x18,
						ILI9341_COLOR_BLACK, ILI9341_COLOR_CYAN);
				TM_ILI9341_Putc(10 + (strlen(outstr) * 11), 30, ' ',
						&TM_Font_11x18, ILI9341_COLOR_MAGENTA,
						ILI9341_COLOR_MAGENTA);
			}

			if (outstr[nameIndex] != '\0')
				TM_ILI9341_Putc(10 + (nameIndex * 11), 30, outstr[nameIndex],
						&TM_Font_11x18, ILI9341_COLOR_BLACK,
						ILI9341_COLOR_YELLOW);
			else
				TM_ILI9341_Putc(10 + (nameIndex * 11), 30, ' ', &TM_Font_11x18,
						ILI9341_COLOR_BLACK, ILI9341_COLOR_YELLOW);
		}
	}
}

void saveSingleLoop(uint32_t n){
	char filename[26];
	get_string(filename);
	FIL fil;
	FATFS FatFs;
	if (f_mount(&FatFs, "", 1) == FR_OK) {
		//Mounted OK, turn on RED LED
		BSP_LED_On(LED_RED);
		if (f_open(&fil, filename, FA_OPEN_ALWAYS | FA_WRITE) == FR_OK){
			SD_WriteAudio(pattern_audio_map[n].sample_position,pattern_audio_map[n + 1].sample_position,&fil);
			f_close(&fil);
			BSP_LED_Off(LED_GREEN);
			//Unmount drive, don't forget this!
			f_mount(0, "", 1);
			BSP_LED_Off(LED_RED);
		}
	 }

	return;
}

void readFromSD(uint32_t n){
	char filename[26];	// filename to open

	FATFS FatFs;
	if (f_mount(&FatFs, "", 1) == FR_OK) {
		//Mounted OK, turn on RED LED
		BSP_LED_On(LED_RED);
		get_file(filename);
//		if (f_open(&fil, filename, FA_OPEN_ALWAYS | FA_READ) == FR_OK){
//			pattern_audio_map[n + 1].sample_position = pattern_audio_map[n].sample_position + SD_ReadAudio(pattern_audio_map[n].sample_position,&fil);
//			f_close(&fil);
//			BSP_LED_Off(LED_GREEN);
//			//Unmount drive, don't forget this!
//
//		}

		f_mount(0, "", 1);
		BSP_LED_Off(LED_RED);
	 }

	return;

}

void saveAllLoops()
{

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

	while(TM_KEYPAD_Read() != TM_KEYPAD_Button_NOPRESSED)
		continue;
	show_status_line = TRUE;
	SHOW_STATUS_LINE();

	menuMultiLine(6,30,"[1] Skip loop backward","[2] Skip loop forward","[3] Pause/Resume loop","[4] Save current loop","[5] Read loop from SD","[AB] Select channels");
	sprintf(lcdline, "Current loop: %u", (unsigned int)(looper.StartPattern + 1));
	menuMultiLine(1,160,lcdline);

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
			case TM_KEYPAD_Button_4:	saveSingleLoop(looper.StartPattern);
										menuShowOptions();
										break;
			case TM_KEYPAD_Button_5:	readFromSD(looper.StartPattern);
										break;

		}

		if(Keypad_Button != TM_KEYPAD_Button_NOPRESSED){
			menuMultiLine(6,30,"[1] Skip loop backward","[2] Skip loop forward","[3] Pause/Resume loop","[4] Save current loop","[5] Read loop from SD","[AB] Select channels");
			sprintf(lcdline, "Current loop: %u", (unsigned int)(looper.StartPattern + 1));
			menuMultiLine(1,160,lcdline);
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

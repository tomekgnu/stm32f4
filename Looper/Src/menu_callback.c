/*
 * menu_callback.c
 *
 *  Created on: 29.07.2018
 *      Author: Tomek
 */
#include "stdio.h"
#include "tm_stm32f4_keypad.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32_hd44780.h"
#include "menu_callback.h"

extern TM_KEYPAD_Button_t Keypad_Button;
extern BOOL Skip_Read_Button;

static char lcdline[20];

void print_letters(void){

	int c = '0';
	sprintf(lcdline,"%c",c);
	TM_HD44780_Puts(0,1,lcdline);
	while ((Keypad_Button = TM_KEYPAD_Read()) != TM_KEYPAD_Button_0)
	{
		if(Keypad_Button == TM_KEYPAD_Button_5){
			c++;
			sprintf(lcdline,"%c",c);
			TM_HD44780_Puts(0,1,lcdline);
		}


	}

	Skip_Read_Button = TRUE;

}

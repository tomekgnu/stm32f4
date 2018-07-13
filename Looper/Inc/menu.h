#ifndef __MENU_H__
#define  __MENU_H__

#include "main.h"
#include "audio.h"
#include "stm32f429xx.h"
#include "tm_stm32f4_keypad.h"

enum {MAIN_MENU,NODE1,NODE2,NODE3,NODE4,NODE5,NODE6,NODE7};



typedef struct {
	char *title;
	uint8_t numOpts;
	uint8_t options[16];
	uint8_t back;
} menuScreen;

void menuShowTimers(__IO CHANNEL *ch1,__IO CHANNEL *ch2);
void menuShow(TM_KEYPAD_Button_t);
void menuInit();

#endif

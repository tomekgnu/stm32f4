#ifndef __MENU_H__
#define  __MENU_H__

#include "main.h"
#include "audio.h"
#include "stm32f429xx.h"
#include "tm_stm32f4_keypad.h"

#define TOTAL_MENU_NODES	8
#define MAX_NUM_OPTS		16

enum {NODE_EMPTY=-1,MAIN_MENU,NODE1,NODE2,NODE3,NODE4,NODE5,NODE6,NODE7};



typedef struct {
	char *title;
	int8_t options[MAX_NUM_OPTS];
	void (*callback)(void);
} menuNodeType;

void menuShowTimers(__IO CHANNEL *ch1,__IO CHANNEL *ch2);
void menuShow(TM_KEYPAD_Button_t);
void menuInit();

#endif

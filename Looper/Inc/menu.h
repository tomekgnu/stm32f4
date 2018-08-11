#ifndef __MENU_H__
#define  __MENU_H__

#include "main.h"
#include "audio.h"
#include "stm32f429xx.h"
#include "tm_stm32f4_keypad.h"

#define TOTAL_MENU_NODES	8
#define MAX_NUM_OPTS		16

typedef enum {NODE_EMPTY=-1,MAIN_MENU,NODE1,NODE2,NODE3,NODE4,NODE5,NODE6,NODE7} NODE_TYPE;



typedef struct {
	char *title;
	NODE_TYPE options[MAX_NUM_OPTS];
	void (*callback)(void);
} menuNodeType;

void menuShowTimers();
void menuShow(TM_KEYPAD_Button_t);
void menuInit();

#endif

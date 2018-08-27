#ifndef __MENU_H__
#define  __MENU_H__

#include "main.h"
#include "audio.h"
#include "stm32f429xx.h"
#include "tm_stm32f4_keypad.h"
#include "stdarg.h"

extern char *messages[];

#define TOTAL_MENU_NODES	8
#define MAX_NUM_OPTS		16

typedef enum {NODE_EMPTY=-1,MAIN_MENU,NODE1,NODE2,NODE3,NODE4,NODE5,NODE6,NODE7} NODE_TYPE;

enum {MAIN = 0, BACK,DOWNL_RTH,PLAY_RTH,ONE_BAR_BACK,ONE_BAR_FORW,START,NO_PATTS,TOO_MANY_SUBB};

typedef struct {
	char *title;
	NODE_TYPE options[MAX_NUM_OPTS];
	void (*callback)(void);
} menuNodeType;

void menuShowTimers();
void menuShow(TM_KEYPAD_Button_t);
void menuInit();
void menuWaitReturn();
void menuMultiLine(uint8_t lines,uint8_t offset,...);
void menuInitMsg();

uint32_t drumMenuInput(uint32_t (*map)[2],uint32_t currPat,uint32_t numOfPatterns,TM_KEYPAD_Button_t *key);

// menu macros
#define MESSG(X)			messages[X]




#endif

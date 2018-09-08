#ifndef __MENU_H__
#define  __MENU_H__

#include "main.h"
#include "audio.h"
#include "stm32f429xx.h"
#include "tm_stm32f4_keypad.h"
#include "stdarg.h"

extern char *messages[];

#define TOTAL_MENU_NODES	20
#define MAX_NUM_OPTS		16

typedef enum {
	NODE_EMPTY = -1,
	MAIN_MENU,
	AUDIO_NODE,
	RHYTHM_NODE,
	AUDIO_RHYTHM_NODE,
	SELECT_CHANNEL_NODE,
	NODE5,
	NODE6,
	DOWNLOAD_RHYTHM,
	PLAY_RHYTHM,
	MOVE_BAR_BACK_START,
	MOVE_BAR_FORW_START,
	START_RHYTHM_NODE,
	MOVE_BAR_BACK_END,
	MOVE_BAR_FORW_END,
	NODE14,
	NODE15,
	NODE16,
	NODE17,
	NODE18,
	NODE19
} NODE_TYPE;

enum {
	MAIN = 0,
	BACK,
	DOWNL_RTH,
	PLAY_RTH,
	ONE_BAR_BACK_START,
	ONE_BAR_FORW_START,
	ONE_BAR_BACK_END,
	ONE_BAR_FORW_END,
	START_RHYTHM,
	NO_PATTS,
	TOO_MANY_PATTS,
	TOO_MANY_SUBB,
	CHANNEL_SELECT,
	AUDIO_RHYTHM
};

typedef struct {
	char *title;
	NODE_TYPE options[MAX_NUM_OPTS];
	void (*callback)(void);
} menuNodeType;

void menuShowTimers();
void menuShow(TM_KEYPAD_Button_t);
void menuInit();
void menuWaitReturn();
void menuMultiLine(uint8_t lines, uint8_t offset, ...);
void menuInitMsg();
void menuStatusLine(char *text);
void menuShowStatus();
void drumMenuInput(uint32_t (*map)[2],uint32_t numOfPatterns, BOOL *play);

// menu macros
#define MESSG(X)			messages[X]

#endif

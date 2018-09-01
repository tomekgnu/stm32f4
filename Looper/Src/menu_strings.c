#include "menu.h"

char *messages[30];

void menuInitMsg(){

	messages[MAIN] = "Main looper menu";
	messages[BACK] = "Go back";
	messages[DOWNL_RTH] = "Download rhythm";
	messages[PLAY_RTH] = "Play rhythm";
	messages[ONE_BAR_BACK_START] = "Move start back";
	messages[ONE_BAR_FORW_START] = "Move start forward";
	messages[ONE_BAR_BACK_END] = "Move end back";
	messages[ONE_BAR_FORW_END] = "Move end forward";
	messages[START] = "Start";
	messages[NO_PATTS] = "No patterns found!";
	messages[TOO_MANY_SUBB] = "Too many subbeats!";
	messages[CHANNEL_SELECT] = "Select channel";
}


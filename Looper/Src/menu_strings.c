#include "menu.h"

char *messages[30];

void menuInitMsg(){

	messages[MAIN] = "Main menu";
	messages[BACK] = "Go back";
	messages[DOWNL_RTH] = "Download rhythm";
	messages[PLAY_RTH] = "Play rhythm";
	messages[ONE_BAR_BACK] = "One bar back";
	messages[ONE_BAR_FORW] = "One bar forward";
	messages[TOGGLE_START_END] = "Toggle start/end";
	messages[START] = "Start";
	messages[NO_PATTS] = "No patterns found!";
	messages[TOO_MANY_SUBB] = "Too many subbeats!";

}


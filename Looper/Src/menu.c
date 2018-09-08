#include "stdio.h"
#include "main.h"
#include "menu.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32_hd44780.h"
#include "string.h"
#include "menu_callback.h"
#include "stdarg.h"
#include "joystick.h"
#include "tm_stm32f4_keypad.h"

static menuNodeType menu_nodes[TOTAL_MENU_NODES];
static uint8_t current_node_index;	// current option
extern TM_KEYPAD_Button_t Keypad_Button;

static void menuShowOptions(){
	TM_KEYPAD_Button_t option_index;
	NODE_TYPE node_index;
	uint16_t offset = 10;
	char *tit;

	TM_ILI9341_Fill(ILI9341_COLOR_MAGENTA);

	for(option_index = TM_KEYPAD_Button_0; option_index < MAX_NUM_OPTS; option_index++){
		node_index = menu_nodes[current_node_index].options[option_index];
		if(node_index == NODE_EMPTY)
			continue;
		if(option_index == TM_KEYPAD_Button_0)
			tit = "Go back";
		else
			tit = menu_nodes[node_index].title;

		sprintf(lcdline,"[%c] %s",TM_KEYPAD_GetChar(option_index),tit);
		TM_ILI9341_Puts(10, offset, lcdline, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
		offset += 20;
	}

}
/**
 * which node,number of options,title,return node
 */
static void initParentNode(uint8_t node_index,char * tit,void (*fun)(void)){
	if(node_index >= TOTAL_MENU_NODES)
		return;
	menu_nodes[node_index].title = tit;
	menu_nodes[node_index].options[TM_KEYPAD_Button_0] = node_index;	// return to itself by default
	menu_nodes[node_index].callback = fun;
}

/**
 * parent node,which parent option,child node
 */
static void connectChildNode(uint8_t parent,uint8_t opt_key,uint8_t child){
	if(child >= TOTAL_MENU_NODES)
		return;
	if(opt_key == TM_KEYPAD_Button_0)	// option "0" will be connected to par_node later
		return;
	if(opt_key > TM_KEYPAD_Button_D)
		return;
	menu_nodes[child].options[TM_KEYPAD_Button_0] = parent;
	menu_nodes[parent].options[opt_key] = child;

}

void menuInit(){
	menuInitMsg();
	memset(menu_nodes,(int)NODE_EMPTY,sizeof(menu_nodes));
	current_node_index = MAIN_MENU;

	// top 3 nodes: AUDIO RHYTHM AUDIO+RHYTHM
	initParentNode(MAIN_MENU,messages[MAIN],NULL);
		initParentNode(AUDIO_NODE,"Audio",audio_only);
		connectChildNode(MAIN_MENU,TM_KEYPAD_Button_1,AUDIO_NODE);
			initParentNode(SELECT_CHANNEL_NODE,messages[CHANNEL_SELECT],select_channel);
			connectChildNode(AUDIO_NODE,TM_KEYPAD_Button_1,SELECT_CHANNEL_NODE);
		initParentNode(RHYTHM_NODE,"Rhythm",NULL);
		connectChildNode(MAIN_MENU,TM_KEYPAD_Button_2,RHYTHM_NODE);
			initParentNode(DOWNLOAD_RHYTHM,messages[DOWNL_RTH],download_rhythm);
			connectChildNode(RHYTHM_NODE,TM_KEYPAD_Button_1,DOWNLOAD_RHYTHM);
			initParentNode(PLAY_RHYTHM,messages[PLAY_RTH],play_rhythm);
			connectChildNode(RHYTHM_NODE,TM_KEYPAD_Button_2,PLAY_RHYTHM);
				initParentNode(MOVE_BAR_BACK_START,messages[ONE_BAR_BACK_START],NULL);
				connectChildNode(PLAY_RHYTHM,TM_KEYPAD_Button_1,MOVE_BAR_BACK_START);
				initParentNode(MOVE_BAR_FORW_START,messages[ONE_BAR_FORW_START],NULL);
				connectChildNode(PLAY_RHYTHM,TM_KEYPAD_Button_2,MOVE_BAR_FORW_START);
				initParentNode(START_RHYTHM_NODE,messages[START_RHYTHM],NULL);
				connectChildNode(PLAY_RHYTHM,TM_KEYPAD_Button_3,START_RHYTHM_NODE);
				initParentNode(MOVE_BAR_BACK_END,messages[ONE_BAR_BACK_END],NULL);
				connectChildNode(PLAY_RHYTHM,TM_KEYPAD_Button_4,MOVE_BAR_BACK_END);
				initParentNode(MOVE_BAR_FORW_END,messages[ONE_BAR_FORW_END],NULL);
				connectChildNode(PLAY_RHYTHM,TM_KEYPAD_Button_5,MOVE_BAR_FORW_END);
		initParentNode(AUDIO_RHYTHM_NODE,messages[AUDIO_RHYTHM],audio_drums);
		connectChildNode(MAIN_MENU,TM_KEYPAD_Button_3,AUDIO_RHYTHM_NODE);

}

void menuShow(TM_KEYPAD_Button_t opt_key){
	if(opt_key > TM_KEYPAD_Button_D)
		return;
	if(menu_nodes[current_node_index].options[opt_key] == NODE_EMPTY)
		return;

	current_node_index = menu_nodes[current_node_index].options[opt_key];
	sprintf(lcdline,"%s",menu_nodes[current_node_index].title);
	TM_HD44780_Clear();
	TM_ILI9341_Fill(ILI9341_COLOR_MAGENTA);
	TM_HD44780_Puts(0,0,lcdline);
	menuShowOptions();
	// execute function handler
	if(menu_nodes[current_node_index].callback != NULL)
		menu_nodes[current_node_index].callback();

	return;
}

void menuShowStatus(){
	if(looper.Recording == FALSE && looper.Playback == FALSE){
		menuStatusLine("All stopped");
		return;
	}
	if(looper.Recording == TRUE){
		menuStatusLine("Recording");
		return;
	}
	if(looper.ch1.Overdub == TRUE || looper.ch2.Overdub == TRUE){
		menuStatusLine("Overdubbing");
		return;
	}
	if(looper.Playback == TRUE){
		menuStatusLine("Playback");
		return;
	}
}


// write short message text at the bottom
void menuStatusLine(char *text){
	sprintf(lcdline,"%-29s",text);
	TM_ILI9341_Puts(0, 221,lcdline, &TM_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
}

void menuMultiLine(uint8_t lines,uint8_t offset,...){
	uint8_t i;
	char *line;
	va_list ap;

	va_start(ap, offset);
	for(i = 0; i < lines; i++,offset += 20) {
	     line = va_arg(ap, char *);
	     TM_ILI9341_Puts(10, offset,line, &TM_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_BLUE2);
	}
	va_end(ap);
}

void menuWaitReturn(){
	while(TM_KEYPAD_Read() != TM_KEYPAD_Button_0)
		continue;
}

void menuShowTimers(){
	uint16_t seconds;
	uint8_t cs;	// centiseconds

	if(looper.Recording == TRUE && looper.ch1.SamplesWritten % 150 == 0){
		seconds = looper.ch1.SamplesWritten / 15000;
		cs = ((float)((looper.ch1.SamplesWritten % 15000) / 15000.00)) * 100;
		sprintf(lcdline,"Rec:  %02u:%02u:%02u sec.",(unsigned int)(seconds / 60),(unsigned int)seconds,(unsigned int)cs);
		TM_ILI9341_Puts(10, 10, lcdline, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	}
	else if(looper.Playback == TRUE && looper.ch1.SamplesRead % 150 == 0){
		seconds = looper.ch1.SamplesRead / 15000;
		cs = ((float)((looper.ch1.SamplesRead % 15000) / 15000.00)) * 100;
		sprintf(lcdline,"Play: %02u:%02u:%02u sec.",(unsigned int)(seconds / 60),(unsigned int)seconds,(unsigned int)cs);
		TM_ILI9341_Puts(10, 10, lcdline, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	}

}
static inline void forwardBar(BOOL start,uint32_t numOfPatterns){
	if(start == TRUE){
		if(looper.startPattern < looper.endPattern)
			looper.startPattern++;
		}
		else if(looper.endPattern < (numOfPatterns - 1))
			looper.endPattern++;

}


static inline void backwardBar(BOOL start){
	if(start == TRUE){
		if(looper.startPattern > 0)
			looper.startPattern--;
		}
	else if(looper.endPattern > looper.startPattern)
		looper.endPattern--;
}

void drumMenuInput(uint32_t (*map)[2],uint32_t numOfPatterns,BOOL *play){
	BOOL input = TRUE;
	BOOL startBar = TRUE;

	while (TRUE){

			Keypad_Button = TM_KEYPAD_Read();
			if(Keypad_Button != TM_KEYPAD_Button_NOPRESSED){
				input = TRUE;
				switch(Keypad_Button){

					case TM_KEYPAD_Button_0:	*play = FALSE; return;
					case TM_KEYPAD_Button_1:	startBar = TRUE;
												backwardBar(startBar);
												break;
					case TM_KEYPAD_Button_2:	startBar = TRUE;
												forwardBar(startBar,numOfPatterns);
												break;
					case TM_KEYPAD_Button_3:	*play = TRUE;  return;
					case TM_KEYPAD_Button_4:	startBar = FALSE;
												backwardBar(startBar);
												break;
					case TM_KEYPAD_Button_5:	startBar = FALSE;
												forwardBar(startBar,numOfPatterns); break;
				}

			}
			else if(looper.Recording == TRUE || looper.Playback == TRUE){
				*play = TRUE;
				break;
			}
			else if(Active_Joystick() == TRUE){
				input = TRUE;
				JOYSTICK js = Read_Joystick();
				if(js.ypos > CENTER)
					forwardBar(startBar,numOfPatterns);
				else if(js.ypos < CENTER)
					backwardBar(startBar);

				if(js.but == TRUE){
					if((looper.DrumState == DRUMS_READY || looper.DrumState == DRUMS_STOPPED) && *play == FALSE){
						*play = TRUE;
						 break;
					}
					if(looper.DrumState == DRUMS_READY && *play == TRUE)
						*play = FALSE;

				}


			}

			if(input == TRUE){
				if(startBar == TRUE){
					sprintf(lcdline,"*Start: %u     ",(unsigned int)(looper.startPattern + 1));
					TM_ILI9341_Puts(10, 150, lcdline, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
					sprintf(lcdline," End:	%u     ",(unsigned int)(looper.endPattern + 1));
					TM_ILI9341_Puts(10, 170, lcdline, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
				}
				else{
					sprintf(lcdline," Start: %u     ",(unsigned int)(looper.startPattern + 1));
					TM_ILI9341_Puts(10, 150, lcdline, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
					sprintf(lcdline,"*End:	%u     ",(unsigned int)(looper.endPattern + 1));
					TM_ILI9341_Puts(10, 170, lcdline, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);

				}

				input = FALSE;
			}
		}


}

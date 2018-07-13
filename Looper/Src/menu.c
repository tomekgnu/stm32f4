#include "menu.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32_hd44780.h"

static char lcdline[30];

static menuScreen menu_screens[8];
static uint8_t option = 0;	// current option

/**
 * which node,number of options,title,return node
 */
static void initNode(uint8_t nod,uint8_t n_opts,char * tit,uint8_t bk){
	menu_screens[nod].numOpts = n_opts;
	menu_screens[nod].title = tit;
	menu_screens[nod].back = bk;
}

/**
 * parent node,which parent option,child node
 */
static void connectNodes(uint8_t par_node,uint8_t par_opt,uint8_t chd_node){
	menu_screens[par_node].options[par_opt] = chd_node;
}

void menuInit(){
	initNode(MAIN_MENU,3,"Main menu",MAIN_MENU);
	initNode(NODE1,2,"Option_0_1",MAIN_MENU);
	initNode(NODE2,1,"Option_0_2",MAIN_MENU);
	initNode(NODE3,1,"Option_0_3",MAIN_MENU);

	initNode(NODE4,0,"Option_1_1",NODE1);
	initNode(NODE5,0,"Option_1_2",NODE1);
	initNode(NODE6,0,"Option_2_1",NODE2);
	initNode(NODE7,0,"Option_3_1",NODE3);

	connectNodes(MAIN_MENU,1,NODE1);
	connectNodes(MAIN_MENU,2,NODE2);
	connectNodes(MAIN_MENU,3,NODE3);

	connectNodes(NODE1,1,NODE4);
	connectNodes(NODE1,2,NODE5);
	connectNodes(NODE2,1,NODE6);
	connectNodes(NODE3,1,NODE7);

}

void menuShow(TM_KEYPAD_Button_t key){

	if(key > menu_screens[option].numOpts)
		return;
	if(key == TM_KEYPAD_Button_0)
		option = menu_screens[option].back;
	else
		option = menu_screens[option].options[key];
	sprintf(lcdline,"%s",menu_screens[option].title);
	TM_HD44780_Puts(0,0,lcdline);
	return;
}

void menuShowTimers(__IO CHANNEL *ch1,__IO CHANNEL *ch2){
	uint16_t seconds;
	uint8_t cs;	// centiseconds

	if(Recording == TRUE && ch1->SamplesWritten % 150 == 0){
		seconds = ch1->SamplesWritten / 15000;
		cs = ((float)((ch1->SamplesWritten % 15000) / 15000.00)) * 100;
		sprintf(lcdline,"Rec:  %02u:%02u:%02u sec.",(unsigned int)(seconds / 60),(unsigned int)seconds,(unsigned int)cs);
		TM_ILI9341_Puts(10, 10, lcdline, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	}
	else if(Playback == TRUE && ch1->SamplesRead % 150 == 0){
		seconds = ch1->SamplesRead / 15000;
		cs = ((float)((ch1->SamplesRead % 15000) / 15000.00)) * 100;
		sprintf(lcdline,"Play: %02u:%02u:%02u sec.",(unsigned int)(seconds / 60),(unsigned int)seconds,(unsigned int)cs);
		TM_ILI9341_Puts(10, 10, lcdline, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE2);
	}

}


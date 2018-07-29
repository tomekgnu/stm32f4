#include "menu.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32_hd44780.h"
#include "string.h"

static char lcdline[30];

static menuNodeType menu_nodes[TOTAL_MENU_NODES];
static uint8_t current_node;	// current option

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
	memset(menu_nodes,(int)NODE_EMPTY,sizeof(menu_nodes));
	current_node = MAIN_MENU;

	initParentNode(MAIN_MENU,"Main menu");
	initParentNode(NODE1,"Option_0_1",NULL);
	initParentNode(NODE2,"Option_0_2",NULL);
	initParentNode(NODE3,"Option_0_3",NULL);

	initParentNode(NODE4,"Option_1_1",NULL);
	initParentNode(NODE5,"Option_1_2",NULL);
	initParentNode(NODE6,"Option_2_1",NULL);
	initParentNode(NODE7,"Option_3_1",NULL);

	connectChildNode(MAIN_MENU,TM_KEYPAD_Button_1,NODE1);
	connectChildNode(MAIN_MENU,TM_KEYPAD_Button_2,NODE2);
	connectChildNode(MAIN_MENU,TM_KEYPAD_Button_7,NODE3);

	connectChildNode(NODE1,TM_KEYPAD_Button_1,NODE4);
	connectChildNode(NODE1,TM_KEYPAD_Button_2,NODE5);
	connectChildNode(NODE2,TM_KEYPAD_Button_1,NODE6);
	connectChildNode(NODE3,TM_KEYPAD_Button_1,NODE7);

}

void menuShow(TM_KEYPAD_Button_t opt_key){
	if(opt_key > TM_KEYPAD_Button_D)
		return;
	if(menu_nodes[current_node].options[opt_key] == NODE_EMPTY)
		return;

	current_node = menu_nodes[current_node].options[opt_key];
	sprintf(lcdline,"%s",menu_nodes[current_node].title);
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


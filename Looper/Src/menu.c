#include "menu.h"
#include "tm_stm32f4_ili9341.h"
#include "drums.h"
#include "midi.h"

/*
 * Acoustic_Bass_Drum,
		Side_Stick,
		Acoustic_Snare,
		Cowbell,
		Low_Floor_Tom,
		High_Floor_Tom,
		Low_Mid_Tom,
		Hi_Mid_Tom,
		High_Tom,
		Closed_Hi_Hat,
		Open_Hi_Hat,
		Pedal_Hi_Hat,
		Crash_Cymbal_1,
		Ride_Cymbal_2,
		Splash_Cymbal,
		Chinese_Cymbal
 */

static uint16_t ypix_level = 204;		// y coord. for drum parts
static uint16_t xpix_beat = 150;		// x coord. for beats
static uint8_t bar_beat = 0;			// 1 beat every 10 pixels
static uint16_t beat_time = 0;

void menuDrumEdit(){
	TM_ILI9341_DrawLine(150,24,150,204,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(160,24,160,204,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(170,24,170,204,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(180,24,180,204,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(190,24,190,204,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(200,24,200,204,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(210,24,210,204,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(220,24,220,204,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(230,24,230,204,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(240,24,240,204,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(250,24,250,204,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(260,24,260,204,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(270,24,270,204,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(280,24,280,204,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(290,24,290,204,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(300,24,300,204,ILI9341_COLOR_GRAY);


	TM_ILI9341_DrawLine(150,24,300,24,ILI9341_COLOR_BLUE2);
	TM_ILI9341_DrawLine(150,84,300,84,ILI9341_COLOR_BLUE2);
	TM_ILI9341_DrawLine(150,144,300,144,ILI9341_COLOR_BLUE2);
	TM_ILI9341_DrawLine(150,204,300,204,ILI9341_COLOR_BLUE2);


	TM_ILI9341_Puts(80, 2, "Drum Edit Menu", &TM_Font_7x10, ILI9341_COLOR_YELLOW, ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(2, 20, "1 Bass drum", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(2, 32, "2 Side stick", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(2, 44, "3 Snare", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(2, 56, "4 Cowbell", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(2, 68, "5 L.floor tom", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(2, 80, "6 H.floor tom", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(2, 92, "7 L.mid tom", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(2, 104, "8 H.mid tom", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(2, 116, "9 H.tom", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(2, 128, "10 Closed hi-hat", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(2, 140, "11 Open hi-hat", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(2, 152, "12 Pedal hi-hat", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(2, 164, "13 Crash cymb.", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(2, 176, "14 Ride cymb.", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(2, 188, "15 Splash cymb.", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(2, 200, "16 Chn.cymb", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(2, 218, "[A]=Right hand [B]=Left hand", &TM_Font_7x10, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
	TM_ILI9341_Puts(2, 230, "[C]=Right foot [D]=Left foot", &TM_Font_7x10, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);

}

static void drawActiveBeat(){
	// delete triangle on the left
	TM_ILI9341_DrawLine(xpix_beat - 13,18,xpix_beat + 13,18,ILI9341_COLOR_MAGENTA);
	TM_ILI9341_DrawLine(xpix_beat - 12,19,xpix_beat + 12,19,ILI9341_COLOR_MAGENTA);
	TM_ILI9341_DrawLine(xpix_beat - 11,20,xpix_beat + 11,20,ILI9341_COLOR_MAGENTA);
	TM_ILI9341_DrawPixel(xpix_beat - 10,21,ILI9341_COLOR_MAGENTA);
	// draw current triangle
	TM_ILI9341_DrawLine(xpix_beat - 3,18,xpix_beat + 3,18,ILI9341_COLOR_RED);
	TM_ILI9341_DrawLine(xpix_beat - 2,19,xpix_beat + 2,19,ILI9341_COLOR_RED);
	TM_ILI9341_DrawLine(xpix_beat - 1,20,xpix_beat + 1,20,ILI9341_COLOR_RED);
	TM_ILI9341_DrawPixel(xpix_beat,21,ILI9341_COLOR_RED);
	// delete triangle on the right
	TM_ILI9341_DrawLine(xpix_beat + 7,18,xpix_beat + 13,18,ILI9341_COLOR_MAGENTA);
	TM_ILI9341_DrawLine(xpix_beat + 8,19,xpix_beat + 12,19,ILI9341_COLOR_MAGENTA);
	TM_ILI9341_DrawLine(xpix_beat + 9,20,xpix_beat + 11,20,ILI9341_COLOR_MAGENTA);
	TM_ILI9341_DrawPixel(xpix_beat + 10,21,ILI9341_COLOR_MAGENTA);
}

void setDrumPart(uint8_t lev){
	switch(lev){
	case L_FOOT: ypix_level = 204;
				break;
	case R_FOOT: ypix_level = 144;
				break;
	case L_HAND: ypix_level = 84;
				break;
	case R_HAND: ypix_level = 24;
				break;

	}

}

void moveBeatForward(){
	if(beat_time < 3750)
		beat_time += 250;
	if(bar_beat < 15)
		bar_beat++;
	if(xpix_beat < 300)
		xpix_beat += 10;
	drawActiveBeat();
}

void moveBeatBack(){
	if(beat_time > 0)
		beat_time -= 250;
	if(bar_beat > 0)
		bar_beat--;
	if(xpix_beat > 150)
		xpix_beat -= 10;
	drawActiveBeat();
}

void placeDrumSymbol(uint8_t val){
	uint32_t color;
	drawActiveBeat();
	TM_ILI9341_DrawFilledCircle(xpix_beat,ypix_level,2,ILI9341_COLOR_GREEN);
	//drumTracks[R_FOOT][TIME][bar_beat] = beat_time;
	drumTracks[R_FOOT][DRUM][bar_beat] = Acoustic_Bass_Drum;
}

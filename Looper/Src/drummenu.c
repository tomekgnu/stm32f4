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

static uint8_t drum_part = L_FOOT;
static uint16_t ypix_level = 204;		// y coord. for drum parts
static uint16_t xpix_beat = 120;		// x coord. for beats
static uint8_t bar_beat = 0;			// 1 beat every 10 pixels
static uint16_t beat_time = 0;
static char lcdline[30];

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


/**
 * Read from file
 */
void menuDrumRead(){
	uint32_t part,beat;
	resetBeat();
	for(part = 0; part < 4; part++){
		for(beat = 0; beat < 16; beat++){
			//if(drumTracks1[part][beat] != 0)
				//placeDrumFromFile(drumTracks1[part][beat]);
			moveBeatForward();
		}
	resetBeat();
	}
}

void menuDrumEdit(){
	TM_ILI9341_DrawLine(120,24,120,204,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(130,24,130,204,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(140,24,140,204,ILI9341_COLOR_GRAY);
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
	TM_ILI9341_DrawLine(310,24,310,204,ILI9341_COLOR_GRAY);

	// 4 horizontal lines representing parts of body
	TM_ILI9341_DrawLine(120,24,310,24,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(120,84,310,84,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(120,144,310,144,ILI9341_COLOR_GRAY);
	TM_ILI9341_DrawLine(120,204,310,204,ILI9341_COLOR_GRAY);


	TM_ILI9341_Puts(100, 2, "Drum Edit Menu", &TM_Font_7x10, ILI9341_COLOR_YELLOW, ILI9341_COLOR_MAGENTA);
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
	//TM_ILI9341_Puts(2, 218, "[A]=Right hand [B]=Left hand", &TM_Font_7x10, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
	//TM_ILI9341_Puts(2, 230, "[C]=Right foot [D]=Left foot", &TM_Font_7x10, ILI9341_COLOR_RED, ILI9341_COLOR_MAGENTA);
	//menuDrumRead();
}

void drawActiveBeat(){
	// draw red point at the bottom
	TM_ILI9341_DrawFilledCircle(xpix_beat,220,3,ILI9341_COLOR_RED);
	// delete point on the left
	TM_ILI9341_DrawFilledCircle(xpix_beat - 10,220,3,ILI9341_COLOR_MAGENTA);
	// delete point on the right
	TM_ILI9341_DrawFilledCircle(xpix_beat + 10,220,3,ILI9341_COLOR_MAGENTA);
}

void setDrumPart(uint8_t lev){
	drum_part = lev;
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

void resetBeat(){
	xpix_beat = 120;
}

void moveBeatForward(){
	if(beat_time < 3750)
		beat_time += 250;
	if(bar_beat < 15)
		bar_beat++;
	if(xpix_beat < 300)
		xpix_beat += 10;
	else
		xpix_beat = 120;
	drawActiveBeat();
}

void moveBeatBack(){
	if(beat_time > 0)
		beat_time -= 250;
	if(bar_beat > 0)
		bar_beat--;
	if(xpix_beat > 120)
		xpix_beat -= 10;
	drawActiveBeat();
}

void clearDrumSymbol(){
	//drumTracks1[drum_part][bar_beat] = 0;
	TM_ILI9341_DrawFilledCircle(xpix_beat,ypix_level,3,ILI9341_COLOR_MAGENTA);
	if(xpix_beat == 120)
		TM_ILI9341_DrawLine(xpix_beat,ypix_level,xpix_beat + 4,ypix_level,ILI9341_COLOR_GRAY);
	else if(xpix_beat == 310)
		TM_ILI9341_DrawLine(xpix_beat - 4,ypix_level,xpix_beat,ypix_level,ILI9341_COLOR_GRAY);
	else
		TM_ILI9341_DrawLine(xpix_beat - 4,ypix_level,xpix_beat + 4,ypix_level,ILI9341_COLOR_GRAY);
	if(ypix_level == 24)
		TM_ILI9341_DrawLine(xpix_beat,ypix_level,xpix_beat,ypix_level + 4,ILI9341_COLOR_GRAY);
	else if(ypix_level == 204)
		TM_ILI9341_DrawLine(xpix_beat,ypix_level,xpix_beat,ypix_level - 4,ILI9341_COLOR_GRAY);
	else
		TM_ILI9341_DrawLine(xpix_beat,ypix_level + 4,xpix_beat,ypix_level - 4,ILI9341_COLOR_GRAY);
}

void placeDrumFromFile(uint8_t val){
	switch(val){
			case Acoustic_Bass_Drum: setDrumPart(R_FOOT);
									 break;
			case Side_Stick:
			case Acoustic_Snare:
			case Cowbell:
			case Low_Floor_Tom:
			case High_Floor_Tom:	setDrumPart(R_HAND);
									break;

			case Hi_Mid_Tom:
			case High_Tom:
			case Closed_Hi_Hat:
			case Open_Hi_Hat:
			case Crash_Cymbal_1:
			case Ride_Cymbal_2:
			case Splash_Cymbal:
			case Chinese_Cymbal:
									setDrumPart(L_HAND);
									break;

			case Pedal_Hi_Hat:		setDrumPart(L_FOOT);
									break;

		}
	TM_ILI9341_DrawFilledCircle(xpix_beat,ypix_level,3,ILI9341_COLOR_GREEN);
}

void placeDrumFromKeyboard(uint8_t val){
	uint32_t color;
	switch(key_to_drum[val - 1]){
		case Acoustic_Bass_Drum: setDrumPart(R_FOOT);
								 break;
		case Side_Stick:
		case Acoustic_Snare:
		case Cowbell:
		case Low_Floor_Tom:
		case High_Floor_Tom:	setDrumPart(R_HAND);
								break;

		case Hi_Mid_Tom:
		case High_Tom:
		case Closed_Hi_Hat:
		case Open_Hi_Hat:
		case Crash_Cymbal_1:
		case Ride_Cymbal_2:
		case Splash_Cymbal:
		case Chinese_Cymbal:
								setDrumPart(L_HAND);
								break;

		case Pedal_Hi_Hat:		setDrumPart(L_FOOT);
								break;

	}
	drawActiveBeat();
	TM_ILI9341_DrawFilledCircle(xpix_beat,ypix_level,3,ILI9341_COLOR_GREEN);
	//drumTracks[R_FOOT][bar_beat] = beat_time;
	//drumTracks1[drum_part][bar_beat] = key_to_drum[val - 1];
}

#include "menu.h"
#include "tm_stm32f4_ili9341.h"

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


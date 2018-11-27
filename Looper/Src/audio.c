#include "main.h"
#include "audio.h"
#include "stm32f429i_discovery_sdram.h"
#include "ads1256_test.h"
#include "string.h"
#include "limits.h"
#include "waveplayer.h"
#include "math.h"
#include "tm_stm32_hd44780.h"
#include "stdlib.h"
#include "math.h"
#include "midi.h"
#include "drums.h"
#include "dac.h"

uint32_t sdram_pointer = 0;
int16_t sample16s;	// sample obtained from ADS1256

static uint32_t startPatternTmp = 0;
static uint32_t endPatternTmp = 0;
static uint32_t sdramPointerTmp = 0;

void getStartEndPatterns(uint32_t *start,uint32_t *end){
	*start = startPatternTmp;
	*end = endPatternTmp;
}

void setStartEndPatterns(uint32_t start,uint32_t end){
	startPatternTmp = start;
	endPatternTmp = end;
	sdram_pointer =  sdramPointerTmp = pattern_audio_map[startPatternTmp].sample_position * looper.SampleBytes;
	(GET_ACTIVE_CHANNEL)->SamplesRead = pattern_audio_map[startPatternTmp].sample_position;
	(GET_INACTIVE_CHANNEL)->SamplesRead = pattern_audio_map[startPatternTmp].sample_position;
	if(looper.Function == AUDIO_DRUMS){
		(GET_ACTIVE_CHANNEL)->SamplesWritten = pattern_audio_map[endPatternTmp + 1].sample_position;
		(GET_INACTIVE_CHANNEL)->SamplesWritten = pattern_audio_map[endPatternTmp + 1].sample_position;
	}
}

void inline resetSamples(){
	if(looper.Playback == TRUE){
		//looper.SampleOffset = 0;
	}
	if(looper.Recording == TRUE){
		//looper.SampleOffset = 0;
	}
}

void signed16_unsigned12(int16_t *buf,int32_t start,int32_t stop){
	for(;start < stop;start++)
		buf[start] = SIGNED16_UNSIGNED12(buf[start]);
}

void record_sample(int16_t swrite,__IO CHANNEL *cha){
	if(looper.StartLooper == FALSE){
		return;
	}

	if(sdram_pointer < SDRAM_SIZE){
		BSP_SDRAM_WriteData16b(SDRAM_DEVICE_ADDR + sdram_pointer + cha->Offset,(uint16_t *) &swrite, 1);
		sdram_pointer += looper.SampleBytes;
		if(looper.Function == AUDIO_ONLY)
			(GET_ACTIVE_CHANNEL)->SamplesWritten++;
	}

}

void record_samples(int16_t swrite,__IO CHANNEL *cha,__IO CHANNEL *chb){
	int16_t sread;

	if(looper.StartLooper == FALSE ){
		return;
	}

	if(sdram_pointer < SDRAM_SIZE){
		if(cha->Active == TRUE){
			BSP_SDRAM_WriteData16b(SDRAM_DEVICE_ADDR + sdram_pointer + cha->Offset,(uint16_t *) &swrite, 1);

			// read other track if it has any samples
			if(pattern_audio_map[looper.StartPattern].channel_recorded[INACTIVE_CHANNEL_INDEX] == TRUE){
				BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer + chb->Offset,(uint16_t *)&sread,1);
				chb->CurrentSample = sread;
				chb->SamplesRead++;
			}

		}
		else if(chb->Active == TRUE){
			BSP_SDRAM_WriteData16b(SDRAM_DEVICE_ADDR + sdram_pointer  + chb->Offset,(uint16_t *) &swrite, 1);

			// read other track if it has any samples
			if(pattern_audio_map[looper.StartPattern].channel_recorded[INACTIVE_CHANNEL_INDEX] == TRUE){
				BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer + cha->Offset,(uint16_t *)&sread,1);
				cha->CurrentSample = sread;
				cha->SamplesRead++;
			}
		}

		sdram_pointer += looper.SampleBytes;
		if(looper.Function == AUDIO_ONLY)
			(GET_ACTIVE_CHANNEL)->SamplesWritten++;
	}

	if((GET_INACTIVE_CHANNEL)->SamplesWritten > 0 && (GET_INACTIVE_CHANNEL)->SamplesRead >= (GET_INACTIVE_CHANNEL)->SamplesWritten){
			BSP_LED_Off(LED_RED);
			show_status_line = TRUE;
			looper.StartLooper = FALSE;
			return;
	}
	if(sdram_pointer == SDRAM_SIZE){
		sdram_pointer = 0;
		looper.StartLooper = FALSE;
	}
}

void read_sample(int16_t swrite,__IO CHANNEL *cha){
	int16_t sread;

	if(looper.StartLooper == FALSE ){
		return;
	}

	BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer + cha->Offset,(uint16_t *) &sread, 1);

	cha->mix32tmp = sread  + swrite;
	cha->CurrentSample = sread;

	if(cha->Clipping == TRUE)
		cha->mix32tmp = (int32_t)(cha->gain * (float)sread  + cha->gain * (float)swrite);

	if(cha->mix32tmp > cha->mix32Max)
		cha->mix32Max = cha->mix32tmp;

	if(cha->Overdub == TRUE)
		BSP_SDRAM_WriteData16b(SDRAM_DEVICE_ADDR + sdram_pointer + cha->Offset,(uint16_t *) &cha->mix32tmp, 1);

	(GET_ACTIVE_CHANNEL)->SamplesRead++;
	sdram_pointer += looper.SampleBytes;

	if((GET_ACTIVE_CHANNEL)->SamplesRead >= (GET_ACTIVE_CHANNEL)->SamplesWritten){
		if(cha->Overdub == TRUE && cha->mix32Max > 16383){
			cha->Clipping = TRUE;
			cha->gain = 16383.00 / cha->mix32Max;
		}
		else{
			cha->Clipping = FALSE;
			cha->gain = 1.0;
		}

		cha->mix32Max = 16383;
		if(looper.Function == AUDIO_DRUMS)
			looper.StartLooper = FALSE;

		setStartEndPatterns(startPatternTmp,endPatternTmp);
		return;
	}



	if(sdram_pointer == SDRAM_SIZE)
		sdram_pointer = 0;
}

void read_samples(int16_t swrite,__IO CHANNEL *cha,__IO CHANNEL *chb){

	int16_t sread[2];

	if(looper.StartLooper == FALSE ){
		return;
	}

	BSP_SDRAM_ReadData(SDRAM_DEVICE_ADDR + sdram_pointer,(uint32_t *) sread, 1);
	cha->CurrentSample = sread[0];
	chb->CurrentSample = sread[1];

	(GET_ACTIVE_CHANNEL)->SamplesRead++;
	sdram_pointer += looper.SampleBytes;

	if((GET_ACTIVE_CHANNEL)->SamplesRead >= (GET_ACTIVE_CHANNEL)->SamplesWritten){
		if(looper.Function == AUDIO_DRUMS)
			looper.StartLooper = FALSE;

		setStartEndPatterns(startPatternTmp,endPatternTmp);

	}
	if(sdram_pointer == SDRAM_SIZE)
		sdram_pointer = 0;

}

void play_sample(__IO CHANNEL *cha){
	//if(cha->SampleCount > 0 && cha->SamplesRead <= cha->SampleCount)
	Write_DAC8552(channel_A,(uint16_t)(cha->CurrentSample + 16383));

}

void play_samples(__IO CHANNEL *cha,__IO CHANNEL *chb){

	//if(looper.TwoChannels && cha->SampleCount > 0 && chb->SampleCount > 0)
	if(looper.TwoChannels && pattern_audio_map[looper.StartPattern].channel_recorded[_CH1] && pattern_audio_map[looper.StartPattern].channel_recorded[_CH2])
		Write_DAC8552_Both((uint16_t)(cha->CurrentSample + 16383),(uint16_t)(chb->CurrentSample + 16383));
	else if(cha->Active == TRUE && pattern_audio_map[looper.StartPattern].channel_recorded[_CH1])
		Write_DAC8552(channel_A,(uint16_t)(cha->CurrentSample + 16383));
	else if(chb->Active == TRUE && pattern_audio_map[looper.StartPattern].channel_recorded[_CH2])
		Write_DAC8552(channel_B,(uint16_t)(chb->CurrentSample + 16383));

}


void play_sample_dac(__IO CHANNEL *cha){
	HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,(cha->CurrentSample / 4) + 2048);
}






void initChannels(){
	looper.ch1.Active = looper.ch2.Active = FALSE;
	looper.ch1.Monitor = looper.ch2.Monitor = FALSE;
	looper.ch1.mix32Max = looper.ch2.mix32Max = FALSE;
	looper.ch1.gain = looper.ch2.gain = 1.0;
	looper.ch1.CurrentSample = looper.ch2.CurrentSample = 0;
}

void setActiveChannelOne(){
	looper.StartLooper = FALSE;
	looper.ch1.Active = TRUE;
	looper.ch2.Active = FALSE;
	looper.ch1.Offset = 0;
	ADS1256_SetDiffChannel(0);
	ADS1256_WriteCmd(CMD_SELFCAL);
	ADS1256_WriteCmd(CMD_SELFOCAL);
}

void setActiveChannelTwo(){
	looper.StartLooper = FALSE;
	looper.ch2.Active = TRUE;
	looper.ch1.Active = FALSE;
	if(looper.TwoChannels == TRUE)
		looper.ch2.Offset = 2;
	else
		looper.ch2.Offset = 0;
	ADS1256_SetDiffChannel(1);
	ADS1256_WriteCmd(CMD_SELFCAL);
	ADS1256_WriteCmd(CMD_SELFOCAL);
}

void toggleActiveBothChannels(){
	looper.TwoChannels = !looper.TwoChannels;
	if(looper.TwoChannels == TRUE)
		looper.SampleBytes = 4;
	else
		looper.SampleBytes = 2;
}


void resetChannel(__IO CHANNEL *ch){
	ch->SamplesWritten = 0;
	ch->SamplesRead = 0;
	ch->Clipping = FALSE;
	ch->Overdub = FALSE;
	ch->mix32Max = 16383;
	ch->gain = 1.0;
	ch->CurrentSample = 0;
}

void showMinMaxSamples(int32_t max,int32_t min){
	char minstr[10],maxstr[10];
	itoa(max,maxstr,10);
	itoa(min,minstr,10);

	TM_HD44780_Clear();
	TM_HD44780_Puts(0,0,maxstr);
	TM_HD44780_Puts(0,1,minstr);
}

void stopAll(){
	looper.Recording = FALSE;
	looper.Playback = FALSE;
	looper.StartLooper = FALSE;
	looper.DrumState = DRUMS_STOPPED;
	//looper.Function = NONE;
	resetChannel(&looper.ch1);
	resetChannel(&looper.ch2);
	pattern_audio_map[looper.StartPattern].channel_recorded[_CH1] = FALSE;
	pattern_audio_map[looper.StartPattern].channel_recorded[_CH2] = FALSE;
	stopDrums();
	BSP_LED_Off(LED_RED);
	BSP_LED_Off(LED_GREEN);
	show_status_line = TRUE;
}

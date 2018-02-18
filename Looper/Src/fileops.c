#include "audio.h"
#include "fileops.h"
#include "main.h"
#include "stm32f429i_discovery_sdram.h"

static __IO uint8_t buff_switch;
static __IO BOOL data_ready = FALSE;
static uint16_t buffer_index = 0;
uint8_t file_buf[2][BUFF_SIZE];
static uint16_t * buffer_pointer = (uint16_t *)file_buf[0];
static UINT bytes;

extern uint32_t sdram_pointer;
extern FUNCTION function;

int16_t readSampleSD(){
	return buffer_pointer[buffer_index++];

//	if(buffer_index == (BUFF_SIZE / 2)){
//		buffer_index = 0;
//		data_ready = TRUE;
//	}

	//return sample;
}

void readSingleTrackSD(FIL *fp){
	buff_switch = 1;
	data_ready = FALSE;
	buffer_index = 0;
	f_read(fp,file_buf[0],BUFF_SIZE,&bytes);
	f_read(fp,file_buf[1],BUFF_SIZE,&bytes);

	function = PLAY_SD;
	while(1){
		while(buffer_index < (BUFF_SIZE / 2))
			continue;
		buffer_index = 0;
		buffer_pointer = (uint16_t *)file_buf[buff_switch];

		if(buff_switch == 1){
			f_read(fp,file_buf[0],BUFF_SIZE,&bytes);
			buff_switch = 0;
		}
		else{
			f_read(fp,file_buf[1],BUFF_SIZE,&bytes);
			buff_switch = 1;
		}

		if(f_eof(fp)){
			f_lseek(fp,SEEK_SET);
			f_read(fp,file_buf[0],BUFF_SIZE,&bytes);
			f_read(fp,file_buf[1],BUFF_SIZE,&bytes);
			buffer_pointer = (uint16_t *)file_buf[0];
			buff_switch = 1;
			buffer_index = 0;

		}
	}

}

void writeSingleTrackSD(__IO CHANNEL *ch,FIL *fp){
	uint32_t samples = ch->SamplesWritten;
	uint8_t remainder = samples % 256;
	sdram_pointer = 0;
	while(samples > 0){
		BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *) file_buf[0], 256);
		f_write(fp,file_buf[0],512,&bytes);
		if(samples > remainder)
			samples -= 256;
		else
			samples -= remainder;

		sdram_pointer += 512;
	}


}

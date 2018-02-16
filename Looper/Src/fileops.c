#include "audio.h"
#include "fileops.h"
#include "main.h"
#include "stm32f429i_discovery_sdram.h"

static __IO BOOL switch_buff;
static __IO BOOL data_ready = FALSE;
static uint16_t buffer_pointer = 0;
static UINT bytes;
uint8_t file_buf[512];
extern uint32_t sdram_pointer;
extern FUNCTION function;

int16_t readSampleSD(){
	int16_t sample;

	if(buffer_pointer == 256 || buffer_pointer == 512)
		data_ready = TRUE;
	if(buffer_pointer == 512)
		buffer_pointer = 0;
	sample = (uint16_t)file_buf[buffer_pointer];
	buffer_pointer += 2;
	return sample;
}

void readSingleTrackSD(FIL *fp){
	switch_buff = TRUE;
	buffer_pointer = 0;
	f_read(fp,file_buf,512,&bytes);
	function = PLAY_SD;
	while(1){
		while(data_ready == FALSE)
			continue;
		data_ready = FALSE;
		if(buffer_pointer == 256)
			f_read(fp,file_buf,256,&bytes);
		if(buffer_pointer == 0)
			f_read(fp,file_buf + 256,256,&bytes);
		if(f_eof(fp))
			f_lseek(fp,SEEK_SET);
	}

}

void writeSingleTrackSD(__IO CHANNEL *ch,FIL *fp){
	uint32_t samples = ch->SamplesWritten;
	uint8_t remainder = samples % 256;
	sdram_pointer = 0;
	while(samples > 0){
		BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *) file_buf, 256);
		f_write(fp,file_buf,512,&bytes);
		if(samples > remainder)
			samples -= 256;
		else
			samples -= remainder;

		sdram_pointer += 512;
	}


}

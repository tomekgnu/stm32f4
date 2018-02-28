#include "audio.h"
#include "fileops.h"
#include "main.h"
#include "stm32f429i_discovery_sdram.h"
#include "tm_stm32_hd44780.h"
#include "spiffs.h"
#include "spiffs_nucleus.h"
#include "fatfs_sd.h"

static __IO uint8_t play_buffer = 0;					//Keeps track of which buffer is currently being used
static __IO BOOL new_buffer_ready = FALSE;			//Flag used by 'Loop' code to tell the Interrupt that new data is ready in the buffer.
static __IO BOOL need_new_data = FALSE;
static __IO uint32_t word_count = 0;
static int16_t file_buf1[WORD_SIZE];
static int16_t file_buf2[WORD_SIZE];
static int16_t * buf_pointer;
FIL *fil;
static UINT bytes_read;
__IO int16_t note;
extern uint32_t sdram_pointer;
extern __IO FUNCTION function;
extern __IO CHANNEL ch1;

void SD_readSample(){

	if(word_count == WORD_SIZE){
	    word_count = 0;		//Reset the count
	    need_new_data = TRUE;
	    if(new_buffer_ready == FALSE)
	    	return;
		}
	ch1.CurrentSample = buf_pointer[word_count];
	play_sample(&ch1);
	word_count += 1;
}

void SD_readSingleTrack(FIL *fp){
	f_read(fp,file_buf1,BYTE_SIZE,&bytes_read);
	f_read(fp,file_buf2,BYTE_SIZE,&bytes_read);
	buf_pointer = file_buf1;
	new_buffer_ready = TRUE;
	play_buffer = 0;

	 while(function == READ_SD)
		 continue;

	 while(function == PLAY_SD){

		 while(need_new_data == FALSE){
			 if(function != PLAY_SD)
				 break;
		 }

	   	need_new_data = FALSE;	//Clear the flag.
	   	if(play_buffer == 0)	//play_buffer indicates which buffer is now empty
	    {
	    	//Get the next BUFFERSIZE bytes from the file.
	   		buf_pointer = file_buf2;
	   		new_buffer_ready = TRUE;
	   		f_read(fp, file_buf1, BYTE_SIZE,&bytes_read);
	    	play_buffer = 1;
	    }
	     else
	     {
	        //Get the next BUFFERSIZE bytes from the file.
	    	 buf_pointer = file_buf1;
	    	 new_buffer_ready = TRUE;
	    	 f_read(fp, file_buf2,BYTE_SIZE,&bytes_read);
	    	 play_buffer = 0;
	     }
	     	//new_buffer_ready flag tells the ISR that the buffer has been filled.
	     //If file_read returns 0 or -1 file is over. Find the next file!
	      if(bytes_read == 0)
	      {
	    	function = SINGLE_CHANNEL;	//Disable interrupts to stop playback.
	        play_buffer = 0;
	        f_lseek(fp,SEEK_SET);
	        f_read(fp,file_buf1,BYTE_SIZE,&bytes_read);
	        f_read(fp,file_buf2,BYTE_SIZE,&bytes_read);
	        function = PLAY_SD;
	      }

	}


}

void SF3_readSingleTrack(spiffs * fs,spiffs_file fh){
	function = NONE;
		SPIFFS_read(fs,fh,file_buf1,BYTE_SIZE);
		SPIFFS_read(fs,fh,file_buf2,BYTE_SIZE);
		function = PLAY_SD;

		 while(1){

			 while(need_new_data == FALSE){

			 }

		   	need_new_data = FALSE;	//Clear the flag.
		   	if(play_buffer == 0)	//play_buffer indicates which buffer is now empty
		    {
		    	//Get the next BUFFERSIZE bytes from the file.
		   		buf_pointer = file_buf2;
		   		play_buffer = 1;
		   		new_buffer_ready = TRUE;
		   		SPIFFS_read(fs,fh,file_buf1,BYTE_SIZE);

		    }
		     else
		     {
		        //Get the next BUFFERSIZE bytes from the file.
		    	 buf_pointer = file_buf1;
		    	 play_buffer = 0;
		    	 new_buffer_ready = TRUE;
		    	 SPIFFS_read(fs,fh,file_buf2,BYTE_SIZE);

		     }
		     	//new_buffer_ready flag tells the ISR that the buffer has been filled.
		     //If file_read returns 0 or -1 file is over. Find the next file!
		      if(SPIFFS_eof(fs,fh))
		      {
		    	function = SINGLE_CHANNEL;;	//Disable interrupts to stop playback.
		        play_buffer = 0;
		        SPIFFS_lseek(fs,fh,0,SPIFFS_SEEK_SET);
		        SPIFFS_read(fs,fh,file_buf1,BYTE_SIZE);
		        SPIFFS_read(fs,fh,file_buf2,BYTE_SIZE);
		        function = PLAY_SD;
		      }

		}
}
void SF3_writeSingleTrack(__IO CHANNEL *ch,spiffs * fs,spiffs_file fh){

	uint32_t allbytes = ch->SamplesWritten * 2;
	uint32_t remainder = allbytes % BYTE_SIZE;

	sdram_pointer = 0;

		while(allbytes > 0){
			if(allbytes > remainder){
				BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *) file_buf1, WORD_SIZE);
				SPIFFS_write(fs,fh,file_buf1,BYTE_SIZE);
				allbytes -= BYTE_SIZE;
				sdram_pointer += BYTE_SIZE;
			}
			else{
				BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *) file_buf1, remainder / 2);
				SPIFFS_write(fs,fh,file_buf1,remainder);
				allbytes -= remainder;
				sdram_pointer += remainder;
			}

			SPIFFS_fflush(fs,fh);

		}
}
void SF3_readSample(){
	if(word_count == WORD_SIZE){
		word_count = 0;		//Reset the count
		need_new_data = TRUE;
	}
	ch1.CurrentSample = buf_pointer[word_count];
	play_sample(&ch1);
	word_count += 1;
}
//int16_t readSampleSD(){
//	int16_t sample;
//
//	BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *)&sample,1);
//	sdram_pointer += 2;
//	if(sdram_pointer == fil->fsize || sdram_pointer == SDRAM_SIZE){
//		sdram_pointer = 0;
//
//	}
//	return sample;
//}

//void readSingleTrackSD(FIL *fp){
//	buff_switch = 1;
//	data_ready = FALSE;
//	buffer_index = 0;
//	sdram_pointer = 0;
//	fil = fp;
//
//	while(1){
//		f_read(fp,file_buf1,BUFF_SIZE,&bytes);
//		BSP_SDRAM_WriteData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t*)file_buf1,bytes / 2);
//		sdram_pointer += bytes;
//		if(f_eof(fp))
//			break;
//	}
//
//	sdram_pointer = 0;
//	function = PLAY_SD;
//
//}

void SD_writeSingleTrack(__IO CHANNEL *ch,FIL *fp){
	uint32_t allbytes = ch->SamplesWritten * 2;
	uint32_t remainder = allbytes % BYTE_SIZE;
	sdram_pointer = 0;
	f_truncate(fp);
	while(allbytes > 0){
		if(allbytes > remainder){
			BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *) file_buf1, WORD_SIZE);
			f_write(fp,file_buf1,BYTE_SIZE,&bytes_read);
			allbytes -= BYTE_SIZE;
			sdram_pointer += BYTE_SIZE;
		}
		else{
			BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *) file_buf1, remainder / 2);
			f_write(fp,file_buf1,remainder,&bytes_read);
			allbytes -= remainder;
			sdram_pointer += remainder;
		}

	}


}

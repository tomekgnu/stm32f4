#include "audio.h"
#include "fileops.h"
#include "main.h"
#include "stm32f429i_discovery_sdram.h"
#include "tm_stm32_hd44780.h"
#include "spiffs.h"
#include "spiffs_nucleus.h"
#include "fatfs_sd.h"
#include "dma.h"
#include "tim.h"
#include "dac.h"

static __IO uint8_t play_buffer = 0;					//Keeps track of which buffer is currently being used
static __IO BOOL need_new_data = FALSE;
static __IO uint32_t word_count = 0;
static int16_t audio_buf[WORD_SIZE];
static int16_t * buf_pointer;
FIL *fil;
static UINT bytes_read;
__IO int16_t note;
extern uint32_t sdram_pointer;
extern __IO FUNCTION function;
extern __IO CHANNEL ch1;


void SD_readSample(){

	if(word_count < WORD_SIZE){
		ch1.CurrentSample = audio_buf[word_count * 2 + WORD_SIZE * play_buffer];
		play_sample(&ch1);
		word_count += 1;
		return;
	}

	need_new_data = TRUE;
	word_count = 0;		//Reset the count


}

void HAL_DACEx_ConvCpltCallbackCh2(DAC_HandleTypeDef* hdac){
	need_new_data = TRUE;
	play_buffer = 0;
	return;
}

void HAL_DACEx_ConvHalfCpltCallbackCh2(DAC_HandleTypeDef* hdac){
	need_new_data = TRUE;
	play_buffer = 1;
	return;
}

void SD_readSingleTrack(FIL *fp){
	f_read(fp,audio_buf,WORD_SIZE,&bytes_read);
	f_read(fp,(uint8_t *)audio_buf + WORD_SIZE,WORD_SIZE,&bytes_read);
	signed16_unsigned12(audio_buf,0,WORD_SIZE);
	play_buffer = 0;
	word_count = 0;

	while(function != PLAY_SD)	// wait for function switch
		 continue;

	HAL_TIM_Base_Start(&htim8);
	HAL_DAC_Start(&hdac,DAC_CHANNEL_2);
	HAL_DAC_Start_DMA(&hdac,DAC_CHANNEL_2,(uint32_t*)audio_buf,WORD_SIZE,DAC_ALIGN_12B_R);

	while(function == PLAY_SD){
		 while(need_new_data == FALSE){
			 if(function != PLAY_SD){
				 HAL_DAC_Stop_DMA(&hdac,DAC_CHANNEL_2);
				 HAL_DAC_Stop(&hdac,DAC_CHANNEL_2);
				 HAL_TIM_Base_Stop(&htim8);
				 return;
			 }
		 }

		need_new_data = FALSE;
		if(play_buffer == 0)	//play_buffer indicates which buffer is now empty
	    {
	   		f_read(fp, (uint8_t *)audio_buf + WORD_SIZE, WORD_SIZE,&bytes_read);
	   		signed16_unsigned12(audio_buf,WORD_HALF_SIZE,WORD_SIZE);
	    }
	     else
	     {
	    	 f_read(fp, (uint8_t *)audio_buf,WORD_SIZE,&bytes_read);
	    	 signed16_unsigned12(audio_buf,0,WORD_HALF_SIZE);
	     }	     	//new_buffer_ready flag tells the ISR that the buffer has been filled.
	     //If file_read returns 0 or -1 file is over. Find the next file!
	      if(bytes_read == 0)
	      {
	    	HAL_DAC_Stop_DMA(&hdac,DAC_CHANNEL_2);
	    	HAL_DAC_Stop(&hdac,DAC_CHANNEL_2);
	    	HAL_TIM_Base_Stop(&htim8);
	    	f_lseek(fp,SEEK_SET);
	    	f_read(fp,(uint8_t *)audio_buf,WORD_SIZE,&bytes_read);
	    	f_read(fp,(uint8_t *)audio_buf + WORD_SIZE,WORD_SIZE,&bytes_read);
	    	signed16_unsigned12(audio_buf,0,WORD_SIZE);
	        need_new_data = FALSE;
	        word_count = 0;
	        play_buffer = 0;
	        HAL_TIM_Base_Start(&htim8);
	        HAL_DAC_Start(&hdac,DAC_CHANNEL_2);
	        HAL_DAC_Start_DMA(&hdac,DAC_CHANNEL_2,(uint32_t*)audio_buf,WORD_SIZE,DAC_ALIGN_12B_R);

	      }

	}


}

void SF3_readSingleTrack(spiffs * fs,spiffs_file fh){
//	function = NONE;
//		SPIFFS_read(fs,fh,file_buf[0],BYTE_SIZE);
//		SPIFFS_read(fs,fh,file_buf[1],BYTE_SIZE);
//		need_new_data = FALSE;
//		play_buffer = 0;
//		function = PLAY_SF3;
//		 while(function == READ_SF3)
//			continue;
//
//		 while(function == PLAY_SF3){
//
//			 while(need_new_data == FALSE){
//				 if(function != PLAY_SF3)
//					 return;
//			 }
//
//		   	need_new_data = FALSE;	//Clear the flag.
//		   	if(play_buffer == 0)	//play_buffer indicates which buffer is now empty
//		    {
//		    	//Get the next BUFFERSIZE bytes from the file.
//		   		play_buffer = 1;
//		   		SPIFFS_read(fs,fh,file_buf[0],BYTE_SIZE);
//
//		    }
//		     else
//		     {
//		        //Get the next BUFFERSIZE bytes from the file.
//		    	 play_buffer = 0;
//		    	 SPIFFS_read(fs,fh,file_buf[1],BYTE_SIZE);
//
//		     }
//		     	//new_buffer_ready flag tells the ISR that the buffer has been filled.
//		     //If file_read returns 0 or -1 file is over. Find the next file!
//		      if(SPIFFS_eof(fs,fh))
//		      {
//		    	function = SINGLE_CHANNEL;	//Disable interrupts to stop playback.
//		        play_buffer = 0;
//		        SPIFFS_lseek(fs,fh,0,SPIFFS_SEEK_SET);
//		        SPIFFS_read(fs,fh,file_buf[0],BYTE_SIZE);
//		        SPIFFS_read(fs,fh,file_buf[1],BYTE_SIZE);
//		        need_new_data = FALSE;
//		        play_buffer = 0;
//		        function = PLAY_SD;
//		      }
//
//		}
}
void SF3_writeSingleTrack(__IO CHANNEL *ch,spiffs * fs,spiffs_file fh){

//	uint32_t allbytes = ch->SamplesWritten * 2;
//	uint32_t remainder = allbytes % BYTE_SIZE;
//
//	sdram_pointer = 0;
//
//		while(allbytes > 0){
//			if(allbytes > remainder){
//				BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *) file_buf[0], WORD_SIZE);
//				SPIFFS_write(fs,fh,file_buf[0],BYTE_SIZE);
//				allbytes -= BYTE_SIZE;
//				sdram_pointer += BYTE_SIZE;
//			}
//			else{
//				BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *) file_buf[0], remainder / 2);
//				SPIFFS_write(fs,fh,file_buf[0],remainder);
//				allbytes -= remainder;
//				sdram_pointer += remainder;
//			}
//
//			SPIFFS_fflush(fs,fh);
//
//		}
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
			BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *) audio_buf, WORD_SIZE);
			f_write(fp,audio_buf,BYTE_SIZE,&bytes_read);
			allbytes -= BYTE_SIZE;
			sdram_pointer += BYTE_SIZE;
		}
		else{
			BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *) audio_buf, remainder / 2);
			f_write(fp,audio_buf,remainder,&bytes_read);
			allbytes -= remainder;
			sdram_pointer += remainder;
		}

	}

}
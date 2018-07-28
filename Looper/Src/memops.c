#include "audio.h"
#include "memops.h"
#include "main.h"
#include "stm32f429i_discovery_sdram.h"
#include "tm_stm32_hd44780.h"
#include "spiffs.h"
#include "spiffs_nucleus.h"
#include "fatfs_sd.h"
#include "dma.h"
#include "tim.h"
#include "dac.h"
#include "SRAMDriver.h"

static __IO uint8_t play_buffer = 0;					//Keeps track of which buffer is currently being used
static __IO BOOL need_new_data = FALSE;
static __IO uint32_t word_count = 0;
int16_t audio_buf[WORD_SIZE];
uint32_t audio_pointer = 0;
BOOL audio_finished = FALSE;
static int16_t * buf_pointer;
FIL *fil;
static UINT bytes_read;
static UINT bytes_written;
__IO int16_t note;
extern uint32_t sdram_pointer;
extern __IO FUNCTION LooperFunction;
extern __IO CHANNEL ch1;
extern __IO CHANNEL ch2;

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

void SRAM_readSingleTrack() {
	HAL_StatusTypeDef stat = HAL_OK;
	SRAM_seekRead(0,SRAM_SET);
	readSRAM((uint8_t *) audio_buf, WORD_SIZE);
	readSRAM((uint8_t *) audio_buf + WORD_SIZE, WORD_SIZE);
	signed16_unsigned12(audio_buf, 0, WORD_SIZE);
	play_buffer = 0;
	word_count = 0;

	while (LooperFunction != PLAY_SRAM)	// wait for function switch
		continue;

	//stat = HAL_TIM_Base_Start(&htim8);
	audio_pointer = 0;
	stat = HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
	stat = HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*) audio_buf, WORD_SIZE,DAC_ALIGN_12B_R);
	HAL_TIM_Base_Start_IT(&htim8);

	if(stat != HAL_OK){
		BSP_LED_On(LED_RED);
		Error_Handler();
	}

	while (LooperFunction == PLAY_SRAM) {
		while (need_new_data == FALSE) {
			if (LooperFunction != PLAY_SRAM) {
				HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_2);
				HAL_DAC_Stop(&hdac, DAC_CHANNEL_2);
				HAL_TIM_Base_Stop_IT(&htim8);
				return;
			}
		}

		need_new_data = FALSE;
		if (play_buffer == 0)//play_buffer indicates which buffer is now empty
		{
			readSRAM((uint8_t *) audio_buf + WORD_SIZE, WORD_SIZE);
			signed16_unsigned12(audio_buf, WORD_HALF_SIZE, WORD_SIZE);
		} else {
			readSRAM((uint8_t *) audio_buf, WORD_SIZE);
			signed16_unsigned12(audio_buf, 0, WORD_HALF_SIZE);
		}//new_buffer_ready flag tells the ISR that the buffer has been filled.
		//If file_read returns 0 or -1 file is over. Find the next file!
		if (SRAM_read() > SRAM_written()) {
			while(audio_finished == FALSE)
				continue;
			HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_2);
			HAL_DAC_Stop(&hdac, DAC_CHANNEL_2);
			//HAL_TIM_Base_Stop(&htim8);
			SRAM_seekRead(0,SRAM_SET);
			readSRAM((uint8_t *) audio_buf, WORD_SIZE);
			readSRAM((uint8_t *) audio_buf + WORD_SIZE, WORD_SIZE);
			signed16_unsigned12(audio_buf, 0, WORD_SIZE);
			need_new_data = FALSE;
			word_count = 0;
			play_buffer = 0;
			audio_finished = FALSE;
			//HAL_TIM_Base_Start(&htim8);
			HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
			HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*) audio_buf,WORD_SIZE, DAC_ALIGN_12B_R);

		}

	}

	HAL_TIM_Base_Stop_IT(&htim8);

}

void SRAM_writeSingleTrack(__IO CHANNEL *ch){
	uint32_t allbytes = ch->SamplesWritten * 2;
	uint32_t remainder = allbytes % BYTE_SIZE;
	SRAM_seekWrite(0,SRAM_SET);
	sdram_pointer = 0;

	while(allbytes > 0){
		if(allbytes > remainder){
			BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *)audio_buf, WORD_SIZE);
			writeSRAM((uint8_t *)audio_buf,BYTE_SIZE);
			allbytes -= BYTE_SIZE;
			sdram_pointer += BYTE_SIZE;
		}
		else{
			BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *)audio_buf, remainder / 2);
			writeSRAM((uint8_t *)audio_buf,remainder);
			allbytes -= remainder;
			sdram_pointer += remainder;
		}

	}

}

void SD_readSingleTrack(FIL *fp){

	f_read(fp,(uint8_t *)audio_buf,WORD_SIZE,&bytes_read);
	f_read(fp,(uint8_t *)audio_buf + WORD_SIZE,WORD_SIZE,&bytes_read);
	signed16_unsigned12(audio_buf,0,WORD_SIZE);
	play_buffer = 0;
	word_count = 0;

	while(LooperFunction != PLAY_SD)	// wait for function switch
		 continue;


	HAL_DAC_Start(&hdac,DAC_CHANNEL_2);
	HAL_DAC_Start_DMA(&hdac,DAC_CHANNEL_2,(uint32_t*)audio_buf,WORD_SIZE,DAC_ALIGN_12B_R);
	HAL_TIM_Base_Start_IT(&htim8);

	while(LooperFunction == PLAY_SD){
		 while(need_new_data == FALSE){
			 if(LooperFunction != PLAY_SD){
				 HAL_DAC_Stop_DMA(&hdac,DAC_CHANNEL_2);
				 HAL_DAC_Stop(&hdac,DAC_CHANNEL_2);
				 HAL_TIM_Base_Stop_IT(&htim8);
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
	    	//HAL_TIM_Base_Stop(&htim8);
	    	f_lseek(fp,SEEK_SET);
	    	f_read(fp,(uint8_t *)audio_buf,WORD_SIZE,&bytes_read);
	    	f_read(fp,(uint8_t *)audio_buf + WORD_SIZE,WORD_SIZE,&bytes_read);
	    	signed16_unsigned12(audio_buf,0,WORD_SIZE);
	        need_new_data = FALSE;
	        word_count = 0;
	        play_buffer = 0;
	        //HAL_TIM_Base_Start(&htim8);
	        HAL_DAC_Start(&hdac,DAC_CHANNEL_2);
	        HAL_DAC_Start_DMA(&hdac,DAC_CHANNEL_2,(uint32_t*)audio_buf,WORD_SIZE,DAC_ALIGN_12B_R);

	      }

	}
	HAL_TIM_Base_Stop_IT(&htim8);

}

void SF3_readSingleTrack(spiffs * fs,spiffs_file fh){
	HAL_TIM_Base_Start(&htim8);
		SPIFFS_read(fs,fh,(uint8_t *)audio_buf,WORD_SIZE);
		SPIFFS_read(fs,fh,(uint8_t *)audio_buf + WORD_SIZE,WORD_SIZE);
		signed16_unsigned12(audio_buf,0,WORD_SIZE);
		play_buffer = 0;
		word_count = 0;
		while(LooperFunction != PLAY_SF3)
			continue;

		HAL_DAC_Start(&hdac,DAC_CHANNEL_2);
		HAL_DAC_Start_DMA(&hdac,DAC_CHANNEL_2,(uint32_t*)audio_buf,WORD_SIZE,DAC_ALIGN_12B_R);

		while(LooperFunction == PLAY_SF3){

			 while(need_new_data == FALSE){
				 if(LooperFunction != PLAY_SF3){
					 HAL_DAC_Stop_DMA(&hdac,DAC_CHANNEL_2);
					 HAL_DAC_Stop(&hdac,DAC_CHANNEL_2);
					// HAL_TIM_Base_Stop(&htim8);
					 return;
				 }
			 }

			need_new_data = FALSE;
		   	if(play_buffer == 0)	//play_buffer indicates which buffer is now empty
		    {
		    	SPIFFS_read(fs,fh,(uint8_t *)audio_buf + WORD_SIZE,WORD_SIZE);
		    	signed16_unsigned12(audio_buf,WORD_HALF_SIZE,WORD_SIZE);
		    }
		     else
		     {
		    	 SPIFFS_read(fs,fh,(uint8_t *)audio_buf,WORD_SIZE);
		    	 signed16_unsigned12(audio_buf,0,WORD_HALF_SIZE);
		     }

		      if(SPIFFS_eof(fs,fh))
		      {
		    	  HAL_DAC_Stop_DMA(&hdac,DAC_CHANNEL_2);
		    	  HAL_DAC_Stop(&hdac,DAC_CHANNEL_2);
		    	 // HAL_TIM_Base_Stop(&htim8);
		    	  SPIFFS_lseek(fs,fh,0,SPIFFS_SEEK_SET);
		    	  SPIFFS_read(fs,fh,(uint8_t *)audio_buf,WORD_SIZE);
		    	  SPIFFS_read(fs,fh,(uint8_t *)audio_buf + WORD_SIZE,WORD_SIZE);
		    	  signed16_unsigned12(audio_buf,0,WORD_SIZE);
		    	  need_new_data = FALSE;
		    	  word_count = 0;
		    	  play_buffer = 0;
		    	  HAL_TIM_Base_Start(&htim8);
		    	  HAL_DAC_Start(&hdac,DAC_CHANNEL_2);
		    	  HAL_DAC_Start_DMA(&hdac,DAC_CHANNEL_2,(uint32_t*)audio_buf,WORD_SIZE,DAC_ALIGN_12B_R);
		      }

		}

		HAL_TIM_Base_Stop(&htim8);
}
void SF3_writeSingleTrack(__IO CHANNEL *ch,spiffs * fs,spiffs_file fh){

	uint32_t allbytes = ch->SamplesWritten * 2;
	uint32_t remainder = allbytes % BYTE_SIZE;

	sdram_pointer = 0;

		while(allbytes > 0){
			if(allbytes > remainder){
				BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *)audio_buf, WORD_SIZE);
				SPIFFS_write(fs,fh,(uint8_t *)audio_buf,BYTE_SIZE);
				allbytes -= BYTE_SIZE;
				sdram_pointer += BYTE_SIZE;
			}
			else{
				BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *)audio_buf, remainder / 2);
				SPIFFS_write(fs,fh,(uint8_t *)audio_buf,remainder);
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

void SD_readToSDRAM(FIL *fp){
	uint8_t *_buf;
	fil = fp;
	resetChannels(&ch1,&ch2);
	_buf = (uint8_t *)malloc(8192);
	bytes_read = 0;
	sdram_pointer = 0;
	while(1){
		f_read(fp,(uint8_t *)_buf,8192,&bytes_read);
		BSP_SDRAM_WriteData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t*)_buf,bytes_read / 2);
		ch1.SamplesWritten += (bytes_read / 2);
		sdram_pointer += bytes_read;
		if(f_eof(fp))
			break;
	}
	free(_buf);
	sdram_pointer = 0;

}

void SD_writeSingleTrack(__IO CHANNEL *ch,FIL *fp){
	uint8_t *_buf;
	uint32_t bytesRemaining = ch->SamplesWritten * 2;
	_buf = (uint8_t *)malloc(8192);
	bytes_written = 0;
	sdram_pointer = 0;
	f_truncate(fp);
	while(bytesRemaining > 0){
		if(bytesRemaining >= 8192){
			BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *) _buf, 4096);
			f_write(fp,_buf,8192,&bytes_written);
		}
		else{
			BSP_SDRAM_ReadData16b(SDRAM_DEVICE_ADDR + sdram_pointer,(uint16_t *) _buf, bytesRemaining / 2);
			f_write(fp,_buf,bytesRemaining,&bytes_written);
		}
		bytesRemaining -= bytes_written;
		sdram_pointer += bytes_written;
	}

	free(_buf);

}
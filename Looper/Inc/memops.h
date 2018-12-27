#ifndef __FILEOPS_H
#define __FILEOPS_H

#include "stm32f429xx.h"
#include "ff.h"
#include "spiffs.h"

#define WORD_SIZE		256
#define WORD_HALF_SIZE	(WORD_SIZE / 2)
#define BYTE_SIZE	(WORD_SIZE * 2)

extern FATFS FatFs;

void SD_readSingleTrack(FIL *fp);
void SD_WriteAudio(uint32_t start,uint32_t end,FIL *fp);
uint32_t SD_ReadAudio(uint32_t start,FIL *fp);
void checkSD();

uint32_t SF3_ReadAudio(uint32_t start,spiffs * fs,spiffs_file fh);
void SF3_WriteAudio(uint32_t start,uint32_t end,spiffs * fs,spiffs_file fh);
void SF3_readSample();

void SRAM_readSingleTrack();
void SRAM_writeSingleTrack(__IO CHANNEL *ch);
void SRAM_download_rhythm(void);

#endif

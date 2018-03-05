#ifndef __FILEOPS_H
#define __FILEOPS_H

#include "stm32f429xx.h"
#include "ff.h"
#include "spiffs.h"

#define WORD_SIZE		128
#define WORD_HALF_SIZE	(WORD_SIZE / 2)
#define BYTE_SIZE	(WORD_SIZE * 2)

void SD_readSingleTrack(FIL *fp);
void SD_writeSingleTrack(__IO CHANNEL *ch,FIL *fp);
void SD_readSample();

void SF3_readSingleTrack(spiffs * fs,spiffs_file fh);
void SF3_writeSingleTrack(__IO CHANNEL *ch,spiffs * fs,spiffs_file fh);
void SF3_readSample();

#endif

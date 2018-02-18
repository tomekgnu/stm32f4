#ifndef __FILEOPS_H
#define __FILEOPS_H

#include "stm32f429xx.h"
#include "ff.h"

#define BUFF_SIZE	512

void readSingleTrackSD(FIL *fp);
void writeSingleTrackSD(__IO CHANNEL *ch,FIL *fp);
int16_t readSampleSD();



#endif

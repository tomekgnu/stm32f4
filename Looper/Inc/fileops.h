#ifndef __FILEOPS_H
#define __FILEOPS_H

#include "stm32f429xx.h"
#include "ff.h"

void readSingleTrackSD(FIL *fp);
void writeSingleTrackSD(__IO CHANNEL *ch,FIL *fp);




#endif

#ifndef __AUDIO_H__
#define __AUDIO_H__
#include "main.h"


void showMinMaxSamples(int32_t min,int32_t max);
void play32s(int16_t newsample,FUNCTION channel);
void record32s(int16_t sample,FUNCTION channel);

#endif

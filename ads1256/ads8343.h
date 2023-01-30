#ifndef __ADS8343_H__
#define __ADS8343_H__

#include "main.h"

#define CS_1() 		HAL_GPIO_WritePin(ADCCS_GPIO_Port,ADCCS_Pin,GPIO_PIN_SET)
#define CS_0()  	HAL_GPIO_WritePin(ADCCS_GPIO_Port,ADCCS_Pin,GPIO_PIN_RESET)

#define CS1_1() 	HAL_GPIO_WritePin(DACCS_GPIO_Port,DACCS_Pin,GPIO_PIN_SET)
#define CS1_0()  	HAL_GPIO_WritePin(DACCS_GPIO_Port,DACCS_Pin,GPIO_PIN_RESET)

typedef enum{SINGLE,DIFFR}ADC_Mode;

uint32_t read(int channel, ADC_Mode mode);
int signedRead(int channel, bool mode);
void writeOneChannel(int channel, unsigned int value); //any one channel
void writeTwoChannels(unsigned int value0, unsigned int value1, bool simul); //channels 0 and 1
void writeThreeChannels(unsigned int value0, unsigned int value1, unsigned int value2, bool simul); //channels 0-3
void writeAllChannels(unsigned int value0, unsigned int value1, unsigned int value2, unsigned int value3, bool simul); //all channels

#endif

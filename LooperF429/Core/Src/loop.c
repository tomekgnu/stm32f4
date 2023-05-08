/*
 * loop.c
 *
 *  Created on: Apr 5, 2023
 *      Author: Tomek
 */
#include "main.h"
#include "loop.h"
#include "pot.h"
#include "dac8564.h"
#include "ads8343.h"
#include <stdbool.h>
#include <stdlib.h>
#include "spi.h"



int16_t minInputSample = MAX_PEAK;
int16_t maxInputSample = MIN_PEAK;
int16_t minRecordedSample = 0;
int16_t maxRecordedSample = 0;

int tmp = 0;
bool clipping = false;
short tmpOffset = 0, finalOffset = 0;
float gain = 1.0;
bool offsetReady = false;
int16_t signedResult;
uint8_t calibration = 4;
extern SPI_HandleTypeDef hspi1;


bool loop = false;
uint32_t count = 0;
int16_t samples[20000];

void loopBack() {
	signedResult = ads8343_Read(AudioInput0);
	dac8564_Write(AudioOutput0, signedResult);
}


void recordLoop() {

	tmp = ads8343_Read(AudioInput0);
	if (tmp > maxInputSample) {
		maxInputSample = tmp;

	}
	if(tmp < minInputSample){
		minInputSample = tmp;
	}
	signedResult = (tmp - finalOffset) * gain;

	if (loop == false) {
		samples[count] = signedResult;
	}

	else {

		samples[count] = samples[count]  + signedResult;
	}

	if(samples[count] > maxRecordedSample)
		maxRecordedSample = samples[count];
	if(samples[count] < minRecordedSample)
		minRecordedSample = samples[count];

	dac8564_Write(0x10, samples[count]);


	count++;


		//correctOffset(count, samples[count], &finalOffset);


		if (count == 20000) {
			count = 0;
			loop = true;
			finalOffset = (minInputSample + maxInputSample) / 2;

			if ((maxRecordedSample - minRecordedSample) > 3000) {
				clipping = true;
				gain = (float)3000 / (float)(maxRecordedSample - minRecordedSample);

			} else {
				gain = 1.0;
				clipping = false;
			}

			maxInputSample = MIN_PEAK;
			minInputSample = MAX_PEAK;
			minRecordedSample = 0;
			maxRecordedSample = 0;

		}



}

float calculateOffset(){
	uint16_t count;
	int16_t value;
	int64_t total = 0;

	for(count = 0; count < 20000; count++){
		value = ads8343_Read(AudioInput0);
		total += value;
	}

	return (float)total / (float)count;
}

void adjustPot(){
	float offset = calculateOffset();
	if(offset < 0){
		POT_Up();
		do{
			POT_Move();
			offset = calculateOffset();
		}while(offset < 0);

	}
	else if(offset > 0){
		POT_Down();
		do{
			POT_Move();
			offset = calculateOffset();
		}while(offset > 0);

	}

}

void correctOffset(uint32_t count, short value, short *avg) {
	static int64_t sum = 0;
	if (count < 20000) {
		sum += value;
	} else {
		*avg = (short) ((float) sum / (float) count);
		sum = 0;
	}

}

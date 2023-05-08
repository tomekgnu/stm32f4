/*
 * ad8343.c
 *
 *  Created on: Apr 3, 2023
 *      Author: Tomek
 */

#include "main.h"
#include "spi.h"
#include "ads8343.h"

int16_t ads8343_Read(uint8_t ch){
	uint8_t controlByte = (0x86 | ch);	// internal clock single-ended or'ed with channel
	uint8_t request[3] = { 0x00, 0x00, controlByte };
	uint8_t response[3];
	int16_t result;

	HAL_GPIO_WritePin(ADCCS_GPIO_Port, ADCCS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&audioSpi, request, response, 3, 100);
	HAL_GPIO_WritePin(ADCCS_GPIO_Port, ADCCS_Pin, GPIO_PIN_SET);

    response[0] <<= 1;
    if(response[1] & 0x80)
		response[0] += 1;
	response[1] <<= 1;
	if(response[2] & 0x80)
		response[1] |= 0x01;

	result = (int16_t)response[0] << 8;
	result += response[1];

	return result;
}

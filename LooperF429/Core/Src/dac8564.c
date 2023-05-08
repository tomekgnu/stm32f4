/*
 * dac8564.c
 *
 *  Created on: Apr 3, 2023
 *      Author: Tomek
 */
#include "main.h"
#include "spi.h"
#include "dac8564.h"

__STATIC_INLINE void DelayUS(__IO uint32_t micros) {

#if !defined(STM32F0xx)
    uint32_t start = DWT->CYCCNT;

    /* Go to number of cycles for system */
    micros *= (HAL_RCC_GetHCLKFreq() / 1000000);

    /* Delay till end */
    while ((DWT->CYCCNT - start) < micros);
#else
    /* Go to clock cycles */
    micros *= (SystemCoreClock / 1000000) / 5;

    /* Wait till done */
    while (micros--);
#endif
}

void dac8564_Write(uint8_t channel, int16_t sample) {
	uint8_t dac[3] = { 0x10, 0x00, 0x00 };
		uint16_t result;
		// convert to unsigned
		if (sample < 0) {
			result = (sample & 0x7FFF);
		} else {
			result = (sample | 0x8000);
		}
		dac[0] |= channel;
		dac[1] = result >> 8;
		dac[2] = result & 0x00FF;

		HAL_GPIO_WritePin(DACCS_GPIO_Port, DACCS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&audioSpi, dac, 3, 100);
		//HAL_SPI_Transmit(&audioSpi, dac + 1, 2, 100);
		HAL_GPIO_WritePin(DACCS_GPIO_Port, DACCS_Pin, GPIO_PIN_SET);



}

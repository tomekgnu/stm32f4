/*
 * pot.c
 *
 *  Created on: 3 kwi 2023
 *      Author: Tomek
 */

#include "main.h"
#include "pot.h"
#include "stm32f4xx_hal_gpio.h"

void POT_Down() {
	HAL_GPIO_WritePin(POT_CS_GPIO_Port, POT_CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(POT_DIR_GPIO_Port, POT_DIR_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(POT_CS_GPIO_Port, POT_CS_Pin, GPIO_PIN_SET);
}

void POT_Up() {
	HAL_GPIO_WritePin(POT_CS_GPIO_Port, POT_CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(POT_DIR_GPIO_Port, POT_DIR_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(POT_CS_GPIO_Port, POT_CS_Pin, GPIO_PIN_SET);
}

void POT_Move() {
	HAL_GPIO_WritePin(POT_CS_GPIO_Port, POT_CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(POT_INC_GPIO_Port, POT_INC_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(POT_INC_GPIO_Port, POT_INC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(POT_CS_GPIO_Port, POT_CS_Pin, GPIO_PIN_SET);
}

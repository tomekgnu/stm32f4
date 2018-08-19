/*
 * joystick.c
 *
 *  Created on: 19.08.2018
 *      Author: Tomek
 */
#include "stm32f429xx.h"
#include "joystick.h"
#include "adc.h"

static uint16_t millis = 0;
static __IO JOYSTICK jstick;
static JOYSTICK novalue = {0};
static BOOL active = FALSE;

static JOYSTICK Read_Joystick_INT(){
	jstick.but = !HAL_GPIO_ReadPin(Joystick_SW_GPIO_Port,Joystick_SW_Pin);
	HAL_ADC_Start(&hadc2);
	HAL_ADC_PollForConversion(&hadc2, 100);
	jstick.xpos = HAL_ADC_GetValue(&hadc2);

	HAL_ADC_PollForConversion(&hadc2, 100);
	jstick.ypos = HAL_ADC_GetValue(&hadc2);

	HAL_ADC_Stop(&hadc2);



	return jstick;

}

JOYSTICK Read_Joystick(){
	if(active == TRUE)
		return novalue;

	if(jstick.xpos != 32 || jstick.ypos != 32 || jstick.but == TRUE){
		active = TRUE;
		return jstick;
	}

	return novalue;
}

void Update_Joystick(){
	Read_Joystick_INT();

	if(++millis >= 500){
		millis = 0;
		active = FALSE;
	}

}

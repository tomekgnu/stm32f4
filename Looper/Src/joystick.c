/*
 * joystick.c
 *
 *  Created on: 19.08.2018
 *      Author: Tomek
 */
#include "stm32f429xx.h"
#include "joystick.h"
#include "adc.h"



JOYSTICK Read_Joystick(){

	JOYSTICK jstick;

	if (HAL_ADC_PollForConversion(&hadc2, 1000) == HAL_OK) {
			  jstick.xpos = HAL_ADC_GetValue(&hadc2);
			  HAL_ADC_Start(&hadc2);
		  }
		  if (HAL_ADC_PollForConversion(&hadc3, 1000) == HAL_OK) {
			  jstick.ypos = HAL_ADC_GetValue(&hadc3);
		  	  HAL_ADC_Start(&hadc3);
		  }

		  jstick.but = HAL_GPIO_ReadPin(Joystick_SW_GPIO_Port,Joystick_SW_Pin);

		  return jstick;

}

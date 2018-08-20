/*
 * joystick.c
 *
 *  Created on: 19.08.2018
 *      Author: Tomek
 */
#include "stm32f429xx.h"
#include "main.h"
#include "joystick.h"
#include "adc.h"

static uint16_t millis = 0;
static __IO JOYSTICK jstick;
static JOYSTICK novalue = {0};
static BOOL active = FALSE;


JOYSTICK Read_Joystick_INT(){


	jstick.but = !HAL_GPIO_ReadPin(Joystick_SW_GPIO_Port,Joystick_SW_Pin);
	jstick.xpos = joystick_data[0];
	jstick.ypos = joystick_data[1];

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

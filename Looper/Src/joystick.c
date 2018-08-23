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

static void Read_Joystick_INT(){

	jstick.but = (HAL_GPIO_ReadPin(Joystick_SW_GPIO_Port,Joystick_SW_Pin) == GPIO_PIN_RESET);
	jstick.xpos = (joystick_data[0] >> 3);
	jstick.ypos = (joystick_data[1] >> 3);
}

JOYSTICK Read_Joystick(){
	return jstick;
}


void Update_Joystick(){
	Read_Joystick_INT();

	if(++millis >= 50000){
		millis = 0;
	}

}

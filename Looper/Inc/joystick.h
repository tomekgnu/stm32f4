#ifndef __JOYSTICK_H__
#define  __JOYSTICK_H__

#include "main.h"

typedef struct{
	uint8_t xpos;
	uint8_t ypos;
	BOOL	but;
} JOYSTICK;


JOYSTICK Read_Joystick();
void Update_Joystick();
BOOL Joystick_Busy();

#endif

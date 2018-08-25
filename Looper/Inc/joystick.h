#ifndef __JOYSTICK_H__
#define  __JOYSTICK_H__

#include "main.h"

typedef struct{
	uint8_t xpos;
	uint8_t ypos;
	BOOL	but;
} JOYSTICK;

#define CENTER	4
#define MAX 	7
#define MIN		0

JOYSTICK Read_Joystick();
void Wait_Joystick();
void Update_Joystick();
BOOL Active_Joystick();

#endif

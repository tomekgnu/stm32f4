#ifndef __MENU_H__
#define  __MENU_H__

#include "main.h"
#include "stm32f429xx.h"

void setDrumPart(uint8_t lev);
void menuDrumEdit();
void placeDrumSymbol(uint8_t val);
void setDrumLevel(uint8_t lev);
void moveBeatBack();
void moveBeatForward();
#endif

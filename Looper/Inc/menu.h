#ifndef __MENU_H__
#define  __MENU_H__

#include "main.h"
#include "audio.h"
#include "stm32f429xx.h"

void menuShowTimers(__IO CHANNEL *ch1,__IO CHANNEL *ch2);
void setDrumPart(uint8_t lev);
void menuDrumEdit();
void clearDrumSymbol();
void placeDrumFromKeyboard(uint8_t val);
void placeDrumFromFile(uint8_t val);
void setDrumLevel(uint8_t lev);
void moveBeatBack();
void moveBeatForward();
void drawActiveBeat();
void resetBeat();
#endif

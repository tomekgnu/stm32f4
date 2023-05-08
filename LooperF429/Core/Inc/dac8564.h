/*
 * dac8564.h
 *
 *  Created on: Apr 3, 2023
 *      Author: Tomek
 */

#ifndef INC_DAC8564_H_
#define INC_DAC8564_H_

#define AudioOutput0		0x00
#define AudioOutput1		0x02
#define AudioOutput2		0x04
#define AudioOutput3		0x06

void dac8564_Write(uint8_t,int16_t);

#endif /* INC_DAC8564_H_ */

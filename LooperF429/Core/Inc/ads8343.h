/*
 * ad8343.h
 *
 *  Created on: Apr 3, 2023
 *      Author: Tomek
 */

#ifndef INC_ADS8343_H_
#define INC_ADS8343_H_

#define AudioInput0			0x10
#define AudioInput1			0x50
#define AudioInput2			0x20
#define AudioInput3			0x60

int16_t ads8343_Read(uint8_t ch);

#endif /* INC_ADS8343_H_ */

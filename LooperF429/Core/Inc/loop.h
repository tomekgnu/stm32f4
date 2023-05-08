/*
 * loop.h
 *
 *  Created on: Apr 5, 2023
 *      Author: Tomek
 */

#ifndef INC_LOOP_H_
#define INC_LOOP_H_

#define MAX_PEAK	1500
#define MIN_PEAK	-1500

void loopBack();
void recordLoop();
float calculateOffset();
void correctOffset(uint32_t count, short value, short *cptr);
void adjustPot();

#endif /* INC_LOOP_H_ */

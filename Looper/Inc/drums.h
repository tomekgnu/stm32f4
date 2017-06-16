#ifndef __DRUMS_H
#define __DRUMS_H

#define SNARE_SAMPLES		(7550 /2)
#define KICK_SAMPLES  		(4066 / 2)
#define TOM_HI_SAMPLES 		(6064 / 2)
#define TOM_LO_SAMPLES		(6014 / 2)
#define CRASH_SAMPLES		(9186 / 2)
#define HIHAT_SAMPLES		(1518 / 2)

struct drumStruct{
	uint8_t kick : 1;
	uint8_t snare : 1;
	uint8_t crash : 1;
	uint8_t tomlo : 1;
	uint8_t tomhi : 1;
	uint8_t hihat : 1;
};

void ReadDrumSamples(void);

#endif

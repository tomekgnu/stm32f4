#ifndef __DRUMS_H
#define __DRUMS_H

#include "stm32f429xx.h"
#include "ff.h"

typedef enum{DRUMS_STOPPED,DRUMS_STARTED,DRUMS_READY,DRUMS_PAUSED}DrumFunction;

typedef struct{
	uint32_t beats;	// number of beats
	uint32_t division; // number of smaller parts in single beat
	uint32_t beattime;	// beat time e.g. 60..120
	uint32_t repeat;
}PatternBeats;

typedef struct{
	uint32_t subbeats;
	uint32_t barDuration;
	uint32_t remainder;
	uint32_t subBeatDuration;

}PatternTimes;

typedef struct{
	uint32_t sram_position;		// pattern offset in sram in bytes
	uint32_t audio_position;	// audio sample offset
	uint8_t ch1rec:1;
	uint8_t ch2rec:1;
}PatternData;

void readDrums(uint32_t *numOfPatterns,uint32_t *numOfBytes,uint32_t *maxResolution);
void drumLoop();
void drumAudioSync();
void resetDrums();
void stopDrums();
void playDrums();
void midiDrumHandler();
void initDrumBeats();
void setBarBeats(uint16_t bar,uint16_t beats);
void setPatternTime(__IO PatternBeats *p,__IO PatternTimes *t);
void updatePatternTime(__IO PatternBeats *p,__IO PatternTimes *t);

#define BEAT_MILLIS(x)	(60000 / (x))	// convert beat time signature (e.g. 60,120) to milliseconds
#define NUM_DRUM_TRACKS	4
#define NUM_ALL_TRACKS		5
#define DRUM_TIM_HDR_SIZE	16	// beats,division,beattime,repeat: each 4 bytes
#define DRUM_INSTR			5	// 4 drums + bass
#define L_HAND	0
#define R_HAND	1
#define L_FOOT	2
#define R_FOOT	3
#define BASS	4

// index constants for drum file header
#define HEADER_NUM_BYTES		0
#define HEADER_NUM_PATTS		1
#define HEADER_MAX_BEATS		2
#define MAX_PATTERNS			200
#define MAX_SUBBEATS			48


extern PatternData pattern_audio_map[MAX_PATTERNS + 1];
extern __IO PatternTimes *timptr;
extern uint32_t drumBeatIndex;
extern uint8_t key_to_drum[];

#endif

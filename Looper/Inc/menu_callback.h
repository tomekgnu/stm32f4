#ifndef __MENU_CALLBACK_H__
#define  __MENU_CALLBACK_H__

#include "ff.h"
#include "spiffs.h"

void audio_only(void);
void audio_rhythm(void);
void drums_only(void);
void audio_drums(void);
void download_rhythm(void);
void select_bars(void);
void select_channel(TM_KEYPAD_Button_t key);
void select_loops(void);
void saveLoopSD(uint32_t n,char *filename);
s32_t saveLoopSF3(uint32_t n,char *filename);
void readFromSD(uint32_t n,char *filename);
void readFromSF3(uint32_t n,char *filename);
void saveAllLoops();
void get_string(char *outstr);
BOOL get_file_sd(char *outstr);
void get_file_sf3(char *outstr);

typedef struct FileEntry{
	char filename[33];
	uint16_t number;
	uint16_t list_pos;
	struct FileEntry *prev;
	struct FileEntry *next;
} FileEntry;
#endif

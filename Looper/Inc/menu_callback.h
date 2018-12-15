#ifndef __MENU_CALLBACK_H__
#define  __MENU_CALLBACK_H__

void audio_only(void);
void audio_rhythm(void);
void drums_only(void);
void audio_drums(void);
void print_letters(void);
void download_rhythm(void);
void select_bars(void);
void select_channel(TM_KEYPAD_Button_t key);
void select_loops(void);
void saveSingleLoop(uint32_t n);
void saveAllLoops();
void get_string(char *outstr);
#endif

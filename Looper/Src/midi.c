#include "main.h"
#include "midi.h"
#include "usart.h"
#include "tm_stm32f4_keypad.h"

__IO uint16_t midiDrumClock = 0;
__IO uint16_t midiMetronomeClock = 0;
uint32_t midiDrumPointers[4];
__IO uint32_t midiMetronomePointer = 0;

uint16_t beats_per_minute	= 60;			// 1s = 1 quarter
uint16_t millis_per_beat = 60000 / 60;

uint8_t midiEvents[MIDI_QUEUE];
uint16_t midiTimes[MIDI_QUEUE];

uint16_t drumTracks[4][2][16];

__IO uint8_t midiRecording = 0;
__IO uint8_t midiPlayback = 0;
__IO uint8_t midiMetronome = 0;

uint8_t key_to_drum[10] = {
		Acoustic_Bass_Drum,
		Acoustic_Snare,
		Low_Floor_Tom,
		Low_Mid_Tom,
		High_Tom,
		Hi_Mid_Tom,
		Open_Hi_Hat,
		Closed_Hi_Hat,
		Crash_Cymbal_1,
		Ride_Cymbal_2
};

void midiHandler(){
	if(midiDrumClock < 4000)
		midiDrumClock++;
	else{
			midiDrumClock = 0;
			midiDrumPointers[L_HAND] = 0;
			midiDrumPointers[R_HAND] = 0;
			midiDrumPointers[L_FOOT] = 0;
			midiDrumPointers[R_FOOT] = 0;
	}
}

void playPercussionEvent(){
	//if(midiEvents[midiDrumPointer] != No_Event)
		//playPercussion(NOTEON,midiEvents[midiDrumPointer]);
}

void recordPercussionEvent(TM_KEYPAD_Button_t e){
	//midiEvents[midiDrumPointer] = key_to_drum[e];
	//midiTimes[midiDrumPointer] = midiDrumClock;
}
void setupMidi(){
	//Reset the VS1053
	  VS1053_LOW();
	  HAL_Delay(100);
	  VS1053_HIGH();
	  HAL_Delay(100);
	  talkMIDI(0xB0, 0x07, 120); //0xB0 is channel message, set channel volume to near max (127)

}
void noteOn(byte channel, byte note, byte attack_velocity) {
  talkMIDI( (0x90 | channel), note, attack_velocity);
}

//Send a MIDI note-off message.  Like releasing a piano key
void noteOff(byte channel, byte note, byte release_velocity) {
  talkMIDI( (0x80 | channel), note, release_velocity);
}

//Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127
void talkMIDI(byte cmd, byte data1, byte data2) {
	HAL_UART_Transmit(&huart1,&cmd,1,100);
	HAL_UART_Transmit(&huart1,&data1,1,100);

  //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes
  //(sort of: http://253.ccarh.org/handout/midiprotocol/)
  if( (cmd & 0xF0) <= 0xB0)
	  HAL_UART_Transmit(&huart1,&data2,1,100);

}

void playPercussion(byte onoff,byte instrument){
	if(onoff == NOTEON)
		noteOn(9, instrument, 60);
	else
		//Turn off the note with a given off/release velocity
		noteOff(9, instrument, 60);
}

void playDrums(){
	if(drumTracks[R_FOOT][DRUM][midiDrumPointers[R_FOOT]] != 0 && midiDrumClock >= drumTracks[R_FOOT][TIME][midiDrumPointers[R_FOOT]])
		playPercussion(NOTEON,drumTracks[R_FOOT][DRUM][midiDrumPointers[R_FOOT]++]);

	if(drumTracks[L_FOOT][DRUM][midiDrumPointers[L_FOOT]] != 0 && midiDrumClock >= drumTracks[L_FOOT][TIME][midiDrumPointers[L_FOOT]])
		playPercussion(NOTEON,drumTracks[L_FOOT][DRUM][midiDrumPointers[L_FOOT]++]);

	if(drumTracks[R_HAND][DRUM][midiDrumPointers[R_HAND]] != 0 && midiDrumClock >= drumTracks[R_HAND][TIME][midiDrumPointers[R_HAND]])
		playPercussion(NOTEON,drumTracks[R_HAND][DRUM][midiDrumPointers[R_HAND]++]);

	if(drumTracks[L_HAND][DRUM][midiDrumPointers[L_HAND]] != 0 && midiDrumClock >= drumTracks[L_HAND][TIME][midiDrumPointers[L_HAND]])
		playPercussion(NOTEON,drumTracks[L_HAND][DRUM][midiDrumPointers[L_HAND]++]);
}
///* MIDI Looper - Mike Cook
// * -----------------
// * records MIDI input then plays it out continuously
//*/
//
//#define bLength 100
//
////variables setup
//  BOOL noteDown = LOW;
//  const int recordLed = 7, replayLed =5; // onboard LED
//  byte channel = 0; // MIDI channel = the value in 'channel' + 1
// int state=0;      // state machine variable 0 = command waiting : 1 = note waiting : 2 = velocity waiting
// int place;
// // buffer for delayed notes
// unsigned long time [bLength];
// byte storeAction[bLength];
// byte storeNote[bLength];
// byte storeVel[bLength];
// unsigned long startTime=0, endTime=0, bufferTime=0;
// BOOL recording = FALSE, playback = FALSE, empty = FALSE;
// const byte recordPin = 12, stopPin = 10;
//
//
////loop: wait for serial data, and interpret the message
//void loop () {
//  checkIn(); // see if anything has arrived at the input
//  if(state == 0) checkOut(); // see if we need to send anything out
//  getControls(); // read switches
//  }
//
//void getControls(){
//  static long debounce = 0;
//  static long debounceTime = 25;
//  boolean rec = !digitalRead(recordPin);
//  if(!recording && rec && digitalRead(stopPin) && millis() > debounce){
//    recording = true;
//    debounce = millis() + debounceTime;
//    if(playback){
//       playback = false;
//       wipeBuffer();
//    }
//    startTime = millis();
//    digitalWrite(recordLed, HIGH);
//  }
//  if(recording && !rec && millis() > debounce){ //debounce
//     recording = false;
//     playback = true;
//    debounce = millis() + debounceTime;
//    time[place] = 0; // mark buffer end
//     bufferTime = millis() - startTime;
//     digitalWrite(recordLed, LOW);
//     prepBuffer();
//   }
//   if(!digitalRead(stopPin)){
//      recording = false;
//      playback = false;
//      wipeBuffer();
//    }
//}
//
//void wipeBuffer(){
//  for(int i =0; i<bLength; i++){
//    time[i] = 0L;
//  }
//  place = 0;
//}
//
//void prepBuffer(){ // set buffer for next time it should play
//  int i=0;
//  while(time[i] != 0){
//    time[i] += bufferTime;
//    i++;
//  }
//  place = 0;
//}
//
//void checkIn(){
//  static byte note = 60;
//  if (Serial.available() > 0) {
//    // read the incoming byte:
//    byte incomingByte = Serial.read();
//    Serial.write(incomingByte);
//
//  switch (state){
//      case 0:
//    // look for as status-byte, our channel, note on
//         if (incomingByte == ( 0x90 | channel)){  // read only one channel
//            noteDown = HIGH;
//            state=1;
//         }
//    // look for as status-byte, our channel, note off
//         if (incomingByte == (0x80 | channel)){   // read only one channel
//            noteDown = LOW;
//            state=1;
//        }
//         // look for any after touch, or program message
//         if ((incomingByte & 0xE0) == 0xC0){
//            state=4;  // just wait for the data
//         }
//         // look for any control or polyphonic after touch
//         if ((incomingByte & 0xE0) == 0xA0){
//            state=3;  // just wait for two bytes of data
//         }
//         // look for any pitch wheel or Channel Mode data
//         if ((incomingByte & 0xF0) == 0xA0 || (incomingByte & 0xF0) == 0xB0){
//            state=3;  // just wait for two bytes of data
//         }
//         break;
//       case 1:
//       // get the note to play or stop
//          if(incomingByte < 128) {
//             note=incomingByte;
//             state=2;
//          }
//          else {
//            state = 0;  // reset state machine as this should be a note number
//          }
//         break;
//       case 2:
//       // get the velocity
//          if(incomingByte < 128) {
//              doNote(note, incomingByte, noteDown); // do something with the note on message
//          }
//          state = 0;  // reset state machine to start
//         break;
//       case 3: // first of two bytes to discard
//       state = 4; // next byte to discard
//         break;
//       case 4: // data to discard
//       state = 0; // reset state machine
//     }
//  }
//}
//
//void doNote(byte note, byte velocity, int down){
//  // if velocity = 0 on a 'Note ON' command, treat it as a note off
//  if ((down == HIGH) && (velocity == 0)){
//      down = LOW;
//  }
//  // do something with the note message
//
//  if(down == LOW) { // record the note off in the buffer
//    bufferSave(0x80,note,velocity);
//  }
//  else {
//    bufferSave(0x90,note,velocity);
//  }
// }
//
//void noteSend(byte cmd, byte data1, byte data2) {
//  cmd = cmd | channel;
//
//  Serial.write(cmd);
//  Serial.write(data1);
//  Serial.write(data2);
//}
//
//void bufferSave(byte action, byte note, byte vel){
//  // place note in next position in buffer
//  if(recording){
//     time[place] = millis();
//     storeAction[place] = action;
//     storeNote[place] = note;
//     storeVel[place] = vel;
//     place++; // for next time
//       if(place >= bLength) { // come to the end of the buffer
//       // make it start recording again because record key is still held
//       recording = false;
//       playback = true;
//       }
//  }
//}
//
//void checkOut(){ // see if we need to send anything from the buffer
//  if(playback && time[0] != 0L) {
//    digitalWrite(replayLed,HIGH);
//    if(millis() > time[place]){
//     noteSend(storeAction[place], storeNote[place], storeVel[place]);
//     time[place] += bufferTime; // set buffer entry for next play
//     place++; // point at next play
//     if(time[place] ==0) place = 0; // wrap pointer round
//    }
//    digitalWrite(replayLed,LOW);
//  }
//}

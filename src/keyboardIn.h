//
//  keyboardIn.h
//  MIDI Keyboard Input Implementation
//  Created by Heewon Oh on 13/11/2017
//  Humelo
//

#ifndef keyboardIn_h
#define keyboardIn_h

#include "midiUtil.h"
#include "ofxMidi.h" //ofxMidi case
#include <queue>
#include <vector>
#include <string>
using namespace std;

enum NOTE_PRIORITY{HIGHEST,LOWEST,LATEST};
enum NOTE_STATUS{NOTE_ON, NOTE_OFF};

class midiUtil;
class qtKey;
class keyboardIn;

class qtKey : public midiUtil{
    //qwerty MIDI keyboard input
private:
    queue<int> rawInput;
    bool print;
    string s;
    const int keycode[15]={97,119,115,101,100,102,116,103,121,104,117,106,107 ,111,108}; //qwerty midi keyboard
    const int funcKey[4] = {122,120,99,118}; //ascii for function input
    int currentOctave = 3; //set current octave to 3oct (C3 - D4)
//    int rawInput;
public:
    qtKey(){print = true;}
    void getInputs(int key);//get all keyboard inputs
    void changeOct(int key);
    bool isPressed(bool key);
    int keyToNote();//translate keycaps to note number
    NOTE_STATUS ns;
};

class keyboardIn : public midiUtil{
private:
    const int maxVoice = 16;
    deque<Note> rawInput;
    deque<Note> keys;
    //double freqToSynth [108]; not implemented yet
    int voice; //voices per note
    
public:
    midiUtil util;
    keyboardIn(){
    };
    ~keyboardIn(){
    };
    void receiveKey(ofxMidiMessage& msg);
    double getFreq(Note &n);
    bool isPressed(Note n){//copied from midiUtil.h
        
        /*
         
         rtMidi gives two enum based on raw midi status byte 0x90(NOTE_ON), 0x80(NOTE_OFF) from Midi keyboard.
         but some don’t give enum.
         check more about midi stat enumeration : midi.org
         */
        //this only checks velocity.
        if(n.velocity > 0){ //note on only gets velocity.
            return true;//note on only gets velocity.
        }
        else{
            //if velocity is zero..
            return false;
        }
        //so now it's evenly same as any usual midi device.
    }
};
#endif /* keyboardIn_h */

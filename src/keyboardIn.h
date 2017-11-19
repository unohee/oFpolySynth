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
class midiUtil; //forward declaration
class qtKey;

class qtKey{
    //qwerty MIDI keyboard input
private:
    bool isPressed;
    const char keys[15] = {'a','w','s','e','d',
        'f','t','g','y','h',
        'u','j','k','o','l'}; //get keycaps for piano playing
public:
    void getInputs(int key);//get all keyboard inputs
    int keyToNote(int key);//translate keycaps to note number

};

class keyboardIn{
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

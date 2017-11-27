//
//  keyboardIn.cpp
//  MIDI Keyboard Input Implementation
//  Created by Heewon Oh on 13/11/2017
//  Humelo
//

#include "keyboardIn.h"
void qtKey::getInputs(int key){
    //octave, velocity change functions
    switch(getIndex(key, funcKey)){
        case 0 :
            if(currentOctave > -2)currentOctave --;
            cout<<"Octave Down"<<currentOctave<<endl;
            break;
        case 1 :
            if(currentOctave < 7)currentOctave ++;
            cout<<"Octave Up"<<currentOctave<<endl;
            break;
        case 2 :
            break;
        case 3 :
            break;
        default :
            break;
    }
    //change keycode to midi note
    makeNote(key);
}
int qtKey::keyToNote(){
    int noteOut = rawInput.back();
    return noteOut;
}
bool qtKey::isPressed(bool key){
    if(key){
        
    }else{
        
    }
}
//--------------------------------------------------------------
void keyboardIn::receiveKey(ofxMidiMessage& msg){
    //note that rtmidi test version (with ofxmidi and synth wrapped)
    //soon will be transfer to JUCE
    Note n;
    n.note = msg.pitch;
    n.velocity = msg.velocity;
    
    if(isPressed(n)){ //check whether a key is pressed
        voice ++; //increments the amount of voice (notes) in each input
        
        rawInput.push_front(n);
        //always first element of raw midi input goes to the last of deque
        keys.push_back(rawInput.front());
    }else{
        rawInput.pop_front(); //First Out.
        while(voice > 0){
            voice --;
            keys.pop_front();
        }
    }
    //printout deque for output
    for(std::deque<Note>::iterator it = keys.begin() ; it != keys.end(); ++it){
        cout<<util.notes[(*it).note-12]<<' ';
        //put triggers and Notes data for synth then... boom!
        
        
    }
    cout<<'\n';
    cout<<"Input Voices :"<<keys.size()<<endl;
}

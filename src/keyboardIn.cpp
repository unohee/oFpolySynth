//
//  keyboardIn.cpp
//  MIDI Keyboard Input Implementation
//  Created by Heewon Oh on 13/11/2017
//  Humelo
//

#include "keyboardIn.h"

void qtKey::getInputs(int key){
    if(key == funcKey[0]){
        //decrease octave
        if(currentOctave >= -2 || currentOctave <= 7)
            currentOctave --;
        if(print)cout<<"[Octave Range Changed:"<<currentOctave<<"]"<<endl;
    }else if(key == funcKey[1]){
        //increase octave
        if(currentOctave >= -2 || currentOctave <= 7) currentOctave ++;
        if(print)cout<<"[Octave Range Changed:"<<currentOctave<<"]"<<endl;
    }else if(key == funcKey[2]){
        //decrease velocity
    }else if(key == funcKey[3]){
        //increase velocity
    }
    //change keycode to midi notCe
    for(int i=0;i < 15;i++){
        if(keycode[i] == key){
            int thisNote = (i+((currentOctave+2)*12));
            rawInput.push(thisNote);
            if(rawInput.size()>16){
                rawInput.pop();
                cout<<"Last element is cleared"<<endl;
            }
            if(print){
                s = s + "[Midi Note: " + to_string(rawInput.front()) +", Musical Note: "+notes[thisNote]+"]"+'\n';
                cout<<s;
                s= "";
            }
        }
    }
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

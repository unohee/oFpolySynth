//
//  keyboardIn.cpp
//  MIDI Keyboard Input Implementation
//  Created by Heewon Oh on 13/11/2017
//  Humelo
//

#include "keyboardIn.h"

void qtKey::getInputs(int key){
    
}
int qtKey::keyToNote(int Key){
    for(int i=0;i < 19;i++){
        if(keycodeL[i] == Key || keycodeU[i] == Key){
            cout<<keys[i]<<","<<Key<<","<<notes[i]<<endl;
            return Key;
        }
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

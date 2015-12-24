    #pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxMaxim.h"
#include "ofxGui.h"


class ofApp : public ofBaseApp, public ofxMidiListener {
    
public:
    
    void setup();
    void update();
    void draw();
    void exit();
    
    //ofxMidi------
    void newMidiMessage(ofxMidiMessage& eventArgs);
    
    stringstream text;
    
    double outputTwoChannels[2];
    enum NOTE_STATUS {KEY_ON, KEY_OFF};
    
    
private:
    
    ofxMidiIn midiIn;
    ofxMidiMessage midiMessage;
    ofxPanel gui;
    
    //parameters for GUI
    
    ofParameter<int> env1atk; //attack
    ofParameter<int> env1dec; //decay
    ofParameter<float> env1sus; //sustain
    ofParameter<float> env1rel; //release
    
    ofParameter<int> env2atk; //attack
    ofParameter<int> env2dec; //decay
    ofParameter<float> env2sus; //sustain
    ofParameter<float> env2rel; //release

    //Maximilian-------
    double ref; //reference frequency
    
    void audioIn(float * input, int bufferSize, int nChannels);
    void audioOut(float * output, int bufferSize, int nChannels);
    
    double VCOout[16], VCFout[16], ADSRout[16], ampOut;

    double OSCin[16];

    
    //put maximilian declaration under.
    maxiOsc VCO[16], subOsc[16], phasor[16], LFO1[16];
    maxiFilter VCF[16], //lopass with resonance.
    ramp[16],// filter to create ramp-generator.
    porta[16]; //portamento
    convert mtof;
    
    maxiMix outputTwoChannel;
    maxiEnv ADSR1[16], ADSR2[16];
    
protected:
    
    bool note_on;
    bool note_off;
    bool isPressed[108];
    int note[16];
    int voice;//amount to check the size of incomming MIDI message

};

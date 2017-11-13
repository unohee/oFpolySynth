//Heewon Oh, Goldsmiths, University of London

#include "ofApp.h"

//gets samplerate and buffersize as const.
#define SRATE 44100 //SAMPLE RATE
#define BUFFER_SIZE 512 // BUFFERSIZE

//--------------------------------------------------------------
void ofApp::setup() {
    
    //NoteEvent
    
    //ofxMidi------------------
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // Codes below are referenced from ofxMidi MIDI input example https://github.com/danomatika/ofxMidi/tree/master/midiInputExample/src
    
    // print input ports to console
    midiIn.listPorts(); // via instance
    midiIn.openPort(0); //call MIDI interface by name. as default. first midi interface will be connected. check your midi interface list in commandline below.
    
    // add ofApp as a listener
    midiIn.addListener(this);
    // print received messages to the console
    midiIn.setVerbose(false);
    
    //END Reference----
    
    
    //GUI

    gui.setup("Envelope"); //Name it
    gui.setPosition(10, ofGetWindowHeight()/2); //set position at middle left
    
    //ENV1
    gui.add(env1atk.set("Filter A", 30, 1, 2000));
    gui.add(env1dec.set("Filter D", 200, 1, 2000));
    gui.add(env1sus.set("Filter S", 1, 0.1f, 1.0));
    gui.add(env1rel.set("Filter R", 5000, 1, 5000));
    
    //ENV2
    gui.add(env2atk.set("AMP A", 1, 1, 2000));
    gui.add(env2dec.set("AMP D", 200, 1, 2000));
    gui.add(env2sus.set("AMP S", 1, 0.1f, 1.0));
    gui.add(env2rel.set("AMP R", 350, 1, 5000));
    
    
    ref = 440.0; // in order to use ote conversion to frequency function.  we need our reference frequency. and it is 440hz.
    
    //load maximilian object here first!
    
    //Maximilian, Audio variables inits.
    
    
    //maxiENV setup.
    for(int i = 0; i < 16; i++){
        OSCin[i]=440;
        //Filter Envelope
        
        ADSR1[i].setAttack(env1atk);
        ADSR1[i].setDecay(env1dec);
        ADSR1[i].setSustain(env1sus);
        ADSR1[i].setRelease(env1rel);
        
        //AMP Envelope
        ADSR2[i].setAttack(env2atk);
        ADSR2[i].setDecay(env2dec);
        ADSR2[i].setSustain(env2rel);
        ADSR2[i].setRelease(env2sus);
    }

    
    
    //any audio code has to be before ofSoundStreamSetup. otherwise it's crashed.
    ofSoundStreamSetup(2, 0, this, SRATE, BUFFER_SIZE, 4);
    
    
}
//--------------------------------------------------------------
double midiTofreq (int m){
    //MIDI to frequency Conversion function.
    double mtof = (440.0 / 32.0) * pow(2,(m - 9.0) / 12.0); // modified equation from this C code http://subsynth.sourceforge.net/midinote2freq.html
    
    return mtof; //return frequency.
}
//--------------------------------------------------------------
void ofApp::update() {
    
}
//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(0);
    ofBackground(255);
    
    //Draw GUI
    gui.draw();
    
    //Simple Graphics for MIDI Testing - ofxMidi example referenced.
    
    

    // draw the last recieved message contents to the screen
    text << "Received: " << ofxMidiMessage::getStatusString(midiMessage.status);
    ofDrawBitmapString(text.str(), 20, 20);
    text.str(""); // clear
    
    text << "channel: " << midiMessage.channel;
    ofDrawBitmapString(text.str(), 20, 34);
    text.str(""); // clear
    
    text << "pitch: " << midiMessage.pitch;
    ofDrawBitmapString(text.str(), 20, 48);
    text.str(""); // clear
    ofRect(20, 58, ofMap(midiMessage.pitch, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "velocity: " << midiMessage.velocity;
    ofDrawBitmapString(text.str(), 20, 96);
    text.str(""); // clear
    ofRect(20, 105, ofMap(midiMessage.velocity, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "control: " << midiMessage.control;
    ofDrawBitmapString(text.str(), 20, 144);
    text.str(""); // clear
    ofRect(20, 154, ofMap(midiMessage.control, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "value: " << midiMessage.value;
    ofDrawBitmapString(text.str(), 20, 192);
    text.str(""); // clear
    if(midiMessage.status == MIDI_PITCH_BEND) {
        ofRect(20, 202, ofMap(midiMessage.value, 0, MIDI_MAX_BEND, 0, ofGetWidth()-40), 20);
    }
    else {
        ofRect(20, 202, ofMap(midiMessage.value, 0, 127, 0, ofGetWidth()-40), 20);
    }
    
    text << "delta: " << midiMessage.deltatime;
    ofDrawBitmapString(text.str(), 20, 240);
    text.str(""); // clear
    
}
//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
    
    //RtMidi Callback.
    // make a copy of the latest message
    midiMessage = msg;
    //Polyphony Method (Simplified Version. note that it doesn't have note stealing.)
    //based on Maximilian polyphony example's structure. https://github.com/micknoise/Maximilian/blob/master/maximilian_examples/15.polysynth.cpp
    
    //*check how many keys are pressed
    keyIn.receiveKey(msg);
    //and do iterations which keys were pressed.
    for(int i = 1; i <108; i++){ //the range of midi note is 1-108
            if(isPressed[msg.pitch]){//NOTE_ON
                note[voice] = msg.pitch; //assign note value in each voice arrays.
                
                OSCin[voice] = midiTofreq(note[voice]); //send mtof pitch to VCOs.
//                OSCin[voice] = util.mtof(note[voice]);
                
                //and Trigger 2 Env.
                ADSR1[voice].trigger = 1;
                ADSR2[voice].trigger = 1;
            }
            if(!isPressed[msg.pitch]){//NOTE_OFF
                for(int i=0; i<16; i++){
                    
                    //stops Envelopes in Note OFF situation (0 velocity)
                    ADSR1[i].trigger = 0;
                    ADSR2[i].trigger = 0;
                }
            }
    }
    
    //printout
    if(note_on){
        for(int i = 0; i < voice; i++){
                cout<< "Note On : "<< note[i] << endl;
        }
        cout<< voice << " Voice"<< endl;
    }
    if(note_off){
        cout<< "Note Off : "<< msg.pitch << endl;
    }
    
}

//--------------------------------------------------------------
void ofApp::audioIn(float * output, int bufferSize, int nChannels){
    
    
}
//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels){

    
    for(int i = 0; i < bufferSize; i++){
        
        for(int i = 0; i < 16; i++){

            //these envelope parameters will be updated as GUI modulates values..
            
            //Filter Envelope
            ADSR1[i].setAttack(env1atk);
            ADSR1[i].setDecay(env1dec);
            ADSR1[i].setSustain(env1sus);
            ADSR1[i].setRelease(env1rel);
            
            //AMP Envelope
            ADSR2[i].setAttack(env2atk);
            ADSR2[i].setDecay(env2dec);
            ADSR2[i].setSustain(env2rel);
            ADSR2[i].setRelease(env2sus);
            

            VCOout[i] = VCO[i].sawn(porta[i].lopass(OSCin[i], 0.001)) + subOsc[i].sawn(OSCin[i]/2);
            //use lopass filter in frequency input for adding glide(portamento) feel.
            
            VCFout[i] = VCF[i].lores(VCOout[i], 500*ADSRout[i]*10, 2);
            
            ADSRout[i] = ADSR1[i].adsr(1.0,ADSR1[i].trigger); //ADSR1 controls filter.
            
            ampOut += VCFout[i] * ADSR2[i].adsr(1.0, ADSR2[i].trigger) / (16 * bufferSize) * 50; // divide it same size of voices. otherwise it burst.

        }
        
        outputTwoChannel.stereo(ampOut, outputTwoChannels, 0.5); //simple way to map mono sounds to stereo channel.
        output[i*nChannels] = outputTwoChannels[0];
        output[i*nChannels + 1] = outputTwoChannels[1];
        
        
    }
}


//--------------------------------------------------------------
void ofApp::exit() {
    // clean up
    midiIn.closePort();
    midiIn.removeListener(this);
    ofSoundStreamClose();
}




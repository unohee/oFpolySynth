

/*Heewon Oh, Goldsmiths, University of London
*http://gitlab.doc.gold.ac.uk/heewon-oh/ofMaxim-polysynth
*/
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
    

//ASSIGNING NEW ENUMS FOR M-Audio Keystation 32----------------------------------

    NOTE_STATUS KEY;
    //Create new Enumeration to seperate two states: NOTE_ON but velocity is bigger than 0 and else.
    //ofxMidi gives two enum based on raw midi status byte 0x90(NOTE_ON), 0x80(NOTE_OFF) from Midi keyboard. but mine doesnÕt do same thing.
    
    /*
     *MIDI.org says that
     
     ÒIn MIDI systems, the activation of a particular note and the release of the same note are considered as two separate events. When a key is pressed on a MIDI keyboard instrument or MIDI keyboard controller, the keyboard sends a Note On message on the MIDI OUT port. The keyboard may be set to transmit on any one of the sixteen logical MIDI channels, and the status byte for the Note On message will indicate the selected Channel number. The Note On status byte is followed by two data bytes, which specify key number (indicating which key was pressed) and velocity (how hard the key was pressed).Ó
     */
    
    
    
    note_on = false;
    note_off = false;

//Assigning new Enums based on Velocity------------------------------------------
    if(msg.velocity > 0){ //note on only gets velocity.
        

        KEY = KEY_ON;//note on only gets velocity.
        
        //or check ENUM directly from ofxMidi library.
        if(MIDI_NOTE_ON) note_on = TRUE;
        
    }
    else{
        //if velocity is zero..
        
        //Case 1. if ofxMidi doesn't send NOTEOFF enum. we use this.
        KEY = KEY_OFF;//then I say it is NOTE OFF.
        
        //Case 2. If ofxMidi captures NOTEOFF from MIDI Device..
        if(MIDI_NOTE_OFF) note_off = TRUE;
    }
    
    if (KEY == KEY_ON)  note_on = TRUE;
    if (KEY == KEY_OFF) note_off = TRUE;
    //so now it's evenly same as usual midi device.
    
//--------------------------------------------------------------------------------
    
    //Polyphony Method (Simplified Version. note that it doesn't have note stealing.)
    //based on Maximilian polyphony example's structure. https://github.com/micknoise/Maximilian/blob/master/maximilian_examples/15.polysynth.cpp
    
    //*check how many keys are pressed
    if (note_on){
        //then number of input note increments. (if it's 2 note. voice will be 2);
        voice++;
    }
    if (note_off){
        //erase note.
        voice--;
    }
    //** and do iterations which keys were pressed.
    for(int i = 1; i <108; i++){ //the range of midi note is 1-108
        if(note_on){
            //Keypressed.
            isPressed[msg.pitch] = true;
        }
        if(note_off){
            //Keyreleased.
            isPressed[msg.pitch] = false;
        }

            if(isPressed[msg.pitch]){//NOTE_ON
                if(voice==16){
                    voice = 0;
                }
                note[voice] = msg.pitch; //assign note value in each voice arrays.
                
                OSCin[voice] = midiTofreq(note[voice]); //send mtof pitch to VCOs.
                
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
}




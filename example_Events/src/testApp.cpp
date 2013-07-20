// Synesthesia Study
// by Morgan Packad
// morganpackard.com

#include "testApp.h"
#include <functional>


const int NUM_VOICES = 5;

//--------------------------------------------------------------
void testApp::setup(){

  ofHideCursor();
  
  ofSetFrameRate(60);
  ofSetVerticalSync(true);
  ofSetFullscreen(true);
  
  ///////////////////////
  // Tonic Synth setup
  ///////////////////////
  
  Generator allVoices;
  
  Generator noise = PinkNoise();
  
  vector<float> scale;
  scale.push_back(0);
  scale.push_back(2);
  scale.push_back(4);
  scale.push_back(7);
  scale.push_back(11);
  
  for(int i = 0; i < NUM_VOICES; i++){
    
    ////////////////////////////////////////////
    // Create a UI representation of this sound
    ////////////////////////////////////////////
    
    float xPos = ofGetScreenWidth() * i / NUM_VOICES;
    FlashingRectangle* flasher = new FlashingRectangle(xPos, ofGetScreenWidth() / NUM_VOICES);
    flashingRectangles.push_back( flasher );
  
    ////////////////////////////////////////////
    // Create the synthesis network
    ////////////////////////////////////////////
    
    Generator voice;
    
    // Send a trigger message immediately, don't wait for the first beat to happen with the metrenome 
    ControlTrigger initialTrigger;
    initialTrigger.trigger();
    ControlGenerator resetTrigger = initialTrigger + ControlMetro().bpm(ofRandom(10, 15));
    
    ControlGenerator noiseTrigger = ControlMetro().bpm( ControlRandom().min(50).max(200).trigger( resetTrigger ));
    
    ControlGenerator pulseLen = ControlRandom().min(0.1).max(0.5).trigger(resetTrigger);
    
    ControlGenerator pulse =  ControlPulse().length(pulseLen).input(noiseTrigger);
    
    Generator env =  ADSR(0.01,0,0.5,0.01).decay(pulseLen * 0.5).trigger( pulse );
    
    voice = noise * env;
    
    ControlGenerator highPassAmount = ControlRandom().min(0).max(1).trigger(resetTrigger);
    
    ControlSnapToScale scaleSnapper = ControlSnapToScale().setScale(scale);
    scaleSnapper.input(40 + highPassAmount * 50);
    ControlGenerator filterFreq = ControlMidiToFreq().input(scaleSnapper);
    ControlGenerator q = ControlRandom().min(0.5).max(1).trigger(resetTrigger);
    q = q * q * 50;
    
    voice = HPF24().cutoff(filterFreq).input(voice).Q(q);
    
    float pan = (2 * (float)i / NUM_VOICES) - 1;
    voice =  MonoToStereoPanner().pan(pan).input(voice) * (15.0 / NUM_VOICES);
    
    allVoices = allVoices + voice;
    
    ///////////////////////////////
    // Create ofEvents based on
    // ControlGenerators, hook the
    // events up to the UI object
    ////////////////////////////////
    
    ofEvent<float>* pulseEvent = synth.createOFEvent(pulse);
    ofAddListener(*pulseEvent, flasher, &FlashingRectangle::pulseHappened);
    
    ofEvent<float>* filterFreqChangedEvent = synth.createOFEvent(highPassAmount);
    ofAddListener(*filterFreqChangedEvent, flasher, &FlashingRectangle::filterFreqChanged);
    
  }
  
  Generator allvoicesPlusReverb = Reverb().input(allVoices).density(1).dryLevel(0).wetLevel(0.2).decayTime(1);
  
  // set the synth's final output generator
  synth.setOutputGen( allVoices + allvoicesPlusReverb );
  
  // Important -- audio doesn't work without this
  ofSoundStreamSetup(2, 0, this, 44100, 256, 4);
  
}


//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

  ofClear(0, 0, 0);

  for(vector<FlashingRectangle*>::iterator it = flashingRectangles.begin(); it != flashingRectangles.end(); it++){
    (*it)->draw();
  }
  
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::audioRequested (float * output, int bufferSize, int nChannels){
  synth.fillBufferOfFloats(output, bufferSize, nChannels);
}
#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
  ofSoundStreamSetup(2, 0, this, 44100, 256, 4);
  
  // create a new oscillator which we'll use for the actual audio signal
  SineWave tone = SineWave();
  
  // create a sine wave we'll use for some vibrato
  SineWave vibratoOsc = SineWave();
  vibratoOsc.freq(10);
  
  // you can use the regular arithmatic operators on Generators and their subclasses (SineWave extends Generator)
  float basePitch = 400;
  Generator frequency = basePitch + (vibratoOsc * basePitch * 0.01);
  
  // plug that frequency generator into the frequency slot of the main audio-producing sine wave
  tone.freq(frequency);
  
  // let's also create a tremelo effect
  SineWave tremeloSine = SineWave().freq(1);
  
  // set the synth's final output generator
  synth.setOutputGen( tone * tremeloSine );
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

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
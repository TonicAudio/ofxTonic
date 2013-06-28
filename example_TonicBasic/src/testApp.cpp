#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
  ofSoundStreamSetup(2, 0, this, 44100, 256, 4);
  
  synth.setOutputGen( SineWave().freq(200 + SineWave().freq(0.3) * 20 ) * SineWave().freq(1) );
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
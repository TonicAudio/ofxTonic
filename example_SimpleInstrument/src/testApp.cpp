#include "testApp.h"

#define NUMBER_OF_KEYS 10

//--------------------------------------------------------------
void testApp::setup(){
  ofSoundStreamSetup(2, 0, this, 44100, 256, 4);
  
  /*
   Generators and ControlGenerators both output a steady stream of data. 
   Generators output at the sample rate (in this case, 44100 hertz.
   ControlGenerators output at the control rate, which is much lower than the sample rate.
  */
  
  // create a named parameter on the synth which we can set at runtime
  ControlGenerator midiNote = synth.addParameter("midiNumber");
  
  // convert a midi note to a frequency (plugging that parameter into another object)
  ControlGenerator noteFreq =  ControlMidiToFreq().input(midiNote);
  
  // Here's the actual noise-making object
  Generator tone = SawtoothWave().freq( noteFreq );
  
  // Let's put a filter on the tone
  tone = LPF12().input(tone).Q(10).cutoff((noteFreq * 2) + SineWave().freq(3) * 0.5 * noteFreq);
  
  // It's just a steady tone until we modulate the amplitude with an envelope
  ControlGenerator envelopeTrigger = synth.addParameter("trigger");
  Generator toneWithEnvelope = tone * ADSR().attack(0.01).decay(1.5).sustain(0).release(0).trigger(envelopeTrigger).legato(true);
  
  // let's send the tone through some delay
  Generator toneWithDelay = StereoDelay(0.5, 0.75).input(toneWithEnvelope).wetLevel(0.1).feedback(0.2);
  
  synth.setOutputGen( toneWithDelay );
}


//--------------------------------------------------------------
void testApp::trigger(){
  static int twoOctavePentatonicScale[10] = {0, 2, 4, 7, 9, 12, 14, 16, 19, 21};
  int degreeToTrigger = floor(ofClamp(scaleDegree, 0, 9));
	
  // set a parameter that we created when we defined the synth
  synth.setParameter("midiNumber", 44 + twoOctavePentatonicScale[degreeToTrigger]);
  
  // simply setting the value of a parameter causes that parameter to send a "trigger" message to any
  // using them as triggers
  synth.setParameter("trigger", 1);
}


//--------------------------------------------------------------
void testApp::setScaleDegreeBasedOnMouseX(){
  int newScaleDegree = ofGetMouseX() * NUMBER_OF_KEYS / ofGetWindowWidth();
  if(ofGetMousePressed() && ( newScaleDegree != scaleDegree )){
    scaleDegree = newScaleDegree;
    trigger();
  }else{
    scaleDegree = newScaleDegree;
  }
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
  float keyWidth = ofGetWindowWidth() / NUMBER_OF_KEYS;
  for(int i = 0; i < NUMBER_OF_KEYS; i++){
    if((i == scaleDegree) && ofGetMousePressed() ){
      ofSetColor(255, 151, 0);
    }else{
      int brightness =  100 + (55 * i / NUMBER_OF_KEYS);
      ofSetColor(brightness, brightness, brightness);
    }
    ofRect(keyWidth * i, 0, keyWidth, ofGetWindowHeight());
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
  setScaleDegreeBasedOnMouseX();
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
  setScaleDegreeBasedOnMouseX();
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
  
  trigger();

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
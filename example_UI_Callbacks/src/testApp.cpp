#include "testApp.h"
#include <functional>

#define INT_TO_STRING( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()


const int NUM_VOICES = 5;

//--------------------------------------------------------------
void testApp::setup(){

  ofHideCursor();
  ofSoundStreamSetup(2, 0, this, 44100, 256, 4);
  ofSetFrameRate(60);
  ofSetVerticalSync(true);
  ofSetFullscreen(true);
  
  ///////////////////////
  // Tonic Synth setup
  ///////////////////////
  
  Generator allVoices;
  
  Generator noise = PinkNoise();
  
  for(int i = 0; i < NUM_VOICES; i++){
    
    float xPos = ofGetScreenWidth() * i / NUM_VOICES;
    FlashingRectangle* flasher = new FlashingRectangle();
    flasher->rect = ofRectangle(xPos, 0, ofGetScreenWidth() / NUM_VOICES, ofGetWindowHeight());
    flasher->maxWidth = ofGetWindowWidth() / NUM_VOICES;
    flashingRectangles.push_back( flasher );
  
    ControlGenerator resetTrigger = ControlMetro().bpm(ofRandom(10, 15));
    ControlGenerator noiseTrigger = ControlMetro().bpm( ControlRandom().min(50).max(500).trigger( resetTrigger ));
    
    // Every time noiseTrigger triggers, a message will be sent to all ControlChangeSubscriber
    // objects subscribing.
    string subscriptionName =  "noiseTrigger" + INT_TO_STRING(i);
    synth.publishChanges(noiseTrigger, subscriptionName);
    synth.addControlChangeSubscriber(subscriptionName, &flasher->flasher);
    
    ControlGenerator pulseLen = ControlRandom().min(0.03).max(0.5).trigger(resetTrigger);
    pulseLen = pulseLen * pulseLen;
    
    ControlGenerator pulse =  ControlPulse().length(pulseLen).input(noiseTrigger);
    string pulseSubscriptionName = "plulse" + INT_TO_STRING(i);
    synth.publishChanges(pulse, pulseSubscriptionName);
    synth.addControlChangeSubscriber(pulseSubscriptionName, &flasher->flasher);
    Generator env =  ADSR(0.01,0,0.5,0.01).decay(pulseLen * 0.5).trigger( pulse );
    
    Generator voice = noise * env;
    
    ControlGenerator highPassAmount = ControlRandom().min(0).max(1).trigger(resetTrigger);
    highPassAmount = highPassAmount * highPassAmount;
    string highPassSubscriptionName = "highPassAmount" + INT_TO_STRING(i);
    synth.publishChanges(highPassAmount, highPassSubscriptionName);
    synth.addControlChangeSubscriber(highPassSubscriptionName, &flasher->intensitySetter);
    ControlGenerator filterFreq = 70 + highPassAmount * 5000;
    ControlGenerator q = ControlRandom().min(0).max(1).trigger(resetTrigger);
    q = q * q * 20;
    ControlGenerator filterVolumeMakeup = highPassAmount + 0.3;
    voice = BPF24().cutoff(filterFreq).input(voice).Q(q);
    
    float pan = (2 * (float)i / NUM_VOICES) - 1;
    voice =  MonoToStereoPanner().pan(pan).input(voice);
    
    
    
//    ///////////////////////// sine waves ////////////////////
//    
//    voice = SineWave().freq(filterFreq) * env;
//    voice = MonoToStereoPanner().pan(pan).input(voice);
//    
    
    allVoices = allVoices + voice / NUM_VOICES;
    
    
  }
  
  // set the synth's final output generator
  synth.setOutputGen( allVoices );
  
}


//--------------------------------------------------------------
void testApp::update(){
  // You have to call this method in order for the synth to send the control change messages to its subscribers
  synth.sendControlChangesToSubscribers();
  for(vector<FlashingRectangle*>::iterator it = flashingRectangles.begin(); it != flashingRectangles.end(); it++){
    (*it)->update();
  }
}

//--------------------------------------------------------------
void testApp::draw(){

  ofClear(0, 0, 0);

  for(vector<FlashingRectangle*>::iterator it = flashingRectangles.begin(); it != flashingRectangles.end(); it++){
    ofColor color = (*it)->getColor();
    ofSetColor(color);
    ofRectangle rect = (*it)->rect;
    rect.setHeight(ofGetWindowHeight());
    ofRect(rect);
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
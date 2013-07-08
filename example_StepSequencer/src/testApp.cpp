#include "testApp.h"

#define INT_TO_STRING( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

//--------------------------------------------------------------
void testApp::setup(){

  sliderArea = ofRectangle(0, 0, ofGetWindowWidth() / 2, ofGetWindowHeight());

  ///////////////////////////////
  //      Audio Stuff
  ///////////////////////////////

  // initialize ofSoundStreamSetup 
  ofSoundStreamSetup(2, 0, this, 44100, 256, 4);
  
  const int NUM_STEPS = 8;

  // synth paramters are like instance variables -- they're values you can set later, by
  // cally synth.setParameter()
  ControlGenerator bpm = synth.addParameter("tempo",100).min(50).max(300);
  ControlGenerator transpose = synth.addParameter("transpose", 0).min(-6).max(6);

  // ControlMetro generates a "trigger" message at a given bpm. We multiply it by four because we
  // want four 16th notes for every beat
  ControlGenerator metro = ControlMetro().bpm(4 * bpm);
  
  // ControlStepper increments a value every time it's triggered, and then starts at the beginning again
  // Here, we're using it to move forward in the sequence
  ControlGenerator step = ControlStepper().end(NUM_STEPS).trigger(metro);
  
  // ControlSwitcher holds a list of ControlGenerators, and routes whichever one the inputIndex is pointing
  // to to its output.
  ControlSwitcher pitches = ControlSwitcher().inputIndex(step);
  ControlSwitcher cutoffs = ControlSwitcher().inputIndex(step);
  ControlSwitcher glides = ControlSwitcher().inputIndex(step);
  
  // stick a bunch of random values into the pitch and cutoff lists
  for(int i = 0; i < NUM_STEPS; i++){
    ControlGenerator pitchForThisStep = synth.addParameter("step" + INT_TO_STRING(i) + "Pitch", randomFloat(10, 80)).min(10).max(80);
    pitches.addInput(pitchForThisStep);
    
    ControlGenerator cutoff = synth.addParameter("step" + INT_TO_STRING(i) + "Cutoff", 500).min(30).max(1500);
    cutoffs.addInput(cutoff);
    
    ControlGenerator glide = synth.addParameter("step" + INT_TO_STRING(i) + "Glide", 0).min(0).max(0.1);
    glides.addInput(glide);
    
  }

  // Define a scale according to steps in a 12-note octave. This is a pentatonic scale. Like using
  // just the black keys on a piano
  vector<float> scale;
  scale.push_back(0);
  scale.push_back(2);
  scale.push_back(3);
  scale.push_back(5);
  scale.push_back(7);
  scale.push_back(10);

  // ControlSnapToScale snaps a float value to the nearest scale value, no matter what octave its in
  ControlGenerator midiNote = transpose + ControlSnapToScale().setScale(scale).input(pitches);

  ControlGenerator frequencyInHertz = ControlMidiToFreq().input(midiNote);
  
  // now that we've done all that, we have a frequency signal that's changing 4x per beat
  Generator tone = RectWave().freq( frequencyInHertz.smoothed().length(glides) );
  
  // create an amplitude signal with an ADSR envelope, and scale it down a little so it's not scary loud
  Generator amplitude = ADSR(0.01, 0.1, 0,0).trigger(metro) * 0.3;
  
  // create a filter, and feed the cutoff sequence in to it
  LPF24 filter =  LPF24().cutoff(cutoffs).Q(0.1);
  filter.input(tone * amplitude);
  
  // rout the output of the filter to the synth's main output
  synth.setOutputGen( filter );
  
  // build a slider for each parameter
  vector<ControlParameter> synthParameters = synth.getParameters();
  for(int i = 0; i < synthParameters.size(); i++){
    sliders.push_back(ParameterSlider(synthParameters.at(i)));
  }
  
  
}

///////////// YOU MUST IMPLEMENT THIS METHED OR NO SOUND WILL HAPPEN //////////
void testApp::audioRequested (float * output, int bufferSize, int nChannels){
  synth.fillBufferOfFloats(output, bufferSize, nChannels);
}




//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
  ofClear(100, 100, 100);
  
  // create some sliders to control those parameters
  float sliderHeight = sliderArea.getHeight() / sliders.size();
  for(int i = 0; i< sliders.size(); i++){
    ofPoint sliderStart = ofPoint(sliderArea.getX(), sliderArea.getY() + i * sliderHeight);
    ParameterSlider& slider = sliders.at(i);
    float sliderPosition = (slider.param.getValue() - slider.param.getMin()) / (slider.param.getMax() - slider.param.getMin());
    ofSetColor(50, 50, 50);
    ofRect(sliderStart.x, sliderStart.y, sliderArea.getWidth() * sliderPosition, sliderHeight - 3);
    ofSetColor(255, 255, 255);
    stringstream sliderLabel;
    sliderLabel << slider.param.getDisplayName()
      << ": "
      << slider.param.getValue();
    ofDrawBitmapString(sliderLabel.str(), sliderStart.x + 5, sliderStart.y + 20);
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
  if (sliderArea.inside(x, y)) {
    float amountDown = (y - sliderArea.getY()) / sliderArea.getHeight();
    float amountAcross = (x - sliderArea.getX()) / sliderArea.getWidth();
    printf("amountAcross: %f\n", amountAcross);
    int sliderIndex = sliders.size() * amountDown;
    sliders.at(sliderIndex).param.setNormalizedValue(amountAcross);
  }
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
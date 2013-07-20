//
//  ofxTonicSynth.cpp
//  example_UI_Callbacks
//
//  Created by Morgan Packard on 7/13/13.
//
//

#include "ofxTonicSynth.h"
#include "ofSoundStream.h"

namespace Tonic {

ofxTonicSynth::ofxTonicSynth(){
  Synth::Synth();
  ofAddListener(ofEvents().update, this, &ofxTonicSynth::update);
  ofAddListener(ofEvents().audioRequested, this, &ofxTonicSynth::audioRequested);
}

ofxTonicSynth::~ofxTonicSynth(){
  vector<ofControlChangetSubscriber*>::iterator it = eventSubscribers.begin();
  for(; it < eventSubscribers.end(); it++){
    delete *it;
  }
  ofRemoveListener(ofEvents().update, this, &ofxTonicSynth::update);
  ofRemoveListener(ofEvents().audioRequested, this, &ofxTonicSynth::audioRequested);
}

ofEvent<float>* ofxTonicSynth::createOFEvent(ControlGenerator gen){
  ofControlChangetSubscriber* subscriber = new ofControlChangetSubscriber();
  publishChanges(gen, "").addValueChangedSubscriber(subscriber);
  return &subscriber->event;
}

void ofxTonicSynth::update(ofEventArgs& args){
  sendControlChangesToSubscribers();
}


void ofxTonicSynth::audioRequested(ofAudioEventArgs& args){

}

}
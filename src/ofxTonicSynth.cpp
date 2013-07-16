//
//  ofxTonicSynth.cpp
//  example_UI_Callbacks
//
//  Created by Morgan Packard on 7/13/13.
//
//

#include "ofxTonicSynth.h"

namespace Tonic {


ofxTonicSynth::~ofxTonicSynth(){
  vector<ofControlChangetSubscriber*>::iterator it = eventSubscribers.begin();
  for(; it < eventSubscribers.end(); it++){
    delete *it;
  }
}

ofEvent<float>* ofxTonicSynth::createOFEvent(ControlGenerator gen){
  ofControlChangetSubscriber* subscriber = new ofControlChangetSubscriber();
  publishChanges(gen, "").addValueChangedSubscriber(subscriber);
  return &subscriber->event;
}

void ofxTonicSynth::update(ofEventArgs& args){
  sendControlChangesToSubscribers();
}

}
//
//  ofxTonicSynth.h
//  example_UI_Callbacks
//
//  Created by Morgan Packard on 7/13/13.
//
//

#ifndef __example_UI_Callbacks__ofxTonicSynth__
#define __example_UI_Callbacks__ofxTonicSynth__

#include <iostream>
#include "Tonic/Synth.h"
#include "ofEvents.h"

/*
  Simple subclass of tonic::Synth to add a few oF-specific things. Mainly integration with the oF event system.
*/

namespace Tonic {

class ofxTonicSynth;

class ofControlChangetSubscriber : public ControlChangeSubscriber{
  public:
    ofEvent<float> event;
    ofxTonicSynth* synth;
    void valueChanged(string name, TonicFloat value){
      ofNotifyEvent(event, value, synth);
    }
};

class ofxTonicSynth : public Synth{
  
  vector<ofControlChangetSubscriber*> eventSubscribers;

public:

  ofxTonicSynth();

  ~ofxTonicSynth();
  
  void update(ofEventArgs& args);
  void audioRequested(ofAudioEventArgs& args);
  
  /*!
    Event is triggered every time the value of the ControlGenerator changes.
  */
  ofEvent<float>* createOFEvent(ControlGenerator gen);
  
};


}

#endif /* defined(__example_UI_Callbacks__ofxTonicSynth__) */

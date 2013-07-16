#pragma once

#include "ofMain.h"
#include "ofxTonic.h"

using namespace Tonic;


class FlashingRectangle{

  bool isOn;
  ofRectangle rect;
  ofColor color;
  
public:
  
  FlashingRectangle(float xPosition, float width):
    rect(xPosition, 0, width, ofGetScreenHeight()){
  }
  
  void pulseHappened(float& val){
    isOn = val;
  }
  
  void filterFreqChanged(float & val){
    rect.setHeight((1 - val) * ofGetScreenHeight());
    float colorIntensity =0.2 + 0.8 * (val);
    color = ofColor(55 * colorIntensity, 55 * colorIntensity, 255 * colorIntensity);
  }
  
  void draw(){
    if(isOn){
      ofSetColor(color);
      ofRect(rect);
    }
  }
  
};


class testApp : public ofBaseApp{

  ofxTonicSynth synth;
  vector<FlashingRectangle*> flashingRectangles;

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    void audioRequested (float * output, int bufferSize, int nChannels);
		
};

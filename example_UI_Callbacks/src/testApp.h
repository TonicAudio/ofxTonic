#pragma once

#include "ofMain.h"
#include "Tonic.h"

using namespace Tonic;


class FlashingRectangle{

protected:

  ///////////////////////
  // Inner class Flasher
  ///////////////////////
  class Flasher : public ControlChangeSubscriber{
  public:
    FlashingRectangle* target;
    Flasher(FlashingRectangle* targetArg):target(targetArg){}
    void valueChanged(string name, float value){
      target->brightness = value;
    }
  };
  
  ///////////////////////
  // Inner class IntensitySetter
  ///////////////////////    
  class IntensitySetter : public ControlChangeSubscriber{
    FlashingRectangle* target;
  public:
    IntensitySetter(FlashingRectangle* targetArg):target(targetArg){}
    void valueChanged(string name, float value){
      target->setIntensity(1 - value);
    }
  };
  
public:
  
  float brightness;
  float intensity;
  ofRectangle rect;
  Flasher flasher;
  IntensitySetter intensitySetter;
  
  float maxWidth;
  
  FlashingRectangle() : flasher(this), intensitySetter(this){
    brightness = 0;
  }
  
  void update(){
   // brightness = max(0, brightness - 0.1);
  }
  
  ofColor getColor(){
    int colorVal = (brightness > 0.5 ? 255 : 0) * intensity;
    if(brightness > 0.5){
      float colorIntensity = 0.2 + 0.8 * intensity;
      return ofColor(255 * colorIntensity, 151 * colorIntensity, 0);
    }else{
      return ofColor(colorVal, colorVal, colorVal);
    }
  }
  
  void flash(){
      brightness = 1;
  }
  
  void setIntensity(float intensityArg){
    intensity = intensityArg;
  }
  
};


class testApp : public ofBaseApp{

  Synth synth;
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

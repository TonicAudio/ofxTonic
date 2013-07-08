#pragma once

#include "ofMain.h"
#include "Tonic.h"

using namespace Tonic;


class FlashingRectangle{

protected:

  float brightness;
  float intensity;

  class Flasher : public ControlChangeSubscriber{
    FlashingRectangle* target;
  public:
    Flasher(FlashingRectangle* targetArg):target(targetArg){}
    void valueChanged(string name, float value){target->flash();}
  };
      
  class IntensitySetter : public ControlChangeSubscriber{
    FlashingRectangle* target;
  public:
    IntensitySetter(FlashingRectangle* targetArg):target(targetArg){}
    void valueChanged(string name, float value){target->setIntensity(1 - value);}
  };
  
public:
  
  ofRectangle rect;
  Flasher flasher;
  IntensitySetter intensitySetter;
  
  float maxWidth;
  
  FlashingRectangle() : flasher(this), intensitySetter(this){
    brightness = 0;
  }
  
  void update(){
    brightness = max(0, brightness - 0.1);
  }
  
  ofColor getColor(){
    int colorVal = (brightness > 0.5 ? 255 : 0) * intensity;
    if(brightness > 0.5){
      return ofColor(255 * 0.5, 151 * 0.5, 0 * 0.5);
    }else{
      return ofColor(colorVal, colorVal, colorVal);
    }
  }
  
  void flash(){
      brightness = 1;
  }
  
  void setIntensity(float intensityArg){
    rect.setWidth(intensityArg * maxWidth);
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

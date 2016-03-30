ofxTonic
===========

The Tonic audio synthesis Library, wrapped for openFrameworks. Tonic is a high performance, pure C++ audio patching tool with a refreshingly crisp and simple syntax. 

For info on Tonic, see:
https://github.com/TonicAudio/Tonic

Watch a video tutorial at https://www.youtube.com/watch?v=ppf3b8BC6yU

The code contained in the src/ folder of this repository is copied directly from this commit in the main Tonic repository:
a2c2546ec9923ad79296b06e181a2b6fff69394d

Note: we are currently reviving and updating Tonic. We need to update the
examples here to make them work in openFrameworks 0.9.3.
We have tested this fork on XCode (OSX) and QtCreator (Linux) and the examples
were running fine:
https://github.com/jeonghopark/ofxTonic/tree/a292ea983b4f22f0e2e33c3b4d19b54ada7daf28
We will port back those changes soon.

__Supported Platforms__

XCode projects for now. Full PC, Linux support should be easy and arriving soon (hopefully), but hasn't been tested yet.

__Example__

```cpp

// ofApp.h

#pragma once

#include "ofMain.h"
#include "ofxTonic.h"

using namespace Tonic;

class ofApp : public ofBaseApp{

    ofxTonicSynth synth;

public:
    void setup();
    void update();
    void draw();
    void audioRequested (float * output, int bufferSize, int nChannels);
};

// ofApp.cpp

#include "ofApp.h"

void ofApp::setup() {
    // set up a stereo output in oF
    ofSoundStreamSettings settings;
    settings.numOutputChannels = 2;
    settings.setOutListener(this);
    ofSoundStreamSetup(settings);

    // create a sine wave we'll use for some vibrato
    SineWave vibratoOsc = SineWave().freq(10);

    // plug that frequency generator into the frequency slot of the main audio-producing sine wave
    SineWave tone = SineWave().freq(400 + 20 * vibratoOsc);

    // set the synth's final output generator
    synth.setOutputGen( tone );
}

void ofApp::update() { }

void ofApp::draw() { }

void ofApp::audioRequested (float * output, int bufferSize, int nChannels) {
    synth.fillBufferOfFloats(output, bufferSize, nChannels);
}

```



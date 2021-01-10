#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxFft.h"

#define BUFFER_SIZE      256
#define HORIZONTAL_CELLS 128

class ofApp : public ofBaseApp{
	
	public:
		
		void setup();
		void setupGui();
		void update();
		void draw();
		void drawGui(ofEventArgs & args);
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
				
		void audioIn(ofSoundBuffer & input);

        array<int,HORIZONTAL_CELLS + 2> initialSeed;    
        array<int,HORIZONTAL_CELLS + 2> seed;    
            
		std::array<float, HORIZONTAL_CELLS> fftSmoothed{{0}};
		
		ofSoundStream soundStream;
		ofParameterGroup parameters;
		ofParameter<float> threshold;
		ofParameter<int> rule;
		ofParameter<bool> beat;
		ofParameter<float> duration;
		ofxPanel gui;
        ofxFft* fft;

        ofMutex soundMutex;
	    array<float, 256> drawBins, middleBins, audioBins;

        int lastSeed = 0;
	    int bufferSize;

        int bpmLastChange = 0;
        int bpmLastClick  = 0;
        int bpmDuration   = 0;
		
        ofVideoPlayer 		bgMovie;
        ofVideoPlayer 		overlayMovie;
    
        ofPixels pixels1;
        ofPixels pixels2;

};

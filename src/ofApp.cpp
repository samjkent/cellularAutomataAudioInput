#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	 
	
	ofSetVerticalSync(true);
	
	soundStream.printDeviceList();
	
    bgMovie.load("movies/mountain.mp4");
	bgMovie.setLoopState(OF_LOOP_NORMAL);
    bgMovie.setUseTexture(false);
	bgMovie.play();
    
    overlayMovie.load("movies/ubahn.mp4");
    overlayMovie.setUseTexture(false);
	overlayMovie.setLoopState(OF_LOOP_NORMAL);
	overlayMovie.play();
	
    bufferSize = BUFFER_SIZE;


    rule = 0x0;

	ofSoundStreamSettings settings;

	auto devices = soundStream.getMatchingDevices("default");
	if(!devices.empty()){
		settings.setInDevice(devices[0]);
	}
    
    fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING);

	settings.setInListener(this);
	settings.sampleRate = 44100;
	settings.numOutputChannels = 0;
	settings.numInputChannels = 2;
	settings.bufferSize = bufferSize;
	soundStream.setup(settings);
    	
    // Initial seed
    for(int x = 1; x < HORIZONTAL_CELLS + 1; x++) {
        initialSeed[x] = rand() % 2;
    }

}

//--------------------------------------------------------------
void ofApp::update(){

    // Use latest fft
    for(int i = 0; i < HORIZONTAL_CELLS; i++) {
        initialSeed[i] = (drawBins[i] > threshold) ? 1 : 0;
    }

    // Change rule
    if(beat && duration > 0) {
        if((ofGetElapsedTimeMillis() - bpmLastChange) > duration) {
            rule = rand() % 256;
            bpmLastChange = ofGetElapsedTimeMillis();
        }
    }

    // Update vid
    bgMovie.update();
    overlayMovie.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

    pixels1 = bgMovie.getPixels();
    pixels2 = overlayMovie.getPixels();
    
    ofTexture textureOut;
    textureOut.allocate(overlayMovie.getWidth(),overlayMovie.getHeight(),GL_RGB);
    
    array<int,HORIZONTAL_CELLS + 2> iteration_seed;    
    int w = overlayMovie.getWidth() / HORIZONTAL_CELLS;

    for(int y=0; y < overlayMovie.getHeight(); y++) {

        // Calculate iteration on pixel boundary
        if((y % w) == 0) {
            for(int n = 1; n < HORIZONTAL_CELLS + 1; n++) {
                iteration_seed[n] = 0;
                // First row
                if(y == 0) {
                    if(initialSeed[n] == 1) {
                        iteration_seed[n] = 1;
                    }
                } else { // Subsequent rows
                    uint8_t pattern = seed[n-1] << 2 | seed[n] << 1 | seed[n+1];
                    if((rule) & (1<<(pattern))) {
                        iteration_seed[n] = 1;
                    }
                }
            }

            seed = iteration_seed;
        }

        // Draw row                 
        for(int x=0; x < overlayMovie.getWidth(); x++) {
    
            int base = 3*((y*overlayMovie.getWidth()) + x);
            
            if(iteration_seed[x/w]) {
                pixels1[base]     = pixels2[base];
                pixels1[base + 1] = pixels2[base + 1];
                pixels1[base + 2] = pixels2[base + 2];
            }

        }
        
    }

    textureOut.loadData(pixels1);
    textureOut.draw(0,0,ofGetWindowWidth(),ofGetWindowHeight());
    
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer & input){
    fft->setSignal(input.getBuffer().data());
    float* curFft = fft->getAmplitude();
	
    for(int i = 0; i < fft->getBinSize(); i++) {
		drawBins[i] = curFft[i];
	}
    
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::setupGui(){
	parameters.setName("parameters");
	parameters.add(threshold.set("threshold",0.005,0,0.02));
	parameters.add(rule.set("rule #",0,0,255));
	parameters.add(beat.set("change on timeout",0,0,255));
	parameters.add(duration.set("duration",0,0,255));
	gui.setup(parameters);
	ofSetBackgroundColor(0);
}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs & args){
	gui.draw();
}


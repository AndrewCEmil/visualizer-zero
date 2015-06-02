#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	 
	
	ofSetVerticalSync(true);
	ofSetCircleResolution(80);
    ofBackground(54, 54, 54);
	
	// 0 output channels, 
	// 2 input channels
	// 44100 samples per second
	// 256 samples per buffer
	// 4 num buffers (latency)
	
	soundStream.listDevices();
	
	//if you want to set a different device id 
	soundStream.setDeviceID(1); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
		
	smoothedVol     = 0.0;
	scaledVol		= 0.0;
    
    //FFT stuff
    plotHeight = 768;
    plotWidth = 1024;
    bufferSize = 2048;
    
    fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING, OF_FFT_FFTW);
    
    drawBins.resize(fft->getBinSize());
    middleBins.resize(fft->getBinSize());
    audioBins.resize(fft->getBinSize());
    
    shader.load("shadersGL3/shader");
    

	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);

}

//--------------------------------------------------------------
void ofApp::update(){ }

//--------------------------------------------------------------
void ofApp::draw(){
	ofPushStyle();
		ofPushMatrix();
    
		ofSetColor(225);
		ofDrawBitmapString("Frequency domain", 4, 18);
        string msg = ofToString((int) ofGetFrameRate()) + " fps";
        ofDrawBitmapString(msg, 50, 100);
    
        soundMutex.lock();
        drawBins = middleBins;
        soundMutex.unlock();
    
        plot(drawBins, -plotHeight, plotHeight / 2);
    
        ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::plot(vector<float>& buffer, float scale, float offset) {
    ofSetColor(255);
    int n = buffer.size();
    double avg = 0;
    int leftx = 0;
    int lefty = 0;
    int rightx = 0;
    int righty = 0;
    for (int i = 0; i < 2; i++) {
        avg = 0;
        for (int j = 0; j < buffer.size() / (2 - i); j++) {
            avg += sqrt(buffer[i + j]);
        }
        avg = (avg / (buffer.size() / 2));

        if(i == 0) {
            leftx = buffer.size() / 4;
            lefty = plotHeight - (avg * scale);
        } else {
            rightx = 3 * buffer.size() / 4;
            righty = plotHeight - (avg * scale);
        }
    }
    
    float leftdist = sqrt((((float(plotWidth) /2) - leftx) * ((float(plotWidth) /2) - leftx)) + (((float(plotHeight) /2) - lefty) * ((float(plotWidth) /2) - lefty)));
    float rightdist = sqrt((((float(plotWidth) /2)  - rightx) * ((float(plotWidth) /2)  - rightx)) + (((float(plotWidth) /2) - righty) * ((float(plotWidth) /2) - righty)));
    float result = (leftdist + rightdist) / 2.0;
    shader.begin();
    std::cout << "leftx: " << float(leftx) << std::endl;
    shader.setUniform1f("leftxv", float(leftx));
    std::cout << "lefty: " << float(lefty) << std::endl;
    shader.setUniform1f("leftyv", float(lefty));
    std::cout << "rightx: " << float(rightx) << std::endl;
    shader.setUniform1f("rightxv", float(rightx));
    std::cout << "righty: " << float(righty) << std::endl;
    shader.setUniform1f("rightyv", float(righty));
    ofRect(0, 0, plotWidth, plotHeight);
    std::cout << "RESULT: " << result << std::endl;
    shader.end();
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels) {
    //FFT section
    float maxValue = 0;
    for(int i = 0; i < bufferSize; i++) {
        if(abs(input[i]) > maxValue) {
            maxValue = abs(input[i]);
        }
    }
    for(int i = 0; i < bufferSize; i++) {
        input[i] /= maxValue;
    }
    
    fft->setSignal(input);
    
    float* curFft = fft->getAmplitude();
    memcpy(&audioBins[0], curFft, sizeof(float) * fft->getBinSize());
    
    maxValue = 0;
    for(int i = 0; i < fft->getBinSize(); i++) {
        if(abs(audioBins[i]) > maxValue) {
            maxValue = abs(audioBins[i]);
        }
    }
    for(int i = 0; i < fft->getBinSize(); i++) {
        audioBins[i] /= maxValue;
    }
    
    soundMutex.lock();
    middleBins = audioBins;
    soundMutex.unlock();
	
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


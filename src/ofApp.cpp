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
    plotHeight = 512;
    bufferSize = 2048;
    
    fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING, OF_FFT_FFTW);
    
    drawBins.resize(fft->getBinSize());
    middleBins.resize(fft->getBinSize());
    audioBins.resize(fft->getBinSize());

	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);

}

//--------------------------------------------------------------
void ofApp::update(){ }

//--------------------------------------------------------------
void ofApp::draw(){
	ofNoFill();
    ofColor colorOne;
    ofColor colorTwo;
    
    colorOne.set (255, 0, 0);
    colorTwo.set (0, 0, 255);
    
    ofEnableAlphaBlending();
    ofBackgroundGradient(colorOne, colorTwo, OF_GRADIENT_LINEAR);

	// draw the frequency domain:
	ofPushStyle();
		ofPushMatrix();
		//ofTranslate(32, 370, 0);
			

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
    
    ofPushStyle();
    ofFill();
    ofSetColor(0, 0, 0, 256);
    ofRect(0, plotHeight, drawBins.size(), 768 - plotHeight);
    ofPopStyle();

}

//--------------------------------------------------------------
void ofApp::plot(vector<float>& buffer, float scale, float offset) {
    ofNoFill();
    int n = buffer.size();
    ofRect(0, 0, n, plotHeight);
    glPushMatrix();
    glTranslatef(0, plotHeight / 2 + offset, 0);
    double avg = 0;
    double val = 0;
    for (int i = 0; i < n; i += 50) {
        avg = 0;
        for (int j = 0; j < 50 && i + j < n; j++) {
            avg += sqrt(buffer[i + j]);
            ofVertex(i + j, sqrt(buffer[i + j]) * scale);
        }
        avg = (avg / std::min(50, n - i));

        ofPushStyle();
        ofFill();
        ofSetColor(0, 0, 0, 256);
        ofRect(i, avg * scale, 50, - (plotHeight - (avg * scale)));
        ofPopStyle();
        
        /*
        for (int y = 0; y < 512; y += 30) {
            val = avg * scale;
            if (abs(val) > y) {
                ofPushStyle();
                ofFill();
                //ofSetColor(y, 256 - y, ((double)i / (double)n) * 256.0);
                ofSetColor(0, 0, 0, 0);
                //ofRect(i, -y, 50, 50);
                ofCircle(i + 10, -y, 10);
                ofPopStyle();
            }
        }*/
    }
    glPopMatrix();
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
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


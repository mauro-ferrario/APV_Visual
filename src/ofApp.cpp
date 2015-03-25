#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofSetFrameRate(30);
  ofBackground(0);
  ofSetVerticalSync(true);
  visual.setup();
  int bufferSize = 256;
  ofSoundStreamSetup(0, 1, this, 44100, beat.getBufferSize(), 4);
}

//--------------------------------------------------------------
void ofApp::update(){
  visual.update();
  beat.update(ofGetElapsedTimeMillis());
}

//--------------------------------------------------------------
void ofApp::draw(){
  visual.draw();
  ofDrawBitmapString(ofToString(ofGetFrameRate()), ofPoint(10,10));
  ofDrawBitmapString(ofToString(visual.particleSystem.particles.size()), ofPoint(10,30));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
  if(key == 'f')
    ofToggleFullscreen();
  if(key == 'a')
    visual.track.play();
  if(key == 's')
  {
    ofPixels pixels;
    visual.mainFbo.readToPixels(pixels);
    ofSaveImage(pixels, "test.png");
  }
}

void ofApp::audioIn(float * input, int bufferSize, int nChannels)
{
  beat.audioReceived(input, bufferSize, nChannels);
  visual.audioIn(input, bufferSize, nChannels, beat.kick());
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
  visual.addParticle(ofVec3f(x,y),  ofRandom(2,10), 0);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
  visual.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::exit(){
  visual.exit();
}

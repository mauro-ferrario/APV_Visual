#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofSetFrameRate(60);
  ofBackground(0);
  ofSetVerticalSync(true);
  visual.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
  visual.update();

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

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

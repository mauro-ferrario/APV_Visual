//
//  APVVisual.cpp
//  APV_Visual
//
//  Created by Mauro Ferrario on 05/02/15.
//
//

#include "APVVisual.h"


APVVisual::APVVisual()
{
  
}

void APVVisual::setup()
{
  size = ofVec2f(ofGetWindowWidth(), ofGetWindowHeight());
  allocateFBO(size.x, size.y);
  initParticleSystem();
  backgroundColor = ofColor(0);
  ofSetCircleResolution(60);
  ofEnableAlphaBlending();
  globalAlphaCoefficent   = 0;
  bDrawPoint              = true;
  bDrawLinePointToPoint   = true;
  bDrawTriangle           = true;
  bConnectPointToPrev     = true;
  minDistancePointToPoint = 0;
  maxDistancePointToPoint = 1000;
  distancePointToPoint    = 100;
  minTriangleCoefficent   = .000001;
  maxTriangleCoefficent   = 3;
  triangleCoefficent      = .5;
  bSameColorTriangle      = true;
  bTimeAlphaTriangle      = true;
  
  left.assign(256, 0.0);
  right.assign(256, 0.0);
  
  smoothedVol     = 0.0;
  scaledVol		= 0.0;
  bufferCounter	= 0;
  totPoints = 0;
}

void APVVisual::initParticleSystem()
{
  particleSystem.init();
  particleSystem.setup();
  //particleSystem.initGoofyNoise();
  //particleSystem.moveNoise = true;
}

void APVVisual::allocateFBO(int width, int height)
{
  mainFbo.allocate(width, height, GL_RGBA32F);
  mainFbo.begin();
  ofClear(0,255);
  mainFbo.end();
}

void APVVisual::update()
{
  mainFbo.begin();
  ofDisableAlphaBlending();
  ofClear(0,255);
  particleSystem.updateAndDraw();
  mainFbo.end();
}

void APVVisual::drawBackground()
{
  ofPushStyle();
  ofSetColor(backgroundColor);
  ofRect(0,0,size.x, size.y);
  ofPopStyle();
}

void APVVisual::draw()
{
  ofPushStyle();
  ofSetColor(255);
  mainFbo.draw(0,0);
  ofPopStyle();
}

GoofyParticle* APVVisual::addParticle(ofVec3f newPosition, float maxVelocity, long int life)
{
  particleSystem.addParticle(newPosition, maxVelocity, life);
}
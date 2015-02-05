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
  allocateFBO(ofGetWindowWidth(), ofGetWindowHeight());
  initParticleSystem();
}

void APVVisual::initParticleSystem()
{
  particleSystem.init();
  particleSystem.setup();
  particleSystem.initGoofyNoise();
  particleSystem.moveNoise = true;
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
  ofClear(0,255);
  particleSystem.updateAndDraw();
  mainFbo.end();
}

void APVVisual::draw()
{
  mainFbo.draw(0,0);
}

GoofyParticle* APVVisual::addParticle(ofVec3f newPosition, float maxVelocity, long int life)
{
  particleSystem.addParticle(newPosition, maxVelocity, life);
}
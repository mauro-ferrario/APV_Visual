//
//  APVParticleSystem.cpp
//  APV_Visual
//
//  Created by Mauro Ferrario on 05/02/15.
//
//

#include "APVParticleSystem.h"
#include "APVVisual.h"

APVParticleSystem::APVParticleSystem()
{
  
}

void APVParticleSystem::setup()
{
  sameFriction = .1;
  sameSpring = .1;
  repulsionForce = 1;
}

GoofyParticle* APVParticleSystem::addParticle(ofVec3f newPosition, float maxVelocity, long int life)
{
  GoofyParticle* particle = new GoofyParticle(newPosition, maxVelocity * percParticleSpeed, sameSpring, sameFriction);
  particle->target = newPosition;
  particles.push_back(particle);
  particle = NULL;
  return particle;
}

void APVParticleSystem::updateAndDrawWithVisual(APVVisual* visual)
{
  if(moveNoise)
    goofyPerlinNoise.update();
  vector<GoofyParticle*>::iterator vItr = particles.begin();
  int cont = 0;
  while ( vItr != particles.end() )
  {
    if(bFollowTarget)
      (*vItr)->followTarget();
    if(moveNoise)
      (*vItr)->moveWithNoise(goofyPerlinNoise, goofyPerlinNoiseForce);;
    if(followFlow)
      (*vItr)->follow(goofyFlowField);
    applyRepulsions((*vItr));
    applyAttraction((*vItr));
    (*vItr)->update();
    lastActionInsideUpdateLoop((*vItr));
    
    if (!(*vItr)->active)
    {
      delete * vItr;
      vItr = particles.erase( vItr );
    }
    else
    {
      if((*vItr)->active)
      {
        GoofyParticle* prevPrevParticle;
        if(cont > 0 && visual->bConnectPointToPrev)
            connectPrevPoint(vItr);
        if(visual->bDrawLinePointToPoint || visual->bDrawTriangle)
        loopIn(vItr, visual, cont);
        if(visual->bDrawPoint)
        {
          ofSetColor(255);
          ofCircle((*vItr)->position, 1);
        }
        cont++;
      }
      vItr++;
    }
  }
  visual = NULL;
}

void APVParticleSystem::loopIn(vector<GoofyParticle*>::iterator vItr, APVVisual* visual, int cont)
{
  vector<GoofyParticle*>::iterator pPointerIn = vItr;
  ofPushStyle();
  ofEnableAlphaBlending();
  while ( pPointerIn != particles.end() )
  {
    if(cont > 2 && visual->bDrawTriangle)
      drawTriangle(vItr, pPointerIn -1, pPointerIn);
    if(visual->bDrawLinePointToPoint)
      drawConnectPoints(vItr, pPointerIn);
    pPointerIn++;
  }
  ofDisableAlphaBlending();
  ofPopStyle();
  visual = NULL;
}

void APVParticleSystem::drawTriangle(vector<GoofyParticle*>::iterator firstPoint, vector<GoofyParticle*>::iterator secondPoint, vector<GoofyParticle*>::iterator thirdPoint)
{
  float triangleCoefficent = .954;
  float alpha = 25;
  bool bSameColorTriangle = true;
  float perimeter = getTrianglePerimeter(firstPoint, secondPoint, thirdPoint);
  
  ofPushStyle();
  
  if(bSameColorTriangle)
  {
    ofSetColor(255,0,0,alpha);
  }
  else
  {
    if(perimeter < 700 / triangleCoefficent)
      ofSetColor(255, 0, 0, alpha);
    else if(perimeter >= 700 / triangleCoefficent && perimeter < 900 / triangleCoefficent)
      ofSetColor(0, 255, 0, alpha);
    else if(perimeter >= 900 / triangleCoefficent && perimeter <= 1300 / triangleCoefficent)
      ofSetColor(0, 0, 255, alpha);
  }
  if(perimeter < 1000 * triangleCoefficent && perimeter > 700)
  {
    ofFill();
    ofTriangle((*firstPoint)->position.x, (*firstPoint)->position.y, (*secondPoint)->position.x, (*secondPoint)->position.y, (*thirdPoint)->position.x, (*thirdPoint)->position.y);
  }
  ofPopStyle();
}


float APVParticleSystem::getTrianglePerimeter(vector<GoofyParticle*>::iterator firstPoint, vector<GoofyParticle*>::iterator secondPoint, vector<GoofyParticle*>::iterator thirdPoint)
{
  return abs(ofDist((*firstPoint)->position.x, (*firstPoint)->position.y, (*thirdPoint)->position.x, (*thirdPoint)->position.y)) + abs(ofDist((*firstPoint)->position.x, (*firstPoint)->position.y, (*secondPoint)->position.x, (*secondPoint)->position.y)) + abs(ofDist((*secondPoint)->position.x, (*secondPoint)->position.y, (*thirdPoint)->position.x, (*thirdPoint)->position.y));
}



void APVParticleSystem::connectPrevPoint(vector<GoofyParticle*>::iterator vItr)
{
  vector<GoofyParticle*>::iterator prevParticle = vItr - 1;
  ofPushStyle();
  ofSetColor(255,255);
  if((*prevParticle)->active)
  {
    ofLine((*vItr)->position, (*prevParticle)->position);
  }
  ofPopStyle();
}

void APVParticleSystem::drawConnectPoints(vector<GoofyParticle*>::iterator vItr, vector<GoofyParticle*>::iterator pPointerIn)
{
  float distance = (*vItr)->position.distance((*pPointerIn)->position);
  if(distance > 100 && distance < 500)
  {
    ofPushStyle();
    ofSetColor(255,10);
    ofLine((*vItr)->position, (*pPointerIn)->position);
    ofPopStyle();
  }
}


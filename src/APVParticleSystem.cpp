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

void APVParticleSystem::setup(APVVisual* _visual)
{
  this->visual = _visual;
  _visual = NULL;
  sameFriction = .83;
  sameSpring = .183;
  sameLimitVelocity = 100;
  repulsionForce = 1;
}

APVParticle* APVParticleSystem::addParticle(ofVec3f newPosition, float maxVelocity, long int life)
{
  APVParticle* particle = new APVParticle(newPosition, maxVelocity * percParticleSpeed, ofColor(255), sameSpring, sameFriction);
  particle->target = newPosition;
  particles.push_back(particle);
  //particle = NULL;
  return particle;
}

void APVParticleSystem::updateAndDrawWithVisual()
{
  ofEnableAlphaBlending();
  if(moveNoise)
    goofyPerlinNoise.update();
  vector<GoofyParticle*>::iterator vItr = particles.begin();
  int cont = 0;
  int totParticles = particles.size();
  int halfTotParticles = totParticles*.5;
  ofVec2f center = ofVec2f(ofGetWindowWidth()*.5, ofGetWindowHeight()*.5);
  
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
      APVParticle* tempParticle   = (APVParticle*)(*vItr);
      tempParticle->friction      = sameFriction;
      tempParticle->spring        = sameSpring;
      tempParticle->limitVelocity = sameLimitVelocity * 100;
      
      if((*vItr)->active)
      {
        if(cont < halfTotParticles)
        {
          if(visual->left[(int)ofMap(cont, 0, halfTotParticles, 0, 256, true)] > 0)
          {
            tempParticle->audioCoefficent = visual->left[(int)ofMap(cont, 0, halfTotParticles, 0, 256, true)];
            float radius = ofGetWindowWidth()*.5;
            float force = tempParticle->audioCoefficent * visual->maxRepulsionForce * repulsionForce;
            float limitSpeed = false;
            GoofyMagneticPoint *repeller = new GoofyMagneticPoint(center, radius, force, limitSpeed);
            
            (*vItr)->applyRepulsion(repeller, false);
            delete repeller;
            repeller = NULL;
          }
        }
        else
        {
          if(visual->right[(int)ofMap(cont, halfTotParticles, totParticles - 1, 0, 256, true)] > 0)
          {
            tempParticle->audioCoefficent = visual->right[(int)ofMap(cont, halfTotParticles, totParticles - 1, 0, 256, true)];
            float radius = ofGetWindowWidth()*.5;
            float force = tempParticle->audioCoefficent * visual->maxRepulsionForce * repulsionForce;
            float limitSpeed = false;
            GoofyMagneticPoint *repeller = new GoofyMagneticPoint(center, radius, force, limitSpeed);
            
            (*vItr)->applyRepulsion(repeller, false);
            delete repeller;
            repeller = NULL;
          }
          // If i don't write this... one particle doesn't update audioCoefficent :-O
          if(cont == particles.size() -1) // || cont == particles.size()-1 || cont == 0)
          {
            tempParticle->audioCoefficent = visual->right[250];
            float radius = ofGetWindowWidth()*.5;
            float force = tempParticle->audioCoefficent * visual->maxRepulsionForce * repulsionForce;
            float limitSpeed = false;
            GoofyMagneticPoint *repeller = new GoofyMagneticPoint(center, radius, force, limitSpeed);
            
            (*vItr)->applyRepulsion(repeller, false);
            delete repeller;
            repeller = NULL;
          }
        }
        tempParticle = NULL;
        GoofyParticle* prevPrevParticle;
        if(cont > 0 && visual->bConnectPointToPrev)
            connectPrevPoint(vItr);
        if(visual->bDrawLinePointToPoint || visual->bDrawTriangle)
        loopIn(vItr, cont);
        if(visual->bDrawPoint)
        {
          ofPushStyle();
          APVParticle* tempParticle = (APVParticle*)(*vItr);
          float alpha = tempParticle->audioCoefficent * visual->globalAlphaCoefficent * 200;
          tempParticle = NULL;
          alpha = 255;
          if(!visual->invertColor)
            ofSetColor(255, alpha);
          else
            ofSetColor(0, alpha);
          ofCircle((*vItr)->position, 1);
          ofPopStyle();
        }
        cont++;
      }
      vItr++;
    }
  }
  ofDisableAlphaBlending();
}

void APVParticleSystem::removePoints(bool removeNow)
{
  vector<GoofyParticle*>::iterator vItr = particles.begin();
  while ( vItr != particles.end() )
  {
    if(removeNow)
      (*vItr)->life = 0;
    else
      (*vItr)->life = ofRandom(0,100);
    (*vItr)->lifeActive = true;
    vItr++;
  }
}

void APVParticleSystem::loopIn(vector<GoofyParticle*>::iterator vItr, int cont)
{
  vector<GoofyParticle*>::iterator pPointerIn = vItr;
  ofPushStyle();
  while ( pPointerIn != particles.end() )
  {
    if(cont > 2 && visual->bDrawTriangle)
      drawTriangle(vItr, pPointerIn -1, pPointerIn);
    if(visual->bDrawLinePointToPoint)
      drawConnectPoints(vItr, pPointerIn);
    pPointerIn++;
  }
  ofPopStyle();
}

void APVParticleSystem::drawTriangle(vector<GoofyParticle*>::iterator firstPoint, vector<GoofyParticle*>::iterator secondPoint, vector<GoofyParticle*>::iterator thirdPoint)
{
  float alpha = 25;
  bool bSameColorTriangle = true;
  float perimeter = getTrianglePerimeter(firstPoint, secondPoint, thirdPoint);
  
  APVParticle* tempParticle = (APVParticle*)(*firstPoint);
  alpha = tempParticle->audioCoefficent * visual->globalAlphaCoefficent * 50;
  tempParticle = NULL;
  ofPushStyle();
  
  ofColor triangleColor = visual->triangleColor;
  
  if(visual->invertColor)
    triangleColor.invert();
  triangleColor.a = alpha;
  
  
  if(bSameColorTriangle)
  {
    ofSetColor(triangleColor);
  }
  else
  {
    if(perimeter < 700 / visual->triangleCoefficent)
      ofSetColor(255, 0, 0, alpha);
    else if(perimeter >= 700 / visual->triangleCoefficent && perimeter < 900 / visual->triangleCoefficent)
      ofSetColor(0, 255, 0, alpha);
    else if(perimeter >= 900 / visual->triangleCoefficent && perimeter <= 1300 / visual->triangleCoefficent)
      ofSetColor(0, 0, 255, alpha);
  }
  if(perimeter > visual->minTrianglePerimeter * visual->trianglePerimeterCoefficent && perimeter < visual->maxTrianglePerimeter * visual->trianglePerimeterCoefficent)
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
  APVParticle* tempParticle = (APVParticle*)(*vItr);
  float alpha = tempParticle->audioCoefficent * visual->globalAlphaCoefficent * 255 * 2;
  tempParticle = NULL;
  ofPushStyle();
  vector<GoofyParticle*>::iterator prevParticle = vItr - 1;
  ofPushStyle();
  if(!visual->invertColor)
    ofSetColor(255, alpha);
  else
    ofSetColor(0, alpha);
  if((*prevParticle)->active)
  {
    ofLine((*vItr)->position, (*prevParticle)->position);
  }
  ofPopStyle();
}

void APVParticleSystem::drawConnectPoints(vector<GoofyParticle*>::iterator vItr, vector<GoofyParticle*>::iterator pPointerIn)
{
  APVParticle* tempParticle = (APVParticle*)(*vItr);
  float alpha = tempParticle->audioCoefficent * visual->globalAlphaCoefficent * 100;
  tempParticle = NULL;
  float distance = (*vItr)->position.distance((*pPointerIn)->position);
  if(distance > visual->minDistancePointToPoint * visual->distancePointToPointCoefficent && distance < visual->maxDistancePointToPoint * visual->distancePointToPointCoefficent)
  {
    ofPushStyle();
    if(!visual->invertColor)
      ofSetColor(255, alpha);
    else
      ofSetColor(0, alpha);
    ofLine((*vItr)->position, (*pPointerIn)->position);
    ofPopStyle();
  }
}


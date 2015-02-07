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
  repulsionForce = 1;
}

APVParticle* APVParticleSystem::addParticle(ofVec3f newPosition, float maxVelocity, long int life)
{
  APVParticle* particle = new APVParticle(newPosition, maxVelocity * percParticleSpeed, ofColor(255), sameSpring, sameFriction);
  particle->target = newPosition;
  particles.push_back(particle);
  particle = NULL;
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
  int totMax = 0;
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
      APVParticle* tempParticle = (APVParticle*)(*vItr);
      if((*vItr)->active)
      {
        if(cont < halfTotParticles)
        {
          if(visual->left[(int)ofMap(cont, 0, halfTotParticles, 0, 256)] > 0)
          {
            if(cont < 10)
              totMax += visual->left[(int)ofMap(cont, 0, halfTotParticles, 0, 256)];
            tempParticle->audioCoefficent = visual->left[(int)ofMap(cont, 0, halfTotParticles, 0, 256)] * 200;
            float radius = ofGetWindowWidth()*.5;
            float force = tempParticle->audioCoefficent * 1;
            float limitSpeed = 10;
            GoofyMagneticPoint *repeller = new GoofyMagneticPoint(center, radius, force, limitSpeed);
            
            (*vItr)->applyRepulsion(repeller);
            delete repeller;
            repeller = NULL;
            
            
          //  if(!emitterMode)
           //  tempParticle->applyRepulsion(repulsionForce * points[a]->audioCoeff , repulsionFromTarget);
          }
        }
        else
        {
          if(visual->right[(int)ofMap(cont, halfTotParticles, totParticles - 1, 0, 256)] > 0)
          {
            tempParticle->audioCoefficent = visual->right[(int)ofMap(cont, halfTotParticles, totParticles - 1, 0, 256)] * 100;
            float radius = ofGetWindowWidth()*.5;
            float force = tempParticle->audioCoefficent * 1;
            float limitSpeed = 10;
            GoofyMagneticPoint *repeller = new GoofyMagneticPoint(center, radius, force, limitSpeed);
            
            (*vItr)->applyRepulsion(repeller);
            delete repeller;
            repeller = NULL;

            //   if(!emitterMode)
          //    points[a]->applyRepulsion(repulsionForce * points[a]->audioCoeff, repulsionFromTarget);
          }
        }
        tempParticle = NULL;
        /*
        
        if(points[a]->target.x == NULL)
        {
          if(points[a]->position.x > ofGetWindowWidth() + LIMIT_OUTISDE || points[a]->position.x < - LIMIT_OUTISDE || points[a]->position.y < -LIMIT_OUTISDE || points[a]->position.y > ofGetWindowHeight() + LIMIT_OUTISDE)
          {
            Particle* pPoint = points[a];
            //   points.begin() + a) = 0; // Devo mettere a 0 l'indirizzo a cui punta
            points.erase(points.begin() + a);
            delete pPoint;  // Cancello l'oggetto
            //pPoint = NULL;
          }
        }
        */
        
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
          ofSetColor(255, alpha);
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
  alpha = tempParticle->audioCoefficent * visual->globalAlphaCoefficent;
  tempParticle = NULL;
  ofPushStyle();
  
  if(bSameColorTriangle)
  {
    ofSetColor(255,0,0,alpha);
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
  if(perimeter > 100 * visual->triangleCoefficent && perimeter < 150 * visual->triangleCoefficent)
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
  float alpha = tempParticle->audioCoefficent * visual->globalAlphaCoefficent * 200;
  tempParticle = NULL;
  ofPushStyle();
  vector<GoofyParticle*>::iterator prevParticle = vItr - 1;
  ofPushStyle();
  ofSetColor(255, alpha);
  if((*prevParticle)->active)
  {
    ofLine((*vItr)->position, (*prevParticle)->position);
  }
  ofPopStyle();
}

void APVParticleSystem::drawConnectPoints(vector<GoofyParticle*>::iterator vItr, vector<GoofyParticle*>::iterator pPointerIn)
{
  APVParticle* tempParticle = (APVParticle*)(*vItr);
  float alpha = tempParticle->audioCoefficent * visual->globalAlphaCoefficent;
  tempParticle = NULL;
  float distance = (*vItr)->position.distance((*pPointerIn)->position);
  if(distance > 100 && distance < 500)
  {
    ofPushStyle();
    ofSetColor(255, alpha);
    ofLine((*vItr)->position, (*pPointerIn)->position);
    ofPopStyle();
  }
}


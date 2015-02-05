//
//  APVParticleSystem.cpp
//  APV_Visual
//
//  Created by Mauro Ferrario on 05/02/15.
//
//

#include "APVParticleSystem.h"


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

void APVParticleSystem::updateAndDraw()
{
  if(moveNoise)
    goofyPerlinNoise.update();
  vector<GoofyParticle*>::iterator vItr = particles.begin();
  vector<GoofyParticle*>::iterator prevParticle = particles.begin();
  vector<GoofyParticle*>::iterator pPointerIn;
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
    
    
    

    
//    if(cont > 2)
//    {
//      prevPrevParticle = prevParticle - 1;
//    }
    
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
        
        if(cont > 1)
        {
          prevParticle = vItr - 1;
          
          if((*prevParticle)->active)
          {
            ofLine((*vItr)->position, (*prevParticle)->position);
          }
        }
        
        ofPushStyle();
        
        ofEnableAlphaBlending();
        ofSetColor(255,10);
        pPointerIn = vItr;
        while ( pPointerIn != particles.end() )
        {
          float distance = (*vItr)->position.distance((*pPointerIn)->position);
          if(distance < 100)
            ofLine((*vItr)->position, (*pPointerIn)->position);
          pPointerIn++;
        }
        ofPopStyle();
        
        ofCircle((*vItr)->position, 1);
        
        cont++;
      }
      vItr++;
    }
  }
}
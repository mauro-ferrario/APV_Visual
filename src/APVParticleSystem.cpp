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
}

GoofyParticle* APVParticleSystem::addParticle(ofVec3f newPosition, float maxVelocity, long int life)
{
  GoofyParticle* particle = new GoofyParticle(newPosition, maxVelocity * percParticleSpeed, sameSpring, sameFriction);
  particle->target = newPosition;
  particles.push_back(particle);
  particle = NULL;
  return particle;
}
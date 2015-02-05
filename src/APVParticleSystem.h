//
//  APVParticleSystem.h
//  APV_Visual
//
//  Created by Mauro Ferrario on 05/02/15.
//
//

#ifndef __APV_Visual__APVParticleSystem__
#define __APV_Visual__APVParticleSystem__

#include "GoofyParticleSystem.h"

class APVParticleSystem: public GoofyParticleSystem
{
public:
  APVParticleSystem();
  void  setup();
  GoofyParticle* addParticle(ofVec3f newPosition, float maxVelocity, long int life);
private:
  float sameSpring;
  float sameFriction;
};

#endif /* defined(__APV_Visual__APVParticleSystem__) */

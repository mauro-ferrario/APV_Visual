//
//  APVParticle.cpp
//  APV_Visual
//
//  Created by Mauro Ferrario on 07/02/15.
//
//

#include "APVParticle.h"


APVParticle::APVParticle(ofVec3f position, float limitVelocity, ofColor color, float spring, float friction, long int life)
{
  init(position, limitVelocity, color, spring, friction, life);
  audioCoefficent = 0;
}
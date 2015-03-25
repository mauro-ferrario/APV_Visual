//
//  APVParticle.h
//  APV_Visual
//
//  Created by Mauro Ferrario on 07/02/15.
//
//

#ifndef __APV_Visual__APVParticle__
#define __APV_Visual__APVParticle__

#include "GoofyParticle.h"


class APVParticle: public GoofyParticle
{
public:
   APVParticle(ofVec3f position = ofVec3f(0,0), float limitVelocity = 10, ofColor color = 255, float spring = ofRandom(.01,.5), float friction = ofRandom(.01,.4), long int life = 0);
  float audioCoefficent;
};

#endif /* defined(__APV_Visual__APVParticle__) */

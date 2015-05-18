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
#include "APVParticle.h"



class APVVisual;

class APVParticleSystem: public GoofyParticleSystem
{
public:
                      APVParticleSystem();
  void                setup(APVVisual* visual);
  APVParticle*        addParticle(ofVec3f newPosition, float maxVelocity, long int life);
  void                updateAndDrawWithVisual();
  void                removePoints(bool removeNow);
  APVVisual*          visual;
  float               sameSpring;
  float               sameFriction;
  float               repulsionForce;
  float               sameLimitVelocity;
  void                lastActionInsideUpdateLoop(GoofyParticle* particle);
private:
  
  void                drawConnectPoints(vector<GoofyParticle*>::iterator vItr, vector<GoofyParticle*>::iterator pPointerIn);
  void                connectPrevPoint(vector<GoofyParticle*>::iterator vItr);
  void                drawTriangle(vector<GoofyParticle*>::iterator firstPoint, vector<GoofyParticle*>::iterator secondPoint, vector<GoofyParticle*>::iterator thirdPoint);
  float               getTrianglePerimeter(vector<GoofyParticle*>::iterator firstPoint, vector<GoofyParticle*>::iterator secondPoint, vector<GoofyParticle*>::iterator thirdPoint);
  void                loopIn(vector<GoofyParticle*>::iterator vItr, int cont);
};

#endif /* defined(__APV_Visual__APVParticleSystem__) */

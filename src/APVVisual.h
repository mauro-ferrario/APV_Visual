//
//  APVVisual.h
//  APV_Visual
//
//  Created by Mauro Ferrario on 05/02/15.
//
//

#ifndef __APV_Visual__APVVisual__
#define __APV_Visual__APVVisual__

#include "ofMain.h"
#include "APVParticleSystem.h"

class APVVisual
{
public:
            APVVisual();
  void      setup();
  void      draw();
  void      update();
  void      windowResized(int newWidth, int newHeight);
  APVParticleSystem particleSystem;
  GoofyParticle* addParticle(ofVec3f newPosition, float maxVelocity, long int life);
  
private:
  ofFbo     mainFbo;
  ofVec2f   size;
  void      allocateFBO(int width, int height);
  void      initParticleSystem();
};

#endif /* defined(__APV_Visual__APVVisual__) */

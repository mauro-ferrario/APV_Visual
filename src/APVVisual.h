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
  
  
  bool globalAlphaCoefficent;
  bool bDrawPoint;
  bool bDrawLinePointToPoint;
  bool bDrawTriangle;
  bool bConnectPointToPrev;
  
private:
  ofFbo     mainFbo;
  ofVec2f   size;
  void      allocateFBO(int width, int height);
  void      initParticleSystem();
  ofColor   backgroundColor;
  void      drawBackground();
  
  
  
  
  float minDistancePointToPoint;
  float maxDistancePointToPoint;
  float distancePointToPoint;
  
  float minTriangleCoefficent;
  float maxTriangleCoefficent;
  float triangleCoefficent;
  
  bool    bSameColorTriangle;
  bool bTimeAlphaTriangle;
  
  
  vector<float>           left;
  vector<float>           right;
  float                   smoothedVol;
  float                   scaledVol;
  int                     bufferCounter;
  
  int totPoints;
};

#endif /* defined(__APV_Visual__APVVisual__) */

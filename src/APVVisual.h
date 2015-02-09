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
#include "ofxOsc.h"
#include<tr1/unordered_map>

class APVVisual
{
public:
            APVVisual();
  void      setup();
  void      draw();
  void      update();
  void      windowResized(int newWidth, int newHeight);
  APVParticleSystem particleSystem;
  GoofyParticle* addParticle(ofVec3f newPosition, float maxVelocity, long int life = 0);
  void audioIn(float * input, int bufferSize, int nChannels);
  
  float globalAlphaCoefficent;
  bool bDrawPoint;
  bool bDrawLinePointToPoint;
  bool bDrawTriangle;
  bool bConnectPointToPrev;
  bool  bFrameHandler;
  vector<float>           left;
  vector<float>           right;
  float triangleCoefficent;
  tr1::unordered_map<string, bool* > mapToBoolValue;
  tr1::unordered_map<string, float* > mapToFloatValue;
  
  float minDistancePointToPoint;
  float maxDistancePointToPoint;
  float distancePointToPointCoefficent;
  
  
  float minTrianglePerimeter;
  float maxTrianglePerimeter;
  float trianglePerimeterCoefficent;
  float maxRepulsionForce;
  ofColor triangleColor;
  
private:
  ofFbo     mainFbo;
  ofVec2f   size;
  void      allocateFBO(int width, int height);
  void      initParticleSystem();
  ofColor   backgroundColor;
  void      drawBackground();
  ofxOscReceiver  receiver;
  void            initOSC();
  void            receiveMessagges();
  
  
  
  bool    bSameColorTriangle;
  bool bTimeAlphaTriangle;
  
  
  float                   smoothedVol;
  float                   scaledVol;
  int                     bufferCounter;
  float                   volume;
  float                   maxVolumeValue;
  
  int totPoints;
};

#endif /* defined(__APV_Visual__APVVisual__) */

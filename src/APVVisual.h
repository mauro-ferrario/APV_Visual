//
//  APVVisual.h
//  APV_Visual
//
//  Created by Mauro Ferrario on 05/02/15.
//
//

#ifndef __APV_Visual__APVVisual__
#define __APV_Visual__APVVisual__

#define     LIMIT_OUTISDE 1000
#define     USE_SYPHON

#include "ofMain.h"
#include "APVParticleSystem.h"
#include "ofxOsc.h"
#include<tr1/unordered_map>
#ifdef USE_SYPHON
#include "ofxSyphon.h"
#endif
#include "OverlayHandler.h"

class APVVisual
{
public:
                                      APVVisual();
  void                                setup(ofVec2f size);
  void                                draw();
  void                                update();
  void                                windowResized(int newWidth, int newHeight);
  APVParticleSystem*                  particleSystem;
  GoofyParticle*                      addParticle(ofVec3f newPosition, float maxVelocity, long int life = 0, bool fromOutside = false);
  void                                audioIn(float * input, int bufferSize, int nChannels, float beatValue);
  void                                exit();
  
  float                               globalAlphaCoefficent;
  bool                                bDrawPoint;
  bool                                bDrawLinePointToPoint;
  bool                                bDrawTriangle;
  bool                                bConnectPointToPrev;
  bool                                bFrameHandler;
  vector<float>                       left;
  vector<float>                       right;
  float triangleCoefficent;
  tr1::unordered_map<string, bool* >  mapToBoolValue;
  tr1::unordered_map<string, float* > mapToFloatValue;
  
  float                               minDistancePointToPoint;
  float                               maxDistancePointToPoint;
  float                               distancePointToPointCoefficent;
  
  float                               volumeInvertCoefficent;
  float                               volumeLevel;
  bool                                manualInvertColor;
  bool                                forceInvertColor;
  
  
  float                               minTrianglePerimeter;
  float                               maxTrianglePerimeter;
  float                               trianglePerimeterCoefficent;
  float                               maxRepulsionForce;
  float                               scaleFactor;
  float                               maxScaleFactor;
  ofColor                             triangleColor;
  
  int                                 totNewPointToDraw;
  int                                 totPointAlreadyDraw;
  int                                 totPrevPoint;
  bool                                invertColor;
  int                                 invertColorTimer;
  float                               volumeCoefficent;
#ifdef USE_SYPHON
  ofxSyphonServer                     individualTextureSyphonServer;
#endif
  OverlayHandler                      overlayHandler;
		ofSoundPlayer                     track;
  float                               currentTimePause;
  ofFbo                               mainFbo;
  ofVec2f                             size;
  void                                setupParticleSystem();
  virtual void                        initParticleSystem();
  bool                                bSameColorTriangle;
  
  
  ofFbo pointFBO;
  
  
  bool                                enablePixelShader;
  bool                                halfPixelShader;
  bool                                samePixelShaderSize;
  bool                                pixelShaderAllLine;
//  float                               pixelShaderSizeLimit;
  float                               pixelShaderWidth;
  float                               pixelShaderHeight;
  
protected:
  virtual void                        processMessage(ofxOscMessage& m, string messageAddress);
  ofxOscReceiver                      receiver;
  virtual void                        setupOSCPointers();
  virtual void                        setupPixelatePointers();
  virtual void                        cleanPointers();
  
private:
  void                                allocateFBO(int width, int height);
  ofColor                             backgroundColor;
  void                                drawBackground();
  void                                initOSC();
  virtual void                                receiveMessagges();
  
//  bool                                bTimeAlphaTriangle;

  
  float                               smoothedVol;
  float                               scaledVol;
  int                                 bufferCounter;
  float                               volume;
  float                               maxVolumeValue;
  int                                 totPoints;
  ofShader                            shader;
  
  virtual void                        addPointFromOSC(ofxOscMessage& m);
  virtual void                        addSinglePointFromOSC(ofxOscMessage& m);
  
  void                                setupMainOscPointers();
  void                                cleanMainOSCPointers();
  
  
};

#endif /* defined(__APV_Visual__APVVisual__) */

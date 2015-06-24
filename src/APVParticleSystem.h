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
  virtual void        updateAndDrawWithVisual();
  void                removePoints(bool removeNow);
  APVVisual*          visual;
  float               sameSpring;
  float               sameFriction;
  float               repulsionForce;
  float               sameLimitVelocity;
  void                lastActionInsideUpdateLoop(GoofyParticle* particle);
  ofFbo               triangleFBO;
  ofFbo               pointFBO;
  ofFbo               connectPrevPointFBO;
  ofFbo               connectPointsFBO;
  ofFbo               fakeFbo;
  bool                useDifferentFBO;
  
private:
  
  void                drawConnectPoints(vector<GoofyParticle*>::iterator vItr, vector<GoofyParticle*>::iterator pPointerIn, int cont, int cont2);
  void                connectPrevPoint(vector<GoofyParticle*>::iterator vItr, int cont);
  void                drawTriangle(vector<GoofyParticle*>::iterator firstPoint, vector<GoofyParticle*>::iterator secondPoint, vector<GoofyParticle*>::iterator thirdPoint, int cont1, int cont2, int cont3);
  void                loopIn(vector<GoofyParticle*>::iterator vItr, int cont);
  
  void                cleanFbos();
  void                initFBOs(ofVec2f size);
  void                cleanSingleFbo(ofFbo fbo);
  void                initSingleFBO(ofFbo& fbo, ofVec2f size);
  void                drawMeshIntoFbo(ofVboMesh& mesh, ofFbo& fbo, ofPrimitiveMode mode, vector<ofIndexType> indices, vector<ofFloatColor> colors);
  void                  cleanMesh();
  void                  drawMeshesIntoFBOs();
  
  ofVboMesh             mesh;
  ofVboMesh             triangleMesh;
  ofVboMesh             connectPointMesh;
  
  
  vector<ofIndexType>   pointIndices;
  vector<ofFloatColor>  pointIndicesColor;
  
  vector<ofIndexType>   connectPointIndices;
  vector<ofFloatColor>  connectPointIndicesColors;
  
  vector<ofIndexType>   triangleIndices;
  vector<ofFloatColor>  triangleIndicesColors;
  
  vector<ofIndexType>   connectPrevPointIndices;
  vector<ofFloatColor>  connectPrevPointColors;
};

#endif /* defined(__APV_Visual__APVParticleSystem__) */

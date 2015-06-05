//
//  APVParticleSystem.cpp
//  APV_Visual
//
//  Created by Mauro Ferrario on 05/02/15.
//
//

#include "APVParticleSystem.h"
#include "APVVisual.h"


APVParticleSystem::APVParticleSystem()
{
  
}

void APVParticleSystem::setup(APVVisual* _visual)
{
  this->visual = _visual;
  cout << this->visual  << endl;
  _visual = NULL;
  sameFriction = .83;
  sameSpring = .183;
  sameLimitVelocity = 100;
  repulsionForce = 1;
  
  useDifferentFBO = true;
  cout << "SETUP APV PARTICLE SYSTEM" << endl;
//  if(useDifferentFBO)
    initFBOs(visual->size);
  mesh.setMode(OF_PRIMITIVE_POINTS);
  glPointSize(3);
}

void APVParticleSystem::initFBOs(ofVec2f size)
{
//  initSingleFBO(fakeFbo, size);
  initSingleFBO(triangleFBO, size);
  initSingleFBO(pointFBO, size);
  initSingleFBO(connectPrevPointFBO, size);
  initSingleFBO(connectPointsFBO, size);
}

void APVParticleSystem::initSingleFBO(ofFbo& fbo, ofVec2f size)
{
  fbo.allocate(size.x, size.y, GL_RGB);
  fbo.begin();
  ofClear(0,255);
  fbo.end();
}

void APVParticleSystem::cleanFbos()
{
  cleanSingleFbo(triangleFBO);
  cleanSingleFbo(pointFBO);
  cleanSingleFbo(connectPrevPointFBO);
  cleanSingleFbo(connectPointsFBO);
}

void APVParticleSystem::cleanSingleFbo(ofFbo fbo)
{
  fbo.begin();
  ofClear(0,0);
  fbo.end();
}

APVParticle* APVParticleSystem::addParticle(ofVec3f newPosition, float maxVelocity, long int life)
{
  APVParticle* particle = new APVParticle(newPosition, maxVelocity * percParticleSpeed, ofColor(255), sameSpring, sameFriction);
  particle->target = newPosition;
  particles.push_back(particle);
  return particle;
}

void APVParticleSystem::lastActionInsideUpdateLoop(GoofyParticle* particle)
{
  if(followFlow)
  {
    if(particle->position.x < -100 )
      particle->position.x = visual->size.x + 50;
//      particle->active = false;
    else if(particle->position.x > visual->size.x + 100 )
      particle->position.x = -50;
//      particle->active = false;
    if(particle->position.y < -100 )
//      particle->active = false;
        particle->position.y = visual->size.y + 50;
    else if(particle->position.y > visual->size.y + 100)
//      particle->active = false;
        particle->position.y = -50;
  }
  else if(!followFlow&&applyWind)
  {
    if(particle->position.x < -100 )
      particle->active = false;
    else if(particle->position.x > visual->size.x + 100 )
       particle->active = false;
    if(particle->position.y < -100 )
      particle->active = false;
    else if(particle->position.y > visual->size.y + 100)
      particle->active = false;
  }
}

void APVParticleSystem::cleanMesh()
{
  pointIndices.clear();
  pointIndicesColor.clear();
  connectPointIndices.clear();
  connectPointIndicesColors.clear();
  triangleIndices.clear();
  triangleIndicesColors.clear();
  connectPrevPointIndices.clear();
  connectPrevPointColors.clear();
  mesh.clear();
}

void APVParticleSystem::updateAndDrawWithVisual()
{
  cleanFbos();
  cleanMesh();
  ofEnableAlphaBlending();
  if(moveNoise)
    goofyPerlinNoise.update();
  
  vector<GoofyParticle*>::iterator vItr = particles.begin();
  int cont = 0;
  int totParticles = particles.size();
  int halfTotParticles = totParticles*.5;
  ofVec2f center = ofVec2f(visual->size.x*.5, visual->size.y*.5);
  
  while ( vItr != particles.end() )
  {
    if(bFollowTarget)
      (*vItr)->followTarget();
    if(moveNoise)
      (*vItr)->moveWithNoise(goofyPerlinNoise, goofyPerlinNoiseForce * 20);;
    if(followFlow)
      (*vItr)->follow(goofyFlowField);
    if(applyWind)
      (*vItr)->addWind(wind);
    
    applyRepulsions((*vItr));
    applyAttraction((*vItr));
    
    (*vItr)->update();
    
    if (!(*vItr)->active)
    {
      delete * vItr;
      vItr = particles.erase( vItr );
    }
    else
    {
      APVParticle* tempParticle   = (APVParticle*)(*vItr);
      tempParticle->friction      = sameFriction;
      tempParticle->spring        = sameSpring;
      tempParticle->limitVelocity = sameLimitVelocity * 100;
      
      if((*vItr)->active)
      {
        if(cont < halfTotParticles)
        {
          if(visual->left[(int)ofMap(cont, 0, halfTotParticles, 0, 256, true)] > 0)
          {
            tempParticle->audioCoefficent = visual->left[(int)ofMap(cont, 0, halfTotParticles, 0, 256, true)];
            float radius = visual->size.x*.5;
            float force = tempParticle->audioCoefficent * visual->maxRepulsionForce * repulsionForce;
            float limitSpeed = false;
            GoofyMagneticPoint *repeller = new GoofyMagneticPoint(center, radius, force, limitSpeed);
            (*vItr)->applyRepulsion(repeller, false);
            delete repeller;
            repeller = NULL;
          }
        }
        else
        {
          if(visual->right[(int)ofMap(cont, halfTotParticles, totParticles - 1, 0, 256, true)] > 0)
          {
            tempParticle->audioCoefficent = visual->right[(int)ofMap(cont, halfTotParticles, totParticles - 1, 0, 256, true)];
            float radius = visual->size.x*.5;
            float force = tempParticle->audioCoefficent * visual->maxRepulsionForce * repulsionForce;
            float limitSpeed = false;
            GoofyMagneticPoint *repeller = new GoofyMagneticPoint(center, radius, force, limitSpeed);
            
            (*vItr)->applyRepulsion(repeller, false);
            delete repeller;
            repeller = NULL;
          }
        }
        mesh.addVertex((*vItr)->position);
        GoofyParticle* prevPrevParticle;
        if(cont > 0 && visual->bConnectPointToPrev)
            connectPrevPoint(vItr, cont);
        if(visual->bDrawLinePointToPoint || visual->bDrawTriangle)
        {
          loopIn(vItr, cont);
        }
        if(visual->bDrawPoint)
        {
          if(tempParticle)
          {
            float alpha = tempParticle->audioCoefficent * visual->globalAlphaCoefficent * 1000;
            tempParticle = NULL;
            ofColor pointColor;
            pointColor = ofColor(255);
            pointColor.a = alpha;
            pointIndices.push_back(cont);
            pointIndicesColor.push_back(pointColor);            
          }
        }
        cont++;
      }
      lastActionInsideUpdateLoop((*vItr));
      vItr++;
    }
  }
  drawMeshesIntoFBOs();
}

void APVParticleSystem::drawMeshesIntoFBOs()
{
  if(visual->bDrawTriangle)
    drawMeshIntoFbo(mesh, triangleFBO, OF_PRIMITIVE_TRIANGLES, triangleIndices, triangleIndicesColors);
  if(visual->bDrawPoint)
    drawMeshIntoFbo(mesh, pointFBO, OF_PRIMITIVE_POINTS, pointIndices, pointIndicesColor);
  if(visual->bConnectPointToPrev)
    drawMeshIntoFbo(mesh, connectPrevPointFBO, OF_PRIMITIVE_LINES, connectPrevPointIndices, connectPrevPointColors);
  if(visual->bDrawLinePointToPoint)
    drawMeshIntoFbo(mesh, connectPointsFBO, OF_PRIMITIVE_LINES, connectPointIndices, connectPointIndicesColors);
}


void APVParticleSystem::drawMeshIntoFbo(ofVboMesh& mesh, ofFbo& fbo, ofPrimitiveMode mode, vector<ofIndexType> indices, vector<ofFloatColor> colors)
{
  mesh.clearIndices();
  mesh.clearColors();
  mesh.addIndices(indices);
  mesh.addColors(colors);
  mesh.setMode(mode);
  fbo.begin();
  float scale = visual->maxScaleFactor * (visual->scaleFactor);
  ofPushMatrix();
  ofTranslate(visual->size.x*.5, visual->size.y*.5);
  ofScale(scale,scale);
  ofTranslate(-visual->size.x*.5, -visual->size.y*.5);
  ofTranslate(0, visual->size.y);
  ofScale(1,-1);
  
  if(indices.size()>0)
    mesh.draw();
  ofPopMatrix();
  fbo.end();
}

void APVParticleSystem::removePoints(bool removeNow)
{
  vector<GoofyParticle*>::iterator vItr = particles.begin();
  while ( vItr != particles.end() )
  {
    if(removeNow)
      (*vItr)->life = 0;
    else
      (*vItr)->life = ofRandom(0,100);
    (*vItr)->lifeActive = true;
    vItr++;
  }
}

void APVParticleSystem::loopIn(vector<GoofyParticle*>::iterator vItr, int cont)
{
  vector<GoofyParticle*>::iterator pPointerIn = vItr;
  ofPushStyle();
  int cont2 = cont;
  while ( pPointerIn != particles.end() )
  {
    if(cont > 2 && visual->bDrawTriangle)
      drawTriangle(vItr, pPointerIn -1, pPointerIn, cont, cont2-1, cont2);
    if(visual->bDrawLinePointToPoint)
      drawConnectPoints(vItr, pPointerIn, cont, cont2);
    pPointerIn++;
    cont2++;
  }
  ofPopStyle();
}

void APVParticleSystem::drawTriangle(vector<GoofyParticle*>::iterator firstPoint, vector<GoofyParticle*>::iterator secondPoint, vector<GoofyParticle*>::iterator thirdPoint, int cont1, int cont2, int cont3)
{
  float alpha;
  
  float dist1 =  abs(ofDist((*firstPoint)->position.x, (*firstPoint)->position.y, (*thirdPoint)->position.x, (*thirdPoint)->position.y));
  float dist2 = abs(ofDist((*firstPoint)->position.x, (*firstPoint)->position.y, (*secondPoint)->position.x, (*secondPoint)->position.y));
  float dist3 = abs(ofDist((*secondPoint)->position.x, (*secondPoint)->position.y, (*thirdPoint)->position.x, (*thirdPoint)->position.y));
  
  float perimeter = dist1+dist2+dist3;
  
  APVParticle* tempParticle = (APVParticle*)(*firstPoint);
  alpha = tempParticle->audioCoefficent * visual->globalAlphaCoefficent * 200;
  tempParticle = NULL;
  ofColor triangleColor = visual->triangleColor;
  
  if(alpha > 150)
  {
    alpha =150;
  }
  triangleColor.a = alpha;
  
  if(!visual->bSameColorTriangle)
  {
    if(perimeter < visual->size.x*.15)
      triangleColor = ofColor(255, 0, 0, alpha);
    else if(perimeter >= visual->size.x*.15 && perimeter < visual->size.x *.25)
      triangleColor = ofColor(0, 255, 0, alpha);
    else if(perimeter >= visual->size.x*.25 && perimeter <= visual->size.x*.35)
      triangleColor = ofColor(0, 0, 255, alpha);
    else
      triangleColor = ofColor(255, alpha);
  }
  else
  {
    if(alpha > 150)
      triangleColor = ofColor(255,alpha);
  }
  
  
  
  int maxDistancePoints = visual->size.y;
  
  if(perimeter > visual->minTrianglePerimeter * visual->triangleCoefficent*1000 && perimeter < visual->maxTrianglePerimeter * visual->triangleCoefficent*1000)
  {
    if(dist1 > maxDistancePoints || dist2 > maxDistancePoints || dist3>maxDistancePoints)
    {
    }
    else
    {
      triangleIndices.push_back(cont1);
      triangleIndices.push_back(cont2);
      triangleIndices.push_back(cont3);
      triangleIndicesColors.push_back(ofFloatColor(triangleColor));
      triangleIndicesColors.push_back(ofFloatColor(triangleColor));
      triangleIndicesColors.push_back(ofFloatColor(triangleColor));
    }
  }
}

void APVParticleSystem::connectPrevPoint(vector<GoofyParticle*>::iterator vItr, int cont)
{
  APVParticle* tempParticle = (APVParticle*)(*vItr);
  float alpha = tempParticle->audioCoefficent * visual->globalAlphaCoefficent * 255;
  tempParticle = NULL;
  vector<GoofyParticle*>::iterator prevParticle = vItr - 1;
  ofColor lineColor = ofColor(255,alpha);
  if((*prevParticle)->active)
  {
    ofLine((*vItr)->position, (*prevParticle)->position);
    connectPrevPointIndices.push_back(cont);
    connectPrevPointIndices.push_back(cont-1);
    connectPrevPointColors.push_back(lineColor);
    connectPrevPointColors.push_back(lineColor);
  }
}

void APVParticleSystem::drawConnectPoints(vector<GoofyParticle*>::iterator vItr, vector<GoofyParticle*>::iterator pPointerIn, int cont, int cont2)
{
  APVParticle* tempParticle = (APVParticle*)(*vItr);
  float alpha = tempParticle->audioCoefficent * visual->globalAlphaCoefficent * 200;
  tempParticle = NULL;
  float distance = (*vItr)->position.distance((*pPointerIn)->position);
  ofColor lineColor = ofColor(255,alpha);
  if(distance > visual->minDistancePointToPoint * visual->distancePointToPointCoefficent && distance < visual->maxDistancePointToPoint * visual->distancePointToPointCoefficent)
  {
    connectPointIndices.push_back(cont);
    connectPointIndices.push_back(cont2);
    connectPointIndicesColors.push_back(ofFloatColor(lineColor));
    connectPointIndicesColors.push_back(ofFloatColor(lineColor));
  }
}


  //
//  APVVisual.cpp
//  APV_Visual
//
//  Created by Mauro Ferrario on 05/02/15.
//
//

#include "APVVisual.h"

APVVisual::APVVisual()
{
  
}


void APVVisual::setup(ofVec2f size)
{
  initParticleSystem();
//  particleSystem = new APVParticleSystem();    
  cleanPointers();
  this->size = size;
  allocateFBO(size.x, size.y);
  backgroundColor = ofColor(0);
  ofSetCircleResolution(60);
  ofEnableAlphaBlending();
  globalAlphaCoefficent   = 0;
  bDrawPoint              = true;
  bDrawLinePointToPoint   = false;
  bDrawTriangle           = false;
  bConnectPointToPrev     = false;
  minDistancePointToPoint = 0;
  maxDistancePointToPoint = 1000;
  distancePointToPointCoefficent = 1000;
  minTrianglePerimeter    = 10;
  maxTrianglePerimeter    = 1000;
  trianglePerimeterCoefficent = 1000;
  triangleCoefficent      = .5;
  bSameColorTriangle      = true;
  bFrameHandler           = true;
  invertColor             = false;
  invertColorTimer        = 0;
  volumeInvertCoefficent  = 1.5;
  
  left.assign(1024, 0.0);
  right.assign(1024, 0.0);
  
  smoothedVol             = 0.0;
  scaledVol               = 0.0;
  bufferCounter           = 0;
  totPoints               = 0;
  maxVolumeValue          = 0;
  maxRepulsionForce       = 100;
  volumeLevel             = 1;
//  initParticleSystem();
  initOSC();
//  shader.load("invertedMask.vert", "invertedMask.frag");

#ifdef USE_SYPHON
  individualTextureSyphonServer.setName("APV_Visual");
#endif
  setupOSCPointers();
  
  totNewPointToDraw       = 0;
  totPointAlreadyDraw     = 0;
  totPrevPoint            = 0;
  maxScaleFactor          = 10;
  scaleFactor             = 1;
//  overlayHandler.setup(this);
  ofSleepMillis(1000);
//  track.loadSound("sounds/LPM_3.wav");
  track.setLoop(false);
  manualInvertColor = true;
  forceInvertColor = true;
  
  particleSystem->moveNoise = true;
  particleSystem->initGoofyNoise();
 // particleSystem.goofyPerlinNoiseForce = 2;

  particleSystem->initGoofyFlowField();  
}

void APVVisual::setupOSCPointers()
{
  setupMainOscPointers();
  setupPixelatePointers();
}

void APVVisual::setupPixelatePointers()
{
  mapToBoolValue["/Shaders/Pixel_Shader/Pixel_Effect"] = &enablePixelShader;
  mapToBoolValue["/Shaders/Pixel_Shader/Half_Pixel_Effect"] = &halfPixelShader;
  mapToBoolValue["/Shaders/Pixel_Shader/Same_Pixel_Size"] = &samePixelShaderSize;
//  mapToFloatValue["/Shaders/Pixel_Shader/Pixel_Size_Limit"] = &pixelShaderSizeLimit;
  mapToFloatValue["/Shaders/Pixel_Shader/Pixel_Width"] = &pixelShaderWidth;
  mapToFloatValue["/Shaders/Pixel_Shader/Pixel_Height"] = &pixelShaderHeight;
}

void APVVisual::setupMainOscPointers()
{
  mapToFloatValue["/Effect/Scale_Factor"] = &scaleFactor;
  mapToBoolValue["/Effect/Draw_point"] = &bDrawPoint;
  mapToBoolValue["/Effect/Draw_triangle"] = &bDrawTriangle;
  mapToBoolValue["/Effect/Connect_to_prev_point"] = &bConnectPointToPrev;
  mapToBoolValue["/Effect/Connect_points"] = &bDrawLinePointToPoint;
  mapToBoolValue["/Effect/Triangles/Same_Color_Triangles"] = &bSameColorTriangle;
  mapToBoolValue["/Movement/Wind/ApplyWind"] = &particleSystem->applyWind;
  mapToBoolValue["/General/Volume/Manual_Invert"] = &manualInvertColor;
  mapToBoolValue["/General/Volume/Force_Invert"] = &forceInvertColor;
  mapToBoolValue["/Movement/Follow_Flow"] =  &particleSystem->followFlow;
  
  mapToFloatValue["/Effect/Connect_Lines/Min_Line_Distance"] = &minDistancePointToPoint;
  mapToFloatValue["/Effect/Connect_Lines/Min_Line_Distance"] = &minDistancePointToPoint;
  mapToFloatValue["/Effect/Connect_Lines/Max_Line_Distance"] = &maxDistancePointToPoint;
  
  mapToFloatValue["/Effect/Triangles/Min_Perimeter"] = &minTrianglePerimeter;
  mapToFloatValue["/Effect/Triangles/Max_Perimeter"] = &maxTrianglePerimeter;
  
  mapToFloatValue["/Movement/Same_Spring"] = &particleSystem->sameSpring;
  mapToFloatValue["/Movement/Same_Friction"] =  &particleSystem->sameFriction;;
  mapToFloatValue["/Movement/Repulsion_Force"] =  &particleSystem->repulsionForce;;
  mapToFloatValue["/Movement/Particle_Speed"] =  &particleSystem->sameLimitVelocity;
  mapToFloatValue["/Effect/Triangles/Triangle_Limit"] =  &triangleCoefficent;
  
  
  mapToFloatValue["/General/Volume/Audio_Invert_Coefficent"] =  &volumeInvertCoefficent;
  mapToFloatValue["/General/Volume/Volume_Level"] =  &volumeLevel;
}

void APVVisual::initOSC()
{
  receiver.setup(6666);
}

void APVVisual::receiveMessagges()
{
  while( receiver.hasWaitingMessages() )
  {
    ofxOscMessage m;
    receiver.getNextMessage( &m );
    string messageAddress = m.getAddress();
//    if(messageAddress != "/General/Visual_Framerate")
      cout << messageAddress << endl;
    
    processMessage(m, messageAddress);
    
    if(mapToBoolValue[messageAddress])
    {
      *mapToBoolValue[messageAddress] = bool(m.getArgAsInt32(0));
    }
    if(mapToFloatValue[messageAddress])
    {
      *mapToFloatValue[messageAddress] = m.getArgAsFloat(0);
    }
  }
}

void APVVisual::processMessage(ofxOscMessage& m, string messageAddress)
{
  if ( messageAddress == "/Play" )
  {
    if(m.getArgAsInt32(0) == 0)
    {
      track.setPaused(true);
    }
    else
    {
      track.setPaused(false);
    }
  }
  if(messageAddress == "/Movement/Wind")
  {
    particleSystem->wind.x = m.getArgAsFloat(0);
    particleSystem->wind.y = m.getArgAsFloat(1);
  }
  if(messageAddress == "/Movement/Perlin/ResX")
  {
    particleSystem->goofyPerlinNoise.resY = m.getArgAsFloat(0);
  }
  if(messageAddress == "/Movement/Perlin/ResY")
  {
    particleSystem->goofyPerlinNoise.resY = m.getArgAsFloat(0);
  }
  if(messageAddress == "/Movement/Perlin/Speed")
  {
    particleSystem->goofyPerlinNoise.speed = m.getArgAsFloat(0);
  }
  if(messageAddress == "/Movement/Perlin/Force")
  {
    particleSystem->goofyPerlinNoiseForce = m.getArgAsFloat(0);
  }
  
  if(messageAddress == "/Flow/force")
  {
    particleSystem->goofyFlowField.force = m.getArgAsFloat(0);
  }
  /*
   if(messageAddress == "/FollowFlow")
   {
   particleSystem.followFlow = m.getArgAsInt32(0);
   }
   */
  if(messageAddress == "/enablePerlin")
  {
    particleSystem->moveNoise = m.getArgAsInt32(0);
  }
  if ( messageAddress == "/Reset_Track" )
  {
    track.stop();
    track.play();
  }
  if ( messageAddress == "/resetFlow" )
  {
    particleSystem->goofyFlowField.resetFlow();
  }
  if ( messageAddress == "/addSinglePoint" )
  {
    addSinglePointFromOSC(m);
  }
  else if ( messageAddress == "/clear" )
  {
    particleSystem->removePoints(false);
  }
  else if ( messageAddress == "/Effect/Triangles/Color" )
  {
    string color = m.getArgAsString(0);
    vector<string> colorChannels = ofSplitString(color, ",");
    triangleColor.r = ofToFloat(colorChannels[0])*255;
    triangleColor.g = ofToFloat(colorChannels[1])*255;
    triangleColor.b = ofToFloat(colorChannels[2])*255;
  }
  else if (messageAddress == "/loadShape" )
  {
    totNewPointToDraw = m.getArgAsInt32( 0 );
    totPointAlreadyDraw = 0;
    this->totPrevPoint = particleSystem->particles.size();
    if(this->totPrevPoint > totNewPointToDraw)
    {
      int totToRemove = this->totPrevPoint-totNewPointToDraw;
      for(int z = 0; z < totToRemove; z++)
      {
        particleSystem->particles[this->totPrevPoint - 1 - z]->target.x = NULL;
        particleSystem->particles[this->totPrevPoint - 1 - z]->lifeActive = true;
        particleSystem->particles[this->totPrevPoint - 1 - z]->life = 10;
      }
    }
  }
  else if ( messageAddress == "/addPoint" )
  {
    addPointFromOSC(m);
  }
  else if(messageAddress == "/FadeIn")
  {
    overlayHandler.startIntro(m.getArgAsInt32(0));
  }
  else if(messageAddress == "/FadeOut")
  {
    overlayHandler.startOutro(m.getArgAsInt32(0));
  }
}

void APVVisual::addSinglePointFromOSC(ofxOscMessage& m)
{
  ofVec3f position = ofVec3f(m.getArgAsFloat( 0 ) * size.x, m.getArgAsFloat( 1 ) * size.y);
  addParticle(position, particleSystem->sameLimitVelocity);
}

void APVVisual::addPointFromOSC(ofxOscMessage& m)
{
  float pointToDrawNow = m.getArgAsFloat( 0 );
  if(totPrevPoint > 0 && totPrevPoint >= this->totPointAlreadyDraw)
  {
    int cont = 0;
    for(int a = totPointAlreadyDraw; a < this->totNewPointToDraw; a++)
    {
      if(cont <= pointToDrawNow)
      {
        if(a < this->totPrevPoint)
        {
          ofPoint tempPoint;
          tempPoint.x = m.getArgAsFloat( 1 + (cont * 2)) * size.x;
          tempPoint.y = m.getArgAsFloat( (1 + (cont * 2) + 1)) * size.y;
          particleSystem->particles[a]->setTarget(tempPoint);
          this->totPointAlreadyDraw++;
        }
        else
        {
          this->totPointAlreadyDraw++;
          ofVec3f targetPos;
          targetPos.x = m.getArgAsFloat( 1 + (cont * 2) ) * size.x;
          targetPos.y = m.getArgAsFloat( (1 + (cont * 2) + 1) ) * size.y;
          addParticle(targetPos, particleSystem->sameLimitVelocity, 0, true);
        }
        cont++;
      }
    }
  }
  else
  {
    for(int a = 0; a < pointToDrawNow; a++)
    {
      this->totPointAlreadyDraw++;
      ofVec3f targetPos = ofVec3f(0);
      targetPos.x = m.getArgAsFloat( 1 + (a * 2)) * size.x;
      targetPos.y = m.getArgAsFloat( 1 + (a * 2) + 1) * size.y;
      addParticle(targetPos, particleSystem->sameLimitVelocity, 0, true);
    }
  }
}

void APVVisual::audioIn(float * input, int bufferSize, int nChannels, float beatValue)
{
  if(beatValue > 1)
    beatValue = 1;
  if((beatValue > .95 * volumeInvertCoefficent && !invertColor && !manualInvertColor) || forceInvertColor)
  {
    invertColor = true;
    invertColorTimer = 0;
  }
  if(invertColor)
  {
    invertColorTimer++;
    if(invertColorTimer > 10)
    {
      invertColor = false;
    }
  }
  float curVol = 0.0;
  int numCounted = 0;
  
//  bufferSize = 512; // Perchè se lo tolgo non va?

    for (int i = 0; i < bufferSize; i++)
    {
      //    cout << "bufferSize = " << bufferSize << endl;
      //    cout << "LEFT SIZE = " << left.size() << endl;
      left[i]		= input[ i * 2 ] * 0.5 * volumeLevel;
      right[i]	= input[ i * 2 +1 ] * 0.5 * volumeLevel;
      curVol += left[i] * left[i];
      curVol += right[i] * right[i];
      numCounted+=2;
    }
  
  curVol /= (float)numCounted;
  curVol = sqrt( curVol );
  smoothedVol *= 0.93;
  smoothedVol += 0.07 * curVol;
  volume = smoothedVol;
  if(volume > maxVolumeValue)
  {
    maxVolumeValue = volume;
  }
  else
  {
    maxVolumeValue -= .001;
  }
  //volume * 1.2;
  if(volume < .05)
    globalAlphaCoefficent = ofMap(volume, 0, .4, 0, 1);
  else
    globalAlphaCoefficent = 1;
  bufferCounter++;
}

void APVVisual::initParticleSystem()
{
  delete particleSystem;
  particleSystem = NULL;
  particleSystem = new APVParticleSystem();
}

void APVVisual::setupParticleSystem()
{
//  particleSystem.init();f
  cout << "SETUP PARTICLE SYSTEM" << endl;
  particleSystem->setup(this);
}

void APVVisual::allocateFBO(int width, int height)
{
  mainFbo.allocate(width, height, GL_RGB);
  mainFbo.begin();
  ofClear(255,0,0,255);
  mainFbo.end();
}

void APVVisual::update()
{
  receiveMessagges();
  
  
//  if(ofGetFrameRate() < 50)
//  {
//    triangleCoefficent -= .005;
//  }
//  else
//  {
//    triangleCoefficent += .005;
//  }
  
//  if(particleSystem.useDifferentFBO)
  if(true)
  {
    particleSystem->updateAndDrawWithVisual();
  }
  else
  {
    mainFbo.begin();
   // ofDisableAlphaBlending();
    ofClear(0,0);
    ofPushStyle();
    if(invertColor)
      ofSetColor(255);
    else
      ofSetColor(0);
    ofRect(0,0,size.x, size.y);
    ofPopStyle();
    float scale = maxScaleFactor * (1-scaleFactor);
    ofPushMatrix();;
    particleSystem->updateAndDrawWithVisual();
    ofPopMatrix();
//    overlayHandler.draw();
    mainFbo.end();
  }
}

void APVVisual::drawBackground()
{
  ofPushStyle();
  ofSetColor(backgroundColor);
  ofRect(0,0,size.x, size.y);
  ofPopStyle();
}

void APVVisual::draw()
{
#ifndef USE_SYPHON
  ofPushStyle();
  ofSetColor(255);
  //shader.begin();
  mainFbo.draw(0,0, size.x, size.y);
  //shader.end();
  ofPopStyle();
#endif
#ifdef USE_SYPHON
  individualTextureSyphonServer.publishTexture(&mainFbo.getTextureReference());
  ofDrawBitmapString("Syphon active", ofPoint(10,60));
#endif
  ofPushStyle();
  ofEnableAlphaBlending();
//  particleSystem.goofyFlowField.draw();
  ofPopStyle();
}

GoofyParticle* APVVisual::addParticle(ofVec3f newPosition, float maxVelocity, long int life, bool fromOutside)
{
  APVParticle* tempParticle = particleSystem->addParticle(newPosition, maxVelocity, life);
  if(fromOutside)
  {
    ofVec2f newPoint;
    if(ofRandom(5) > 2.5)
      newPoint.x = size.x + ofRandom(0, LIMIT_OUTISDE * .5);
    else
      newPoint.x = -ofRandom(0, LIMIT_OUTISDE * .5);
    if(ofRandom(5) > 2.5)
      newPoint.y =  size.y + ofRandom(0, LIMIT_OUTISDE * .5);
    else
      newPoint.y =  -ofRandom(0, LIMIT_OUTISDE * .5);
    tempParticle->position = newPoint;
  }
  tempParticle = NULL;
}

void APVVisual::windowResized(int newWidth, int newHeight)
{
  allocateFBO(newWidth, newHeight);
}

void APVVisual::cleanPointers()
{
  cout << "CLEAR ALL" << endl;
  for (auto& x: mapToFloatValue)
    mapToFloatValue[x.first] = NULL;
  for (auto& x: mapToBoolValue)
    mapToBoolValue[x.first] = NULL;
  //cleanMainOSCPointers();
}

void APVVisual::cleanMainOSCPointers()
{
  mapToFloatValue["/Effect/Scale_Factor"] = NULL;
  mapToBoolValue["/Effect/Draw_point"] = NULL;
  mapToBoolValue["/Effect/Draw_triangle"] = NULL;
  mapToBoolValue["/Effect/Connect_to_prev_point"] = NULL;
  mapToBoolValue["/Effect/Connect_points"] = NULL;
  mapToBoolValue["/Effect/Triangles/Same_Color_Triangles"] = NULL;
  mapToBoolValue["/Movement/Wind/ApplyWind"] = NULL;
  mapToBoolValue["/General/Volume/Manual_Invert"] = NULL;
  mapToBoolValue["/General/Volume/Force_Invert"] = NULL;
  mapToBoolValue["/Movement/Follow_Flow"] =  NULL;
  
  mapToFloatValue["/Effect/Connect_Lines/Min_Line_Distance"] = NULL;
  mapToFloatValue["/Effect/Connect_Lines/Min_Line_Distance"] = NULL;
  mapToFloatValue["/Effect/Connect_Lines/Max_Line_Distance"] = NULL;
  
  mapToFloatValue["/Effect/Triangles/Min_Perimeter"] = NULL;
  mapToFloatValue["/Effect/Triangles/Max_Perimeter"] = NULL;
  
  mapToFloatValue["/Movement/Same_Spring"] = NULL;
  mapToFloatValue["/Movement/Same_Friction"] =  NULL;
  mapToFloatValue["/Movement/Repulsion_Force"] =  NULL;
  mapToFloatValue["/Movement/Particle_Speed"] =  NULL;
  mapToFloatValue["/Effect/Triangles/Triangle_Limit"] =  NULL;
  
  
  mapToFloatValue["/General/Volume/Audio_Invert_Coefficent"] =  NULL;
  mapToFloatValue["/General/Volume/Volume_Level"] = NULL;
}

void APVVisual::exit()
{
  cleanPointers();
}
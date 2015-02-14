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


void APVVisual::setup()
{
  cleanPointers();
  size = ofVec2f(ofGetWindowWidth(), ofGetWindowHeight());
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
  bTimeAlphaTriangle      = true;
  bFrameHandler           = true;
  invertColor             = false;
  invertColorTimer        = 0;
  volumeInvertCoefficent  = 1.5;
  
  left.assign(256, 0.0);
  right.assign(256, 0.0);
  
  smoothedVol     = 0.0;
  scaledVol		= 0.0;
  bufferCounter	= 0;
  totPoints = 0;
  maxVolumeValue = 0;
  maxRepulsionForce = 100;
  initParticleSystem();
  initOSC();

#ifdef USE_SYPHON
  individualTextureSyphonServer.setName("APV_Visual");
#endif
  
  mapToFloatValue["/Effect/Scale_Factor"] = &scaleFactor;
  mapToBoolValue["/Effect/Draw_point"] = &bDrawPoint;
  mapToBoolValue["/Effect/Draw_triangle"] = &bDrawTriangle;
  mapToBoolValue["/Effect/Connect_to_prev_point"] = &bConnectPointToPrev;
  mapToBoolValue["/Effect/Connect_points"] = &bDrawLinePointToPoint;
  mapToBoolValue["/Effect/Triangles/Same_Color_Triangles"] = &bSameColorTriangle;
  
  mapToFloatValue["/Effect/Connect_Lines/Min_Line_Distance"] = &minDistancePointToPoint;
  mapToFloatValue["/Effect/Connect_Lines/Max_Line_Distance"] = &maxDistancePointToPoint;
  
  mapToFloatValue["/Effect/Triangles/Min_Perimeter"] = &minTrianglePerimeter;
  mapToFloatValue["/Effect/Triangles/Max_Perimeter"] = &maxTrianglePerimeter;
  
  mapToFloatValue["/Movement/Same_Spring"] = &particleSystem.sameSpring;
  mapToFloatValue["/Movement/Same_Friction"] =  &particleSystem.sameFriction;;
  mapToFloatValue["/Movement/Repulsion_Force"] =  &particleSystem.repulsionForce;;
  mapToFloatValue["/Movement/Particle_Speed"] =  &particleSystem.sameLimitVelocity;
  
  mapToFloatValue["/General/Audio_Invert_Coefficent"] =  &volumeInvertCoefficent;
  
  totNewPointToDraw = 0;
  totPointAlreadyDraw = 0;
  totPrevPoint = 0;
  maxScaleFactor = 10;
  scaleFactor = 1;
  
  
  track.loadSound("sounds/LPM_3.wav");
  track.setLoop(false);
//  track.play();
  //soundStream.
  
  overlayHandler.setup(this);
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
    //cout << messageAddress << endl;
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
    if ( messageAddress == "/Reset_Track" )
    {
      track.stop();
      track.play();
    }
    if ( messageAddress == "/addSinglePoint" )
    {
      ofVec3f position = ofVec3f(m.getArgAsFloat( 0 ) * ofGetWindowWidth(), m.getArgAsFloat( 1 ) * ofGetWindowHeight());
      addParticle(position, particleSystem.sameLimitVelocity);
    }
    else if ( messageAddress == "/clear" )
    {
      particleSystem.removePoints(false);
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
      int totPointBefore = particleSystem.particles.size();;
      totNewPointToDraw = m.getArgAsInt32( 0 );
      totPointAlreadyDraw = 0;
      totPrevPoint = totPointBefore;
      
      if(totPrevPoint - m.getArgAsInt32( 0 ) > 0)
      {
        for(int a = totNewPointToDraw; a < totPrevPoint; a++)
        {
          particleSystem.particles[a]->target.x = NULL;
          particleSystem.particles[a]->lifeActive = true;
          particleSystem.particles[a]->life = 10;
          ofVec2f center = ofVec2f(ofGetWindowWidth()*.5, ofGetWindowHeight()*.5);
          float radius = ofGetWindowWidth()*.5;
          float force = 500;
          float limitSpeed = false;
          GoofyMagneticPoint *repeller = new GoofyMagneticPoint(center, radius, force, limitSpeed);
          particleSystem.particles[a]->applyRepulsion(repeller, false);
          delete repeller;
          repeller = NULL;
        }
      }
    }
    else if ( messageAddress == "/addPoint" )
    {
      float pointToDrawNow = m.getArgAsFloat( 0 );
      int totPointAlreadyDraw = this->totPointAlreadyDraw;
      //totPointAlreadyDraw = 0;
      int totPrevPoint = particleSystem.particles.size(); // this->totPrevPoint;
      if(totPrevPoint > 0 && totPrevPoint > totPointAlreadyDraw)
      {
        int cont = 0;
        for(int a = totPointAlreadyDraw; a < pointToDrawNow; a++)
        {
          if(cont <= pointToDrawNow)
          {
            if(a < totPrevPoint)
            {
              ofPoint tempPoint;
              tempPoint.x = m.getArgAsFloat( 1 + (cont * 2)) * ofGetWindowWidth();
              tempPoint.y = m.getArgAsFloat( (1 + (cont * 2) + 1)) * ofGetWindowHeight();
              particleSystem.particles[a]->setTarget(tempPoint);
              this->totPointAlreadyDraw++;
            }
            else
            {
              this->totPointAlreadyDraw++;
              ofVec3f targetPos;
              targetPos.x = m.getArgAsFloat( 1 + (cont * 2) ) * ofGetWindowWidth();
              targetPos.y = m.getArgAsFloat( (1 + (cont * 2) + 1) ) * ofGetWindowHeight();
              addParticle(targetPos, particleSystem.sameLimitVelocity, 0, true);
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
          targetPos.x = m.getArgAsFloat( 1 + (a * 2)) * ofGetWindowWidth();
          targetPos.y = m.getArgAsFloat( 1 + (a * 2) + 1) * ofGetWindowHeight();
          addParticle(targetPos, particleSystem.sameLimitVelocity, 0, true);
        }
      }
    }
    else if(messageAddress == "/FadeIn")
    {
      overlayHandler.startIntro(m.getArgAsInt32(0));
    }
    else if(messageAddress == "/FadeOut")
    {
      overlayHandler.startOutro(m.getArgAsInt32(0));
    }
    
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

void APVVisual::audioIn(float * input, int bufferSize, int nChannels, float beatValue)
{
  
    if(beatValue > .95 * volumeInvertCoefficent && !invertColor)
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
  
  // samples are "interleaved"
  int numCounted = 0;
  
  //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
  for (int i = 0; i < bufferSize; i++){
    left[i]		= input[ i * 2 ] * 0.5;
    right[i]	= input[ i * 2 +1 ] * 0.5;
    
    curVol += left[i] * left[i];
    curVol += right[i] * right[i];
    numCounted+=2;
    
  }
  
  //this is how we get the mean of rms :)
  curVol /= (float)numCounted;
  
  // this is how we get the root of rms :)
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
  
  if(volume < .05)
    globalAlphaCoefficent = ofMap(volume, 0, .4, 0, 1);
  else
    globalAlphaCoefficent = 1;
  bufferCounter++;
}

void APVVisual::initParticleSystem()
{
  particleSystem.init();
  particleSystem.setup(this);
}

void APVVisual::allocateFBO(int width, int height)
{
  mainFbo.allocate(width, height, GL_RGBA32F);
  mainFbo.begin();
  ofClear(0,255);
  mainFbo.end();
}

void APVVisual::update()
{
  volumeCoefficent = ofMap(ofGetMouseX(), 0, 1920, 0,1, true);
  receiveMessagges();
  if(ofGetFrameRate() < 50)
  {
    triangleCoefficent -= .005;
  }
  else
  {
    triangleCoefficent += .005;
  }
  mainFbo.begin();
  ofDisableAlphaBlending();
  ofClear(0,0);
  ofPushStyle();
  if(invertColor)
    ofSetColor(255);
  else
    ofSetColor(0);
  ofRect(0,0,size.x, size.y);
  ofPopStyle();
  float scale = maxScaleFactor * (1-scaleFactor);
  ofPushMatrix();
  ofTranslate(1920*.5, 1080*.5);
  ofScale(scale,scale);
  ofTranslate(-1920*.5, -1080*.5);
  particleSystem.updateAndDrawWithVisual();
  ofPopMatrix();
  overlayHandler.draw();
  mainFbo.end();
  ofSetWindowTitle(ofToString(ofGetFrameRate()));
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
  mainFbo.draw(0,0);
  ofPopStyle();
#endif
#ifdef USE_SYPHON
  individualTextureSyphonServer.publishTexture(&mainFbo.getTextureReference());
  ofDrawBitmapString("Syphon active", ofPoint(10,10)):
#endif
}

GoofyParticle* APVVisual::addParticle(ofVec3f newPosition, float maxVelocity, long int life, bool fromOutside)
{
  APVParticle* tempParticle = particleSystem.addParticle(newPosition, maxVelocity, life);
  if(fromOutside)
  {
    ofVec2f newPoint;
    if(ofRandom(5) > 2.5)
      newPoint.x = ofGetWindowWidth() + ofRandom(0, LIMIT_OUTISDE * .5);
    else
      newPoint.x = -ofRandom(0, LIMIT_OUTISDE * .5);
    if(ofRandom(5) > 2.5)
      newPoint.y =  ofGetWindowHeight() + ofRandom(0, LIMIT_OUTISDE * .5);
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
  mapToFloatValue["/Effect/Scale_Factor"] = NULL;;
  mapToBoolValue["/Effect/Draw_point"] = NULL;
  mapToBoolValue["/Effect/Draw_triangle"] = NULL;
  mapToBoolValue["/Effect/Connect_to_prev_point"] = NULL;
  mapToBoolValue["/Effect/Connect_points"] = NULL;
  mapToBoolValue["/Effect/Triangles/Same_Color_Triangles"] = NULL;
  mapToFloatValue["/Effect/Connect_Lines/Min_Line_Distance"] = NULL;
  mapToFloatValue["/Effect/Connect_Lines/Max_Line_Distance"] = NULL;
  mapToFloatValue["/Effect/Triangles/Min_Perimeter"] = NULL;
  mapToFloatValue["/Effect/Triangles/Max_Perimeter"] = NULL;
  mapToFloatValue["/Movement/Same_Spring"] = NULL;
  mapToFloatValue["/Movement/Same_Friction"] =  NULL;
  mapToFloatValue["/Movement/Repulsion_Force"] =  NULL;
  mapToFloatValue["/Movement/Particle_Speed"] =  NULL;
  mapToFloatValue["/General/Audio_Invert_Coefficent"] =  &volumeInvertCoefficent;
}

void APVVisual::exit()
{
  cleanPointers();
}
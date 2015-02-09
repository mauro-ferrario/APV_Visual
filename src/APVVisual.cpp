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
  mapToFloatValue["/Movement/Particle_Speed"] =  &particleSystem.sameLimitVelocity;;
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
    cout << messageAddress << endl;
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
      cout << color << endl;
      triangleColor.r = ofToFloat(colorChannels[0])*255;
      triangleColor.g = ofToFloat(colorChannels[1])*255;
      triangleColor.b = ofToFloat(colorChannels[2])*255;
      
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

void APVVisual::audioIn(float * input, int bufferSize, int nChannels)
{
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
  
  //   globalAlphaCoefficent = 1;
  
  bufferCounter++;
  
 //cout << volume << endl;
}

void APVVisual::initParticleSystem()
{
  particleSystem.init();
  particleSystem.setup(this);
//  particleSystem.initGoofyNoise();
//  particleSystem.moveNoise = true;
//  particleSystem.goofyPerlinNoiseForce = .02;
//  particleSystem.goofyPerlinNoiseForce = .15;
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
  ofClear(0,255);
  particleSystem.updateAndDrawWithVisual();
  mainFbo.end();
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
  ofPushStyle();
  ofSetColor(255);
  mainFbo.draw(0,0);
  ofPopStyle();
}

GoofyParticle* APVVisual::addParticle(ofVec3f newPosition, float maxVelocity, long int life)
{
  particleSystem.addParticle(newPosition, maxVelocity, life);
}

void APVVisual::windowResized(int newWidth, int newHeight)
{
  allocateFBO(newWidth, newHeight);
}
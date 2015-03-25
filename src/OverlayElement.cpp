//
//  OverlayElement.cpp
//  APV_Visual
//
//  Created by Mauro Ferrario on 12/02/15.
//
//

#include "OverlayElement.h"


OverlayElement::OverlayElement()
{
  
}

OverlayElement::OverlayElement(float introDuration, float introDelay, float outroDuration, float outroDelay, ofVec2f position)
{
  this->introDuration = introDuration;
  this->introDelay    = introDelay;
  this->outroDuration = outroDuration;
  this->outroDelay    = outroDelay;
  this->position      = position;
  alpha               = 0;
}

void OverlayElement::update()
{
  alpha = tweenlinear.update();
}

void OverlayElement::draw(ofColor color)
{
  ofPushStyle();
  color.a = alpha;
  ofSetColor(color);
  ofPushMatrix();
  ofTranslate(position);
  draw();
  ofPopMatrix();
  ofPopStyle();
}

void OverlayElement::draw()
{
}

void OverlayElement::startAnimation(OverlayElementType type)
{
  if(type == OVERLAY_ELEMENT_TYPE_INTRO)
  {
    tweenlinear.setParameters(0,easinglinear,ofxTween::easeOut,0, 255,introDuration*1000,introDelay*1000);
  }
  else
  {
    tweenlinear.setParameters(0,easinglinear,ofxTween::easeOut,255, 0,outroDuration*1000,outroDelay*1000);
  }
  this->type = type;
}
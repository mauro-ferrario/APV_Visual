//
//  OverlayImageElement.cpp
//  APV_Visual
//
//  Created by Mauro Ferrario on 14/02/15.
//
//

#include "OverlayImageElement.h"


OverlayImageElement::OverlayImageElement()
{
  
}

OverlayImageElement::OverlayImageElement(float introDuration, float introDelay, float outroDuration, float outroDelay, string url, ofVec2f position)
{
  this->introDuration = introDuration;
  this->introDelay    = introDelay;
  this->outroDuration = outroDuration;
  this->outroDelay    = outroDelay;
  this->position      = position;
  alpha               = 0;
  image.loadImage(url);
}

void OverlayImageElement::draw()
{
  ofPushStyle();
  ofSetColor(255,alpha);
  image.draw(0,0);
  ofPopStyle();
}
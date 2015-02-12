//
//  OverlayTextElement.cpp
//  APV_Visual
//
//  Created by Mauro Ferrario on 12/02/15.
//
//

#include "OverlayTextElement.h"


OverlayTextElement::OverlayTextElement()
{
  
}

OverlayTextElement::OverlayTextElement(float introDuration, float introDelay, float outroDuration, float outroDelay, string text, ofTrueTypeFont* font, ofVec2f position)
{
  this->introDuration = introDuration;
  this->introDelay    = introDelay;
  this->outroDuration = outroDuration;
  this->outroDelay    = outroDelay;
  this->position      = position;
  this->text          = text;
  alpha               = 0;
  initTexts(font);
}

void OverlayTextElement::initTexts(ofTrueTypeFont* font)
{
  this->font = font;
}

void OverlayTextElement::draw()
{
  font->drawString(text, 0, 0);
}
//
//  OverlayText.cpp
//  APV_Visual
//
//  Created by Mauro Ferrario on 12/02/15.
//
//

#include "OverlayHandler.h"
#include "APVVisual.h"

OverlayHandler::OverlayHandler()
{
  
}

void OverlayHandler::setup(APVVisual* visual)
{
  this->visual = visual;
  initFont();
  elements.clear();
  initElements();
}

void OverlayHandler::initFont()
{
  ofTrueTypeFont::setGlobalDpi(72);
  font.loadFont("MuseoSans_500.otf", 60, true, true);
  font.setLineHeight(13.0f);
  font.setLetterSpacing(1.5);
}

void OverlayHandler::draw()
{
  ofEnableAlphaBlending();
  vector<OverlayElement*>::iterator it = elements.begin();
  while(it != elements.end())
  {
    (*it)->update();
    if(!visual->invertColor)
      (*it)->draw(ofColor(255));
    else
      (*it)->draw(ofColor(0));
    it++;
  }
  ofDisableAlphaBlending();
}

void OverlayHandler::startIntro(int element)
{
  startAnimation(element, OVERLAY_ELEMENT_TYPE_INTRO);
}

void OverlayHandler::startOutro(int element)
{
  startAnimation(element, OVERLAY_ELEMENT_TYPE_OUTRO);
}

void OverlayHandler::startAnimation(int element, OverlayElementType type)
{
  elements[element]->startAnimation(type);
}

void OverlayHandler::initElements()
{
  OverlayTextElement* textElement = new OverlayTextElement(0,0,0,0, "AREAS", &font, ofVec2f(1116, 350));
  OverlayTextElement* textElement1 = new OverlayTextElement(0,0,0,0, "ARTISTS", &font, ofVec2f(1415, 350));
  OverlayTextElement* textElement2 = new OverlayTextElement(0,0,0,0, "COUNTRIES", &font, ofVec2f(1240, 350));
  OverlayTextElement* textElement3 = new OverlayTextElement(0,0,0,0, "WORLDWIDE", &font, ofVec2f(1240, 350));
  OverlayTextElement* textElement4 = new OverlayTextElement(0,0,0,0, "EVENTS", &font, ofVec2f(1469, 440));
  addElement(textElement);
  addElement(textElement1);
  addElement(textElement2);
  addElement(textElement3);
  addElement(textElement4);
  textElement = NULL;
  textElement1 = NULL;
  textElement2 = NULL;
  textElement3 = NULL;
  textElement4 = NULL;
  
  
  OverlayImageElement* imageElement1 = new OverlayImageElement(1,0,1,0, "images/gg.png", ofVec2f(400, 350));
  addElement(imageElement1);
  imageElement1 = NULL;
}

void OverlayHandler::addElement(OverlayElement* element)
{
  elements.push_back(element);
  element = NULL;
}
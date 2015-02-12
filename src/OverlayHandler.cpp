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
  font.setLineHeight(18.0f);
  font.setLetterSpacing(1.037);
}

void OverlayHandler::draw()
{
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
  OverlayTextElement* textElement = new OverlayTextElement(.5,0,.5,0, "areas", &font, ofVec2f(1116, 350));
  OverlayTextElement* textElement1 = new OverlayTextElement(.5,0,.5,0, "artists", &font, ofVec2f(1415, 350));
  OverlayTextElement* textElement2 = new OverlayTextElement(.5,0,.5,0, "countries", &font, ofVec2f(1240, 350));
  OverlayTextElement* textElement3 = new OverlayTextElement(.5,0,.5,0, "WorldWide Events", &font, ofVec2f(1240, 350));
  addElement(textElement);
  addElement(textElement1);
  addElement(textElement2);
  addElement(textElement3);
  textElement = NULL;
  textElement1 = NULL;
  textElement2 = NULL;
  textElement3 = NULL;
}

void OverlayHandler::addElement(OverlayElement* element)
{
  elements.push_back(element);
  element = NULL;
}
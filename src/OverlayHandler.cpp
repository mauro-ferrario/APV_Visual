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
  //initElements();
}

void OverlayHandler::initFont()
{
  ofTrueTypeFont::setGlobalDpi(72);
  font.loadFont("MuseoSans_500.otf", 55, true, true);
  font.setLineHeight(13.0f);
  font.setLetterSpacing(2);
  
  font45.loadFont("MuseoSans_500.otf", 45, true, true);
  font45.setLineHeight(13.0f);
  font45.setLetterSpacing(2);
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
  if(elements[element])
    elements[element]->startAnimation(type);
}

void OverlayHandler::initElements()
{
  OverlayImageElement* textElement = new OverlayImageElement(.3,0,.3,0, "images/testo1.png", ofVec2f(1125, 300));
  OverlayImageElement* textElement1 = new OverlayImageElement(.3,0,.3,0, "images/testo2.png", ofVec2f(1415, 300));
  OverlayImageElement* textElement2 = new OverlayImageElement(.3,0,.3,0, "images/testo3.png", ofVec2f(1240, 300));
  OverlayImageElement* textElement3 = new OverlayImageElement(.3,0,.3,0, "images/testo4.png", ofVec2f(1260, 300));
  OverlayImageElement* textElement4 = new OverlayImageElement(.3,0,.3,0, "images/testo5.png", ofVec2f(1458, 390));
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
  
  OverlayImageElement* textElement5 = new OverlayImageElement(.3,0,.3,0, "images/testo6.png", ofVec2f(702, 900));
  addElement(textElement5);
  textElement5 = NULL;
  
  OverlayImageElement* imageDate = new OverlayImageElement(.5,0,.5,0, "images/date.png", ofVec2f(0, 0));
  addElement(imageDate);
  imageDate = NULL;
  OverlayImageElement* imageUniversita = new OverlayImageElement(.5,0,.5,0, "images/universita.png", ofVec2f(0, 0));
  addElement(imageUniversita);
  imageUniversita = NULL;
  
  OverlayImageElement* link = new OverlayImageElement(.5,0,.5,0, "images/testo7.png", ofVec2f(0, 0));
  addElement(link);
  link = NULL;
  
  OverlayImageElement* imageElement2 = new OverlayImageElement(.5,0,.5,0, "images/partners1.png", ofVec2f(0, 0));
  addElement(imageElement2);
  imageElement2 = NULL;
  OverlayImageElement* imageElement3 = new OverlayImageElement(.5,0,.5,0, "images/partners2.png", ofVec2f(0, 0));
  addElement(imageElement3);
  imageElement3 = NULL;
  OverlayImageElement* imageElement4 = new OverlayImageElement(.5,0,.5,0, "images/partners3.png", ofVec2f(0, 0));
  addElement(imageElement4);
  imageElement4 = NULL;
  
  OverlayImageElement* credits = new OverlayImageElement(1,0,1,0, "images/credits.png", ofVec2f(0, 0));
  addElement(credits);
  credits = NULL;
}

void OverlayHandler::addElement(OverlayElement* element)
{
  elements.push_back(element);
  element = NULL;
}
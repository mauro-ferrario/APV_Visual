//
//  OverlayText.h
//  APV_Visual
//
//  Created by Mauro Ferrario on 12/02/15.
//
//

#ifndef __APV_Visual__OverlayText__
#define __APV_Visual__OverlayText__

#include "ofMain.h"
#include "OverlayElement.h"
#include "OverlayTextElement.h"
#include "OverlayImageElement.h"

class APVVisual;

class OverlayHandler
{
public:
                          OverlayHandler();
  void                    setup(APVVisual* visual);
  virtual void            initFont();
  void                    initElements();
  void                    draw();
  ofTrueTypeFont          font;
  ofTrueTypeFont          font45;
  APVVisual*              visual;
  vector<OverlayElement*> elements;
  void                    startIntro(int element);
  void                    startOutro(int element);
  void                    startAnimation(int element, OverlayElementType type);
  void                    addElement(OverlayElement* element);
};

#endif /* defined(__APV_Visual__OverlayText__) */

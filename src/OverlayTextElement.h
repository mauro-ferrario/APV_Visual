//
//  OverlayTextElement.h
//  APV_Visual
//
//  Created by Mauro Ferrario on 12/02/15.
//
//

#ifndef __APV_Visual__OverlayTextElement__
#define __APV_Visual__OverlayTextElement__

#include "OverlayElement.h"

class OverlayTextElement: public OverlayElement
{
public:
                  OverlayTextElement();
                  OverlayTextElement(float introDuration, float introDelay, float outroDuration, float outroDelay, string text, ofTrueTypeFont* font, ofVec2f position);
  void            initTexts(ofTrueTypeFont* font);
  ofTrueTypeFont*	font;
  string          text;
  void            draw();
};

#endif /* defined(__APV_Visual__OverlayTextElement__) */

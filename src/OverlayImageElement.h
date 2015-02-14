//
//  OverlayImageElement.h
//  APV_Visual
//
//  Created by Mauro Ferrario on 14/02/15.
//
//

#ifndef __APV_Visual__OverlayImageElement__
#define __APV_Visual__OverlayImageElement__

#include "OverlayElement.h"

class OverlayImageElement: public OverlayElement
{
public:
  OverlayImageElement();
  OverlayImageElement(float introDuration, float introDelay, float outroDuration, float outroDelay, string url, ofVec2f position);
  ofImage         image;
  void            draw();
};

#endif /* defined(__APV_Visual__OverlayImageElement__) */

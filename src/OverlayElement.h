//
//  OverlayElement.h
//  APV_Visual
//
//  Created by Mauro Ferrario on 12/02/15.
//
//

#ifndef __APV_Visual__OverlayElement__
#define __APV_Visual__OverlayElement__

#include "ofMain.h"
#include "ofxTween.h"
#include "ofxTween.h"

enum OverlayElementType
{
  OVERLAY_ELEMENT_TYPE_INTRO,
  OVERLAY_ELEMENT_TYPE_OUTRO
};

class OverlayElement
{
public:
                      OverlayElement();
                      OverlayElement(float introDuration, float introDelay, float outroDuration, float outroDelay, ofVec2f position);
  void                update();
  void                draw(ofColor color);
  virtual void        draw();
  float               introDuration;
  float               introDelay;
  float               outroDuration;
  float               outroDelay;
  void                startAnimation(OverlayElementType type);
  OverlayElementType  type;
  ofxTween            tweenlinear;
  ofxEasingLinear     easinglinear;
  float               alpha;
  ofVec2f             position;
};

#endif /* defined(__APV_Visual__OverlayElement__) */

#ifndef __IKnobText_h__
#define __IKnobText_h__
//#pragma once

#ifndef __ICControls_h__
#  if defined(show_inclusions)
#  pragma message("INCLUDED ICControls.h from IKnobText.h")
#  endif
#include "ICControls.h"
#endif

#ifndef __IRenderHelper_h__
#  if defined(show_inclusions)
#  pragma message("INCLUDED IRenderHelper.h from IKnobText.h")
#  endif
#include "IRenderHelper.h"
#endif

//static const FloatPoint fp_empty = FloatPoint(-1.0f, -1.0f);

////////////////////////////////////////////////////////////////////////////
// IKnobText
////////////////////////////////////////////////////////////////////////////

// A multibitmap knob.
// The bitmap cycles through states as the mouse drags.
class IKnobText : public IKnobMultiControl, public IRenderHelper
{
protected:
public:
  bool    showEdgeMarks = 1;
  IKnobText::IKnobText(
    IPlugBase* pPlug
    , int x, int y
    , int paramIdx, IBitmap* pBitmap, IText *iText
    , int w = 24, int h = 24, int textH = 12
    , EDirection direction = kVertical
    , char* strFmt = "%0.3f\0"
    , double gearing = DEFAULT_GEARING
    )
    : IKnobMultiControl(pPlug, x, y, paramIdx, pBitmap, direction, gearing)
    , Width(w), Height(h), TextHeight(textH)
    , myText(*iText)
    , mTextFormat(strFmt), mLineHeight(iText->mSize)
    , tRect(x, y + h, x + w, y + textH)
  {
    mTargetRECT.B += textH;
    
    //mTargetRECT = IRECT(mRECT);
  }
  IKnobText::~IKnobText(){}

  bool IKnobText::Draw(IGraphics* pGraphics);
  bool IKnobText::IsDirty();

  void IKnobText::ClearCharNames()
  {
    charNames = nullptr;
    charNamesCount = NULL;
    hasCharNames = false;
  }
  void IKnobText::SetCharNames(int count, char** charnames)
  {
    charNames = charnames;
    charNamesCount = count;
    hasCharNames = true;
  }

protected:

  char**  charNames; // we have the clear charNames properly weather it is or not set ;(
  int     charNamesCount;
  bool    hasCharNames = 0;
  int     edgeMarkWidth = 3;
  //const char* FilterTypes[pFilterCount];
  int     Width, Height, TextHeight;
  int     mLineHeight;
  IRECT   tRect;
  char*   mTextFormat;
  IText   myText;
};

#endif

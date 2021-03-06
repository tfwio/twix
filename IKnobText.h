#ifndef __IKnobText_h__
#define __IKnobText_h__
//#pragma once

#define WHEEL_DEPTH_DEFAULT 0.01
#define WHEEL_DEPTH_CONTROL 0.001

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
  IKnobText(
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
    , mWheelManual(1)
    , mWheelChangeSmall(0.333333)
    , mWheelChangeDefault(1.)
    , edgeMarkWidth(3)
    , hasCharNames(0)
    , mTextActive((*iText).mColor)
    , mTextDisabled(81, myText.mColor.R, myText.mColor.G, myText.mColor.B)
    , mIgnoreEnumText(true)
  {
    mTargetRECT.B += textH;
    mDisablePrompt = false;
    //mTargetRECT = IRECT(mRECT);
  }
  ~IKnobText(){}

  bool Draw(IGraphics* pGraphics);
  bool IsDirty();

  void OnMouseDown(int x, int y, IMouseMod* pMod) override
  {
#ifdef PROTOOLS
    if (pMod->A && mDefaultValue >= 0.0)
    {
      mValue = mDefaultValue;
      SetDirty();
    }
#endif
    if ((GetParam()->Type() == IParam::EParamType::kTypeEnum) && pMod->R)
      PromptUserInput();
    else if ((pMod->S && pMod->R) || (pMod->C && pMod->R))
      PromptUserInput();

    // if (pMod->R) {
    //   PromptUserInput();
    // }
  }
  void OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) override
  {
    double gearing = mGearing;

#ifdef PROTOOLS
#ifdef OS_WIN
    if (pMod->C) gearing *= 10.0;
#else
    if (pMod->R) gearing *= 10.0;
#endif
#else
    if (pMod->C || pMod->S) gearing *= 10.0;
#endif

    if (GetParam()->Type()!=IParam::EParamType::kTypeEnum) mValue += (mDirection == kVertical) ?
      (double)dY / (double)(mRECT.T - mRECT.B) / gearing :
      (double)dX / (double)(mRECT.R - mRECT.L) / gearing;
    else mValue -= (mDirection == kVertical) ?
      (double)dY / (double)(mRECT.T - mRECT.B) / gearing :
      (double)dX / (double)(mRECT.R - mRECT.L) / gearing;

    SetDirty();
  }
  void OnMouseWheel(int x, int y, IMouseMod* pMod, int d) override
  {
//#ifdef PROTOOLS
//    if (pMod->C)
//    {
//      mValue += 0.001 * d;
//    }
//#else
//#endif
    if (mWheelManual){
      switch (GetParam()->Type())
      {
      case IParam::EParamType::kTypeEnum:
        if (pMod->C || pMod->S) mValue += WHEEL_DEPTH_CONTROL * d;
        else mValue = ToNormalizedParam(GetParam()->Int() - (mWheelChangeDefault*d), GetParam()->GetMin(), GetParam()->GetMax(), 1.0);
        break;
      case IParam::EParamType::kTypeInt:
        if (pMod->C || pMod->S) mValue += WHEEL_DEPTH_CONTROL * d;
        else mValue = ToNormalizedParam(GetParam()->Int() + (mWheelChangeDefault*d), GetParam()->GetMin(), GetParam()->GetMax(), 1.0);
        break;
      default:
        if (pMod->C || pMod->S) mValue += WHEEL_DEPTH_CONTROL * d;
        else mValue += WHEEL_DEPTH_DEFAULT * d;
        break;
      }
    }
    else
    {
      if (pMod->C || pMod->S) mValue += WHEEL_DEPTH_CONTROL * d;
      else mValue += WHEEL_DEPTH_DEFAULT * d;
    }

    SetDirty();
  }

  void ClearCharNames()
  {
    charNames = nullptr;
    charNamesCount = 0;
    hasCharNames = false;
  }
  void SetCharNames(int count, char** charnames)
  {
    charNames = charnames;
    charNamesCount = count;
    hasCharNames = true;
  }

  void SetIgnoreEnumText(bool ignore) { mIgnoreEnumText = ignore; }

protected:
  bool    mWheelManual, mIgnoreEnumText;
  IColor  mTextActive, mTextDisabled;
  double  mWheelChangeSmall, mWheelChangeDefault;
  char**  charNames; // we have the clear charNames properly weather it is or not set ;(
  int     charNamesCount;
  bool    hasCharNames;
  int     edgeMarkWidth;
  //const char* FilterTypes[pFilterCount];
  int     Width, Height, TextHeight;
  int     mLineHeight;
  IRECT   tRect;
  char*   mTextFormat;
  IText   myText;
};

#endif

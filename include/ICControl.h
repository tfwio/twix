#ifndef __ICControl__
#define __ICControl__
//#pragma once
#include "ICControls.h"


// Output text to the screen.
class ITextTimeControl : public ITextControl
{
public:
  ITextTimeControl(IPlugBase* pPlug, IRECT pR, IText* pText, const char* str = "")
    : ITextControl(pPlug, pR, pText)
  {
    mText = *pText;
    mStr.Set(str);
  }
  ~ITextTimeControl() {}

  //void ITextTimeControl::OnGUIIdle() override
  //{
  //  IControl::OnGUIIdle();
  //  SetTextFromPlug(AltTimeString(2).Get());
  //  SetDirty();
  //  Redraw();
  //}
  //void Updater()
  //{
  //  GetTimeString(3);
  //}

  int mIncr = 0;
  inline WDL_String AltTimeString(int mode = 1)
  {
    if (mode == 1)
    {
      double smps = mPlug->GetSamplePos();
      int smpMod = int(smps) % int(mPlug->GetSampleRate());
      return WDL_String(std::to_string(smpMod).c_str());
    }
    else if (mode == 2)
    {
      ITimeInfo time;
      mPlug->GetTime(&time);
      if (!time.mTransportIsRunning)
      {
        mIncr++;
        mIncr = mIncr %= 10000;
        WDL_String str = WDL_String("i: ");
        str.Append(std::to_string(mIncr).c_str());
        return str;
      }
      else {
        double smps = mPlug->GetSamplePos();
        return WDL_String(std::to_string(smps).c_str());
      }
    }
    return WDL_String("?");
  }
  inline WDL_String GetTimeString(int mode = 1)
  {
    if (mode == 1)
    {
      double smps = mPlug->GetSamplePos();
      int smpMod = int(smps) % int(mPlug->GetSampleRate());
      return WDL_String(std::to_string(smpMod).c_str());
    }
    else if (mode == 2)
    {
      double smps = mPlug->GetSamplePos();
      return WDL_String(std::to_string(smps).c_str());
    }
    else if (mode == 3)
    {
      double smps = mPlug->GetSamplePos();
      int smpMod = int(smps) % int(mPlug->GetSampleRate());
      smps = 1. / smpMod;
      mStr = WDL_String(std::to_string(smps).c_str());
      SetTextFromPlug(mStr.Get());
      return mStr;
    }

    return WDL_String("?");
  }

  //bool IsDirty() override {
  //  GetTimeString(3);
  //  return true;
  //}

  //void SetTextFromPlug(char* str);
  //void ClearTextFromPlug() { SetTextFromPlug((char *) ""); }
  //bool Draw(IGraphics* pGraphics);

protected:
  //WDL_String mStr;
};
////////////////////////////////////////////////////////////////////////////
// ICControl
////////////////////////////////////////////////////////////////////////////

class ICControl : public IControl
{
protected:
  IColor ColourFg, ColourBg;
  bool mIsDown;
  int mAdjust;
  IntPoint
    iMouseDown,      // one and only
    iMouseDownDiff,  // diff
    iMouseDownD,     // direction
    iMouseMove,      // drag[0]
    iMouseMove2,     // drag[1]
    iMouseOver,      // over[0]
    iMouseOverDiff,  // diff
    iMouseOverD,     // direction
    iMouseOver2      // over[1]
    ;
public:
  virtual void OnMouseOver(int x, int y, IMouseMod *pMod) override {
    IControl::OnMouseOver ( x, y, pMod );
    iMouseMove2    . from ( iMouseMove ); // back up prior MouseMove
    iMouseMove     . set  ( x, y );       // Set a new MouseMove
    iMouseOver2    . from ( iMouseOver ); // Back Up Prior MouseOver
    iMouseOver     . set  ( x, y );       // Set New MouseOver
    iMouseOverDiff . from ( iMouseOver2 - iMouseOver );
    iMouseOverD    . from ( iMouseOverD.compare() );
  }
  virtual void OnMouseDrag(int x, int y, int dX, int dY, IMouseMod *pMod) override {
    iMouseMove2    . from(iMouseMove);
    iMouseMove     . set(x, y);
    iMouseDownDiff . from(iMouseMove2 - iMouseMove);
    iMouseDownD    . from(iMouseDownD.compare());
  }
  virtual void OnMouseUp(int x, int y, IMouseMod *pMod) override {
    mIsDown = false;
    iMouseMove2    . from(iMouseMove);
    iMouseMove     . set(x, y);
    iMouseDownDiff . from(iMouseMove2 - iMouseMove);
  }
  virtual void OnMouseDown(int x, int y, IMouseMod *pMod) override {
    mIsDown = true;
    iMouseDown     . set(x, y);
    iMouseMove2    . from(iMouseMove);
    iMouseMove     . set(x, y);
    //MouseD.from(MouseMove2.X - MouseMove.X, MouseMove2.Y - MouseMove.Y);
  }
  virtual void OnMouseDblClick(int x, int y, IMouseMod *pMod) override {

  }

  ICControl(IPlugBase* pPlug, IRECT pR, int paramIdx = -1, IChannelBlend blendMethod = IChannelBlend::kBlendNone)
    : IControl(pPlug, pR, paramIdx, blendMethod)
    , ColourFg(255, 0, 0, 0), ColourBg(0, 0, 0, 0)
    , iMouseDown(1, 1)
    , iMouseMove(1, 1), iMouseMove2(0, 0)
    , iMouseDownD(0, 0)
    , iMouseOver(0, 0), iMouseOver2(0, 0)
    , iMouseOverD(0, 0)
    , mAdjust(3)
  {
  }
  ~ICControl(){
  }
  inline void update()
  {
    if (this->mParamIdx > -1) {
      IParam *par = this->GetParam();
      if (par) {
        this->mValue = par->Value();
        //this->mValue = par->GetNormalized();
        //this->mDefaultValue = par->GetDefaultNormalized();
      }
    }
    else
    {
      ::MessageBox(0, "WTF?", NULL, 1);
    }
  }
  inline double normalized(){
    return GetParam()->GetNormalized();
  }
  inline double value(){
    return GetParam()->Value();
  }
};



// A set of buttons that maps to a single selection.
// Bitmap has 2 states, off (default state; EG:==0) and on.
class IRadioButtonMatrixControl : public IControl
{
public:
  IRadioButtonMatrixControl(IPlugBase* pPlug, IRECT pR, int paramIdx, int nButtons, IBitmap* pBitmap,
    EDirection direction = kVertical, bool reverse = false);
  ~IRadioButtonMatrixControl() {}

  //
  void OnMouseDown(int x, int y, IMouseMod* pMod);

  //
  bool Draw(IGraphics* pGraphics);

protected:
  WDL_TypedBuf<IRECT> mRECTs;
  IBitmap mBitmap;
};

#endif

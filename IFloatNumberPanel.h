#ifndef __IFloatNumberPanel_h__
#define __IFloatNumberPanel_h__
//#pragma once


// DONT KNOW WHERE THE FOLLOWING COMMENT SHOULD GO IF NOT TO HLL

// - mouse moves left to right.
// - N columns
// - for example if you give me n=6, me give 5.  Give me 9, me give 7.
// - return digit column mouse is in 0123456789012
// - the index N-th char in such as "103,040.10801"
// - digit column returned 0-10 of 11 total columns
// - indx references: `int8 abbie[11]={0,1,2,3,4,5,6,7,8,9,0}`
// -----------------------------------------
// where index 'n' == `5` /returns pow(10,n={6-n[=5]=1})[=10]
// ....

////////////////////////////////////////////////////////////////////////////
// IFloatNumberPanel
////////////////////////////////////////////////////////////////////////////
#include "IColNumber.h"
#include "IColDetail.h"
/**

The important thing to point out about this control is that our
parameter must be an integer whose max is 249 and whose min is -250.
This will yield a parameter value that cane be easily manipulated
by our host.


What I want here is a class that takes care of my Number Renderer.
We know that in windows we can call on some specific functions to get
the mouse to wrap around from the top to the bottom of the control.

I'm thinking that a spinner (up/mid-line/down) button (spin-button)
can be made by assigning such as mentioned, and radio indiciation as
to which digit is (active) controlled by the up/down---while grabbing
the spinner's midline would give the same control as clicking the
respective digit column.

- The first step in writing this control is to write a control that
describes its value.
- We can then add a knob or slider and modify the above
- Finally we would duplicate the effect and prove replication is possible.

**/
class IFloatNumberPanel :
  public IControl,
  public IRenderHelper
{
protected:
  IColor ColourFg, ColourBg;
  int mAdjust;
  // IText *mText; //, int w = 24, int h = 24, int textH = 12, char* strFmt = "%0.3f"
  // IRECT mRect;
protected:
  IntPoint mdown, mdiff, mmove, mmdown, cx;
  int mCharWidth, mDelta;
  IText mText;
  IColor meme, ColorHot, ColorActive, mColorInactive;
  ColumnDetail cd;

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
    #if WIN32
    else
    {
      ::MessageBox(0, "WTF?", NULL, 1);
    }
    #endif
  }
  inline double normalized(){
    return GetParam()->GetNormalized();
  }
  inline double value(){
    return GetParam()->Value();
  }

public:
  IFloatNumberPanel(
    IPlugBase* plug // IPlugBase
    , int c_width
    , IRECT&   rect // underlying PanelControl
    , int paramIdx  // Plugin (Double) Parameter Index
    , IText* mtext  // our text style.  We need something fixed-width
    , char *tooltip = nullptr // if defined, we set as tooltip
    )
    : IControl  (plug, rect, paramIdx)
    , ColourFg(255, 0, 0, 0)
    , ColourBg(0, 0, 0, 0)
    , mAdjust(3)
    // -------------------------------------
    , ColorHot   (110, 105,  70)
    , ColorActive(129, 124,  90)
    , mColorInactive(127, mtext->mColor.R, mtext->mColor.G, mtext->mColor.B)
    , mCharWidth(c_width)
    , mText      (*mtext)
    , meme       (255, 48, 48, 48)
    , mDelta     (0)
    //, IControl::mParamIdx(paramIdx)
    , cx         (0, 0)
    , mdown      (-1, -1)
    , mdiff      (-1, -1)
    , mmove      (-1, -1)
    , cd(rect.L, mCharWidth)
  {
    this->mTooltip = WDL_String(tooltip);
    mDisablePrompt = false;
    this->SetTextEntryLength(12);
  }

  #pragma region GetWRect(void) TopLeft(void) SetCursorPos(IntPoint) FIXME: MAC MAC MAC

  // again, don't know what to do for MAC
  inline IRECT GetWRect() {
    IGraphicsWin *win = (IGraphicsWin*)GetPlug()->GetGUI();
    return win->GetWindowRECT();
  }

  // don't know what to do for MAC.
  inline IntPoint TopLeft() {
    IRECT r = GetWRect(); // probably does not work.
    return { r.L, r.T };
  }

  // what to do for MAC?
  inline void SetCursorPos(IntPoint pt) {
    IntPoint px = pt + TopLeft();
    ::SetCursorPos(px.X, px.Y);
  }

  void OnMouseOut() override
  {
    cd.clear();
    SetDirty(false);
    Redraw();
  }
  #pragma endregion

  void PromptUserInput() override
  {
    if (mParamIdx >= 0 && !mDisablePrompt)
    {
      if (mPlug->GetParam(mParamIdx)->GetNDisplayTexts()) // popup menu
      {
        mPlug->GetGUI()->PromptUserInput(this, mPlug->GetParam(mParamIdx), &mRECT);
      }
      else // text entry
      {
        int cX = (int)mRECT.MW();
        int cY = (int)mRECT.MH();
        int halfW = int(float(mRECT.W()) / 2.f);
        int halfH = int(float(16) / 2.f);

        IRECT txtRECT = IRECT(cX - halfW, cY - halfH, cX + halfW, cY + halfH);
        mPlug->GetGUI()->PromptUserInput(this, mPlug->GetParam(mParamIdx), &txtRECT);
      }

      Redraw();
    }
  }
  virtual void OnMouseOver(int x, int y, IMouseMod* pMod) override;
  virtual void OnMouseDown(int x, int y, IMouseMod* pMod) override;
  virtual void OnMouseWheel(int x, int y, IMouseMod* pMod, int d) override;
  virtual void OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) override;
  virtual void OnMouseUp(int x, int y, IMouseMod* pMod) override;
  virtual bool Draw(IGraphics* pGraphics) override;

  #if WIN32
  // this really should never be used
  void show_msg() {
    std::string xo =
      "mVal: " + std::to_string(this->mValue) +
      "v(): " + std::to_string(value()) +
      ", id: " + std::to_string(mParamIdx) +
      ", cd: " + std::to_string(this->cd.value()) +
      ", V: " + std::to_string(this->GetParam()->Value())
      ;
    ::MessageBox(0, (char*)xo.c_str(), NULL, 0);
  }
  #endif

  void DrawStringPart(IGraphics *pGraphics, IRECT &r, int col, int hot);

  // bool IsDirty() { return true; }
};
#endif

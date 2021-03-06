#include "ICControls.h"

// abstraction for GetIsAmp and GetIsAmpG
bool ParamInfo(IParam *pPram, double pMin, double pMax, IParam::EParamType pType)
{
  return (pPram->GetMin() == pMin) && (pPram->GetMax() == pMax) && (pPram->Type() == pType);
}
// for 0-181 (-40 to 0?) dB amplitude
bool GetIsAmp(IParam *pPram) { return ParamInfo(pPram, 0, 181, IParam::EParamType::kTypeInt); }
// for -20 - +20 dB amplitude
bool GetIsAmpG(IParam *pPram) { return ParamInfo(pPram, -20, 20, IParam::EParamType::kTypeInt); }

////////////////////////////////////////////////////////////////////////////
// ICPanelControl
////////////////////////////////////////////////////////////////////////////

bool ICPanelControl::Draw(IGraphics *pGraphics) {
  const IColor color_blue_ish(12, 75, 132, 50);
  IPanelControl::Draw(pGraphics);
  IRECT xRECT(mRECT);
  xRECT.R -= 1;
  xRECT.B -= 1;
  if (true) pGraphics->DrawRect(&color_blue_ish, &xRECT);
  if (cropShow) DrawCropMarks(pGraphics, mRECT, cropColour, cropSize);
  return 1;
}

////////////////////////////////////////////////////////////////////////////
// IFloatNumberPanel
////////////////////////////////////////////////////////////////////////////

void IFloatNumberPanel::OnMouseOver(int x, int y, IMouseMod* pMod) {
  update();
  cd.m_over(x, y);
}

const double mDrag = 1 / 3;

void IFloatNumberPanel::OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) {
  cd.m_drag(x, y, &mRECT);
  IntPoint a, b, c;
  cd.PointInfo(a, b, c);
  SetCursorPos(b);
  //if (abs(dY) < 2.25) return;
#ifdef EXTRA
  WDL_String me;
  me.SetFormatted(32,"%s � %d %d",GetParam()->GetNameForHost(),dX,dY);
  this->SetTooltip(me.Get());
  ((IGraphicsWin*)GetGUI())->ShowTooltip();
#endif
  // this following 2 lines is for the new parameter type and happens to be wrong.
  // double result = (cd.index1d() * 100) + cd.value();
  // mValue = result; // 
  update();
  mValue = value() + cd.value(); // 
  #if 1
  WDL_String me;
  me.SetFormatted(32, "%s � %d", GetParam()->GetNameForHost(), c.Y);
  this->SetTooltip(me.Get());
  ((IGraphicsWin*)GetGUI())->ShowTooltip();
  #endif
  // -- begin non-automation value change --
  mValue = CONTAIN(mValue, 0., 999999.99999);
  GetParam()->Set(mValue);
  mPlug->OnParamChange(mParamIdx);
  mPlug->InformHostOfParamChange(mParamIdx, mValue);
  //mPlug->SetParameterFromGUI(mParamIdx, mValue);
  // -- begin test-automation value change --
  //GetParam()->SetNormalized(mValue);
  SetDirty(false);
}

void IFloatNumberPanel::OnMouseUp(int x, int y, IMouseMod* pMod) {
  GetGUI()->ShowMouseCursor();
  cd.m_up(x, y);
  //ow_msg();
}

void IFloatNumberPanel::OnMouseDown(int x, int y, IMouseMod* pMod) {
  GetGUI()->HideMouseCursor();
  IControl::OnMouseDown(x,y,pMod);
  update();
  cd.m_down(x, y);
}

void IFloatNumberPanel::OnMouseWheel(int x, int y, IMouseMod* pMod, int d) {
  //SetCursorPos(cd.last_mouseMove());
  //ICControl::OnMouseWheel(x, y, pMod, d);
  mValue = value() + ColumnDetail::check(x, cd.getL(), cd.getW(), d >=1 ); // 
  GetParam()->Set(mValue);
  mPlug->OnParamChange(mParamIdx);
  mPlug->InformHostOfParamChange(mParamIdx, mValue);
  SetDirty(false);
}


const IColor mbg(255, 218, 217, 197), abg(255, 218, 217, 197), hitme(255, 248, 255, 250);
void IFloatNumberPanel::DrawStringPart(IGraphics *pGraphics, IRECT &r, int col, int hot) {
  char str[2];
  str[1] = 0;
  std::string xo = StrUtil::float2str_alt(mValue); // what we draw first
  mText.mColor = IsGrayed() ? mColorInactive : COLOR_BLACK;
  //pGraphics->FillRoundRect(&mbg, &mRECT, &mBlend, 2, 1);
  //pGraphics->RoundRect(&abg, &mRECT, &mBlend, 2, 1);
  for (int i = 0; i < xo.length(); i++)
  {
    str[0] = xo.at(i);
    if (i == col) pGraphics->FillIRect(&hitme, &r);
    //else if (i == hot) pGraphics->FillIRect(&ColorHot, &r);
    //else pGraphics->FillIRect(&meme, &r);
    pGraphics->DrawIText(&mText, str, &r);
    r.MoveX(mCharWidth + 1);
  }
}

bool IFloatNumberPanel::Draw(IGraphics* pGraphics) {
  mValue = mPlug->GetParam(mParamIdx)->Value();
  // 
  IRECT r(mRECT);
  r.SetWidthHeight(mCharWidth, mRECT.H());
  // 
  // pGraphics->SetStrictDrawing(false);
  
  DrawStringPart(pGraphics, r, int(cd.index1()), int(cd.index2()));
  //    // 
  //    r.MoveTo(mRECT.L, r.B + 4);
  //    r.SetWidthHeight(mRECT.W(), mRECT.H());
  //    
  //    std::string xx =
  //    //  " CO=" + std::to_string((int)cd.index1()) +
  //    //  " PY=" + std::to_string(cd.compare());
  //    "1=" + StrUtil::float2str(cd.value())
  //    + " P=" + std::to_string(cd.polarity());
  //    pGraphics->DrawIText(&mText, (char*)xx.c_str(), &r);
  //    r.MoveTo(mRECT.L, r.B + 4);
  //    xx = "2=" + StrUtil::float2str(cd.power());
  //    pGraphics->DrawIText(&mText, (char*)xx.c_str(), &r);
  // pGraphics->SetStrictDrawing(true);
  return 1;
}

////////////////////////////////////////////////////////////////////////////
// IKnobText
////////////////////////////////////////////////////////////////////////////
bool IKnobText::Draw(IGraphics* pGraphics) {
  
  IParam *para = GetParam();
  bool skipText = (para->Type() == IParam::EParamType::kTypeEnum) && mIgnoreEnumText;

  double val = GetIsAmp(para) ? 20. * log10(pow(para->Value(), 2) / 32767.) : para->Value();
  
  int i = BOUNDED(1 + int(0.5 + mValue * (double)(mBitmap.N - 1)), 1, mBitmap.N);
  pGraphics->DrawBitmap(&mBitmap, &mRECT, i, &mBlend); // Draw the knob like a good overloaded class should...

  if (showEdgeMarks) DrawCropMarks(pGraphics, mRECT, &COLOR_RED);
  if (skipText) return true;

  pGraphics->SetStrictDrawing(false); // ENTER NON-STRICT DRAWING SECTION
  int intvalue = para->Int();
  char *some = new char[32]; // 32 is max-chars for param displays.
                             // may as well go with that.
  std::string sv;
  IParam::EParamType t = para->Type();

  if (t == IParam::EParamType::kTypeInt && GetIsAmp(para))
  {
    if (intvalue == 0) sprintf_s(some, 32, "Mute");
    else sprintf_s(some, 32, "%0.2f dB", val);
    sv = std::string(some);
  }
  else if (GetIsAmpG(para))
  {
    sprintf_s(some, 32, "%0.0f dB", val);
    sv = std::string(some);
  }
  else if (hasCharNames)
  {
    sv = charNames[intvalue];
  }
  else switch (t)
  {
  case IParam::EParamType::kTypeEnum: some = (char*)para->GetDisplayText(intvalue); sv = some; break;
  case IParam::EParamType::kTypeInt: sprintf_s(some, 32, "%0.0f", val); sv = some; break;
  case IParam::EParamType::kTypeBool: sprintf_s(some, 32, "%0.3f", val); sv = some; break;
  case IParam::EParamType::kTypeDouble:
  default: sprintf_s(some, 32, "%0.1f", val); sv = some; break;
  }

  myText.mColor = IsGrayed() ? mTextDisabled : mTextActive;

  pGraphics->DrawIText(&myText, (char*)sv.c_str(), &tRect);
  pGraphics->SetStrictDrawing(true); // EXIT NON-STRICT DRAWING SECTION
  return 1;
}

//////////////////////////////////////////////////////////////////////////////
// IRadioButtonMatrixControl
//////////////////////////////////////////////////////////////////////////////

IRadioButtonMatrixControl::IRadioButtonMatrixControl(IPlugBase* pPlug, IRECT pR, int paramIdx, int nButtons,
  IBitmap* pBitmap, EDirection direction, bool reverse)
  : IControl(pPlug, pR, paramIdx), mBitmap(*pBitmap)
{
  mRECTs.Resize(nButtons);
  int h = int((double)pBitmap->H / (double)pBitmap->N);

  if (reverse)
  {
    if (direction == kHorizontal)
    {
      int dX = int((double)(pR.W() - nButtons * pBitmap->W) / (double)(nButtons - 1));
      int x = mRECT.R - pBitmap->W - dX;
      int y = mRECT.T;

      for (int i = 0; i < nButtons; ++i)
      {
        mRECTs.Get()[i] = IRECT(x, y, x + pBitmap->W, y + h);
        x -= pBitmap->W + dX;
      }
    }
    else
    {
      int dY = int((double)(pR.H() - nButtons * h) / (double)(nButtons - 1));
      int x = mRECT.L;
      int y = mRECT.B - h - dY;

      for (int i = 0; i < nButtons; ++i)
      {
        mRECTs.Get()[i] = IRECT(x, y, x + pBitmap->W, y + h);
        y -= h + dY;
      }
    }

  }
  else
  {
    int x = mRECT.L, y = mRECT.T;

    if (direction == kHorizontal)
    {
      int dX = int((double)(pR.W() - nButtons * pBitmap->W) / (double)(nButtons - 1));
      for (int i = 0; i < nButtons; ++i)
      {
        mRECTs.Get()[i] = IRECT(x, y, x + pBitmap->W, y + h);
        x += pBitmap->W + dX;
      }
    }
    else
    {
      int dY = int((double)(pR.H() - nButtons * h) / (double)(nButtons - 1));
      for (int i = 0; i < nButtons; ++i)
      {
        mRECTs.Get()[i] = IRECT(x, y, x + pBitmap->W, y + h);
        y += h + dY;
      }
    }
  }
}

void IRadioButtonMatrixControl::OnMouseDown(int x, int y, IMouseMod* pMod)
{
#ifdef PROTOOLS
  if (pMod->A)
  {
    if (mDefaultValue >= 0.0)
    {
      mValue = mDefaultValue;
      SetDirty();
      return;
    }
  }
  else
#endif
  if (pMod->R)
  {
    PromptUserInput();
    return;
  }

  int i, n = mRECTs.GetSize();

  for (i = 0; i < n; ++i)
  {
    if (mRECTs.Get()[i].Contains(x, y))
    {
      mValue = (double)i / (double)(n - 1);
      break;
    }
  }

  SetDirty();
}

bool IRadioButtonMatrixControl::Draw(IGraphics* pGraphics)
{
  int i, n = mRECTs.GetSize();
  int active = int(0.5 + mValue * (double)(n - 1));
  active = BOUNDED(active, 0, n - 1);
  for (i = 0; i < n; ++i)
  {
    if (i == active)
    {
      pGraphics->DrawBitmap(&mBitmap, &mRECTs.Get()[i], 2, &mBlend);
    }
    else
    {
      pGraphics->DrawBitmap(&mBitmap, &mRECTs.Get()[i], 1, &mBlend);
    }
  }
  return true;
}
//bool IKnobText::Draw(IGraphics* pGraphics) {
//
//  double val = GetParam()->Value();             // value as double
//  std::string strvalue = std::to_string(val);   // value as string
//
//  // DRAW OUR BITMAP =======================================================
//  int i = BOUNDED(1 + int(0.5 + mValue * (double)(mBitmap.N - 1)), 1, mBitmap.N);
//  pGraphics->DrawBitmap(&mBitmap, &mRECT, i, &mBlend); // Draw the knob like a good overloaded class should...
//
//  // NOW DRAW THE OUTLINE CROSSHAIR THINGIES ===============================
//  if (showEdgeMarks) DrawCropMarks(pGraphics, mRECT, &COLOR_RED);
//
//  // Continue on to draw the text ==========================================
//
//  pGraphics->SetStrictDrawing(false); // ENTER NON-STRICT DRAWING SECTION
//  
//  char *some = new char[32]; // 32 is max-chars for param displays.
//                             // may as well go with that.
//  //char *some = (char*)malloc(32);
//
//  // this time we get the value as an int?
//  int intvalue = GetParam()->Int();
//  IParam::EParamType t = GetParam()->Type();
//  switch (t)
//  {
//  case IParam::EParamType::kTypeEnum:
//  case IParam::EParamType::kTypeInt:
//    sprintf_s(some, 32, "%0.0f", val);
//    //sprintf_s(some, 90, mTextFormat, intvalue);
//    break;
//  case IParam::EParamType::kTypeBool:
//    sprintf_s(some, 32, "%0.3f", val);
//    //sprintf_s(some, 32, mTextFormat /* "%3.0f"*/, val);
//    //sprintf_s(some, 90, mTextFormat, intvalue);
//    break;
//  case IParam::EParamType::kTypeDouble:
//  default:
//    sprintf_s(some, 32, "%0.1f", val);
//    break;
//  }
//  std::string sv(some);
//  //free(some);
//  //delete some;
//
//  // so we're drawing text which was supplied for an enumeration, or the parameter's text value.
//  pGraphics->DrawIText(&myText, hasCharNames ? charNames[intvalue] : (char*)sv.c_str(), &tRect);
//  //pGraphics->DrawIText(&myText, hasCharNames ? charNames[intvalue] : (char*)strvalue.c_str(), &tRect);
//  pGraphics->SetStrictDrawing(true); // EXIT NON-STRICT DRAWING SECTION
//
//  return 1;
//}

bool IKnobText::IsDirty(){ return 1; }

////////////////////////////////////////////////////////////////////////////


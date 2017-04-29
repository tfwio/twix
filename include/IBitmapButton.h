#ifndef __IBitmapButton_h__
#define __IBitmapButton_h__
#ifdef _WIN32
#pragma once
#endif

#include "ICControls.h"

////////////////////////////////////////////////////////////////////////////
// IBitmapButton
////////////////////////////////////////////////////////////////////////////

#include <functional>

// We want our control to display different states:
// - mouse-hover, mouse-down, normal (and perhaps disabled)
class IBitmapButton : public ICControl
{

public:

  IBitmapButton(IPlugBase* pPlug, int x, int y, IBitmap* pBitmap,
    IChannelBlend::EBlendMethod blendMethod = IChannelBlend::kBlendNone)
    : ICControl(pPlug, IRECT(x, y, pBitmap), -1, blendMethod)
    , mTargetParamId(-1)
    , mTargetParamValue(-1)
    , mBitmap(*pBitmap)
    , mHasDownHandler(false)
    , mHasUpHandler(false)
  {
    mDblAsSingleClick = true;
  }

  virtual ~IBitmapButton() override {}

  //////////////////////////////////////////////////////////////////////////
  // IBitmapButton *Handler
  //////////////////////////////////////////////////////////////////////////
  enum EHandlerType
  {
    handle_move,
    handle_down,
    handle_up
  };

  IBitmapButton *setParamInfo(int pId, int pValue)
  {
    mTargetParamId = pId;
    mTargetParamValue = pValue;
    return this;
  }

  virtual IBitmapButton *Handler(std::function<void()> callback, EHandlerType hType)
  {
    switch (hType)
    {
    case handle_up: mUpEventHandler = callback;  mHasUpHandler = true; break;
    case handle_down: mDownEventHandler = callback; mHasDownHandler = true; break;
    }
    return this;
  }

  virtual bool Draw(IGraphics* pGraphics) override {
    int i = 1;
    if (mBitmap.N > 1)
    {
      i = 1 + int(0.5 + mValue * (double)(mBitmap.N - 1));
      i = BOUNDED(i, 1, mBitmap.N);
    }
    return pGraphics->DrawBitmap(&mBitmap, &mRECT, i, &mBlend);
  }

  //bool IsDirty() { return true; }
  virtual void OnMouseUp(int x, int y, IMouseMod *pMod) override {
    ICControl::OnMouseUp(x, y, pMod);
    mValue = IsTargetParamSelected() ? 1 : 0;
    if (this->mHasUpHandler) this->mUpEventHandler();
  }
  virtual void OnMouseOver(int x, int y, IMouseMod *pMod) override {
    ICControl::OnMouseOver(x, y, pMod);
    mValue = mIsDown ? 0.5 : 1;
  }
  virtual void OnMouseOut() override {
    IControl::OnMouseOut();
    mValue = IsTargetParamSelected() ? 1 : 0;
  }
  virtual void OnMouseDblClick(int x, int y, IMouseMod *pMod) override {}
  virtual void OnMouseDown(int x, int y, IMouseMod *pMod) override
  {
    ICControl::OnMouseDown(x, y, pMod);
    mValue = IsTargetParamSelected() ? 1. : 0.5;
    if (this->mHasDownHandler) this->mDownEventHandler();
    if (IsTargeted() && !IsTargetParamSelected()) SetTargetParam();
  }

private:


  inline bool IsTargetParamSelected()
  {
    if (!IsTargeted()) return false;
    auto para = GetPlug()->GetParam(mTargetParamId);
    return para->GetNormalized() == ToNormalizedParam(mTargetParamValue, para->GetMin(), para->GetMax(), para->GetShape());
  }

  inline void SetTargetParam()
  {
    if (!IsTargeted()) return;
    auto para = GetPlug()->GetParam(mTargetParamId);
    para->SetNormalized(mTargetParamValue); // ToNormalizedParam(mTargetParamValue, para->GetMin(), para->GetMax(), para->GetShape()));
  }

  inline bool IsTargeted() const { return mTargetParamId >=0; }

  int mTargetParamId;
  int mTargetParamValue;

  // protected:
  IBitmap mBitmap;
  std::function<void()> mDownEventHandler, mUpEventHandler;
  bool mHasDownHandler, mHasUpHandler;
};
#endif


#ifndef __ICPanelControl__
#define __ICPanelControl__
//#pragma once


#include "ICControls.h"

////////////////////////////////////////////////////////////////////////////
// ICPanelControl
////////////////////////////////////////////////////////////////////////////

class ICPanelControl : public IPanelControl, public IRenderHelper
{
public:
  ICPanelControl(
    IPlugBase* pPlug,
    IRECT pR,
    const IColor* pColor,
    const IColor* pColor2,
    IntPoint size=IntPoint(7,5)) : IPanelControl(pPlug, pR, pColor)
    , cropShow(true)
    , cropColour(pColor2)
    , cropSize(size)
  {
  }
  ~ICPanelControl(){ /*~IPanelControl();*/ }

  virtual bool Draw(IGraphics *pGraphics) override;
  bool     cropShow;

protected:
  const IColor* cropColour;
  IntPoint cropSize;
};

#endif

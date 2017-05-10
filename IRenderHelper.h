#ifndef __IRenderHelper_h__
#define __IRenderHelper_h__
//#pragma once

#include <IControl.h>
////////////////////////////////////////////////////////////////////////////
// IRenderHelper
////////////////////////////////////////////////////////////////////////////

bool ParamInfo(IParam *pPram, double pMin, double pMax, IParam::EParamType pType);
bool GetIsAmp(IParam *pPram);
bool GetIsAmpG(IParam *pPram);


// contains some methods that are inherited into IControl controls
// for rendering a border around the control's boundary, perhaps while
// under mouse-up, mouse-over or mouse-down conditions.
class IRenderHelper
{
protected:
  
  void GetMarker(
    IGraphics *gfx,
    const IColor *colour,
    int c, // 0=TL, 1=TR, 2=BR, 3=BL
    IRECT &rect,
    // out points
    IntPoint &p0, IntPoint &p1, IntPoint &p2)
  {
    const IntPoint PX = { 3, 0 }, PY = { 0, 3 };
    IntPoint L = { rect.L, rect.T }, RB = { rect.R - 1, rect.B - 1 };
    IntPoint S(RB - L);
    IntPoint TL = { L.X, L.Y }, TR = { L.X + S.X, L.Y };
    IntPoint BL = { L.X, L.Y + S.Y }, BR = { L.X + S.X, L.Y + S.Y };
    switch (c)
    {
    case 0: p0 = TL + PY, p1 = TL, p2 = TL + PX; break;
    case 1: p0 = TR + PY, p1 = TR, p2 = TR - PX; break;
    case 2: p0 = BL - PY, p1 = BL, p2 = BL + PX; break;
    case 3: p0 = BR - PY, p1 = BR, p2 = BR - PX; break;
    }
    gfx->DrawLine(colour, (float)p0.X, (float)p0.Y, (float)p1.X, (float)p1.Y);
    gfx->DrawLine(colour, (float)p1.X, (float)p1.Y, (float)p2.X, (float)p2.Y);
  }
  
  void DrawCropMarks(
    IGraphics *gfx, IRECT &rect,
    const IColor *colour,
    IntPoint mark = { 3, 3 })
  {
    IntPoint p0(0, 0), p1(0, 0), p2(0, 0);
    GetMarker(gfx, colour, 0, rect, p0, p1, p2);// TOP LEFT
    GetMarker(gfx, colour, 1, rect, p0, p1, p2);// TOP RIGHT
    GetMarker(gfx, colour, 2, rect, p0, p1, p2);// BOTTOM LEFT
    GetMarker(gfx, colour, 3, rect, p0, p1, p2);// BOTTOM RIGHT
  }
};

#endif
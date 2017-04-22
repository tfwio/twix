#ifndef __ICControls_h__
#define __ICControls_h__
/*
TO USE THIS FILE, YOU MUST FIRST INCLUDE YOUR OWN VERSION OF IPLUGINFO AS USED HERE.
FOR EXAMPLE IN "IPlugInfo.h" WE HAVE THE FOLLOWING TWO INCLUDES...

#include "IPlug_include_in_plug_hdr.h"
#include "ICControls.h"
*/
#if _WIN32
#pragma once
#endif

#ifndef _IPLUG_INCLUDE_HDR_
//  here for intellisense (just in case)
//  safe as long as its loaded before the cpp version (methink)
#   include "IPlug_include_in_plug_hdr.h"
#endif

#define IRRECT(x,y,w,h) IRECT(x, y, x + w, y + h)
#define BMP_FRAMES_H(pGFX,pBMP,rID,rFN,iFRAMES) pBMP = pGFX->LoadIBitmap(rID,rFN,iFRAMES,true)
#define BMP_FRAMES_V(pGFX,pBMP,rID,rFN,iFRAMES) pBMP = pGFX->LoadIBitmap(rID,rFN,iFRAMES,false)
#define ctl_rect(name,x,y,w,h) name.SetRect(x,y,w,h)
#define ctl_xy(name,x,y,r2) name.SetRect(x,y,r2.W(),r2.H())
#define __MaxCharCount__ 32

#ifndef use_iplug
// obsolete since its in PropertyPages (VS2013e)
#define use_iplug
#endif

//
// redundant IPLUG control includes
////////////////////////////////////////////////

#include "on.tfwio.h"

#define __l(vv,min,max) ((vv <= min) ? min : ((vv >= max) ? max : vv))

// a few functions that can be dropped into any IControl class.
#include "IRenderHelper.h"

#include "ICControl.h"
#include "IKnobText.h"
#include "ICPanelControl.h"
#include "IFloatNumberPanel.h"
#include "IKnobText.h"
#include "IBitmapButton.h"


#endif

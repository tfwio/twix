#ifndef __ICControls_h__
#define __ICControls_h__
/*
FOR EXAMPLE IN "IPlugInfo.h" WE HAVE THE FOLLOWING TWO INCLUDES...

#include "IPlug_include_in_plug_hdr.h"
#include "ICControls.h"
*/

#if !defined(_IPLUG_INCLUDE_HDR_)
#  if defined(show_inclusions)
#  pragma message("_IPLUG_INCLUDE_HDR_ from ICControls")
#  endif
// JUST IN CASE
#include "IPlug_inc_hpp.h"
#endif

#ifndef use_iplug
// obsolete since its in PropertyPages (VS2013e)
#define use_iplug
#endif

////////////////////////////////////////////////

#ifndef __tfw_ioh__
#  if defined(show_inclusions)
#  pragma message("why am I including on.tfwio.h from ICControls.h here?")
#  endif
#include "on.tfwio.h"
#endif

#include "iplug-macros.h"

// a few functions that can be dropped into any IControl class.
#include "IRenderHelper.h"


// in ICControls.cpp
//////////////////////////////////////////
#include "ICControl.h"
#include "IKnobText.h"
#include "IBitmapButton.h"
#include "ICPanelControl.h"
#include "IFloatNumberPanel.h"

#endif

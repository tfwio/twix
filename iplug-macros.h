#ifndef __iplug_macros_h__
#define __iplug_macros_h__

#define IRRECT(x,y,w,h) IRECT(x, y, x + w, y + h)
#define BMP_FRAMES_H(pGFX,pBMP,rID,rFN,iFRAMES) pBMP = pGFX->LoadIBitmap(rID,rFN,iFRAMES,true)
#define BMP_FRAMES_V(pGFX,pBMP,rID,rFN,iFRAMES) pBMP = pGFX->LoadIBitmap(rID,rFN,iFRAMES,false)
#define ctl_rect(name,x,y,w,h) name.SetRect(x,y,w,h)
#define ctl_xy(name,x,y,r2) name.SetRect(x,y,r2.W(),r2.H())
#define __MaxCharCount__ 32
#define __l(vv,min,max) ((vv <= min) ? min : ((vv >= max) ? max : vv))

#endif
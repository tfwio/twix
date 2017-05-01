#ifndef __tfw_defs__
#define __tfw_defs__

#ifndef __tfw_ioh__
#  if defined(show_inclusions)
#  pragma message("WARNING: redundant include \"on.tfio.h\" from \"on.defs.h\"")
#  endif
#include "on.tfwio.h"
#endif


#define ns2(a,b) namespace a { namespace b {
#define ns2_begin(a,b) namespace a { namespace b {
#define ns2_end }; };
//
#define ns3(a,b,c) namespace a { namespace b { namespace c {
#define ns3_begin(a,b,c) namespace a { namespace b { namespace c {
#define ns3_end() }; }; };

// define stuff (TRUE FALSE) before including this defs file.
#define xnum_isfloat 1
#define use_uni 1

#ifdef __cplusplus
namespace tfwio {
#endif // __cplusplus

  // section:BEGIN tfw_icu.h

  // what are you doing here?
#if defined(use_uni)
#  if defined(UNICODE) || defined(_UNICODE) && !defined(_MBCS)
#   undef UNICODE
#   undef _UNICODE
#  endif
#endif

// Seriously, you're not going to be using any of this.
#if defined(UNICODE)||defined(_UNICODE)&&!defined(_MBCS)
  typedef wchar_t mbstr_x, tstr;
#else
  typedef char    mbstr_x, tstr;
#endif

// see what I'm talking about? // and what is this doing here?
  // section:END tfw_icu.h
  // section:BEGIN tfw_defs.h
#ifndef xcall
# define xcall
#endif
#ifndef dcall
# define dcall inline
#endif

// no __wchar_t in mingw `typedef __wchar_t xtr, *xstr;  // and this`
typedef wchar_t xtr, *xstr;  // and this

// COME ON!  Really? ---but I guess this makes sense to switch between them
#if xnum_isfloat
#pragma message("NOTE: `xnum` is defined as float")
  typedef float xnum;
#elif xnum_isdbl
#pragma message("NOTE: `xnum` is defined as double")
typedef double xnum;
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef M_PI
  // square root of 2
#  define SQ2     1.4142135623730950488016887242097
#  define M_PI    3.1415926535897932384626433832795
#  define M_2PI   6.283185307179586476925286766559
  // 1/PI
#  define M_1DP   0.31830988618379067153776752674503
#  define M_1DPf  0.31830988618379067153776752674503f
  // 2/PI
#  define M_2DP   0.63661977236758134307553505349006
#  define M_2DPf  0.63661977236758134307553505349006f
  // 1/(2/PI)
#  define M_1D2DP 1.5707963267948966192313216916397
  // sin(2.0 * M_PI * f / fs)
#  define SNF(f,fs) sin(M_2PI*f/fs)
  // cos(2.0 * M_PI * f / fs)
#  define CSF(f,fs) cos(M_2PI*f/fs)
  // tan(2.0 * M_PI * f / fs)
#  define TNF(f,fs) tan(M_PI*f/fs)
#  define M_LN2  0.69314718055994530941723212145818
#  define M_LN2D 0.34657359027997265470861606072909


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ANGLE: http://www.calculatorsoup.com/calculators/conversions/angle.php
// 1 degree = 0.01745329 radians
// see _1rad for slope reference
#define _1deg 0.01745329
// actual value = 57.29578778556937
// 1 degree / 0.01745329 radians = 1
// SLOPE: http://resources.esri.com/help/9.3/ArcGISengine/java/gp_toolref/spatial_analyst_tools/how_slope_works.htm
// and if your bored, http://resources.esri.com/help/9.3/arcgisengine/java/gp_toolref/spatial_analyst_tools/how_aspect_works.htm
#define _1rad (1 / _1deg)
#endif
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CONTAIN(number,min,max) ((number < min) ? min : (number > max ? max : number))
/////////////////////////////////////////////////////////////////////////////////////////////////////////


typedef std::vector<xnum> vnum, *xvnum;
typedef std::vector<int>  vint, *xvint;
typedef std::vector<std::vector<xnum>> listnum, *xlistnum;
  /**
  * WTF?
  */
#ifndef usn
# define usn(n) defined(n) ? #n
#endif
  /** WTF?
  *
  */
#define nls(s) "\n    " s
#define def(iv) iv
#define def2(n,iv) n iv




// TYPES
/////////////////////////////////////////////////////////////////////////////////////////////////////////


  /**
  * Standard Types (Numeric)
  */
  typedef signed char   SB, sb, SI8, si8;
  typedef unsigned char UB, ub, UI8, ui8;
  typedef signed char   FB, fb;  //< EG: FB[nBits]; Signed, fixed-point bit value (nBits is the number of bits used to store the value)

  typedef signed short  SI16, si16;
  typedef signed __int32    SI32, si32;
  typedef unsigned __int16  UI16, ui16;
  typedef unsigned __int32  UI32, ui32;

  // Unsigned 24 bit ints (as far as the scope of this library)
  // are primarily for image types, though none of the image libary has
  // really been implemented.

#define MAXUI24 16777215
#define MAXUI24r MAXUI24+1 // (zero reclusive)
#define MINUI24 0
  // ============================================================================
  //  Various integer conversion utils
  //      I should have freaking documented these guys when I wrote them.
  //      Perhaps their usefulness is evident.
  //  Maybe
  //      um...
  // ============================================================================
  /**
  * Reverse a unary 24-bit integer
  */
  /**  */
# define _ui24r(v) ui32((v[2]<<16)|(v[1]<<8)|(v[0]))
  /**  */
# define _ui24(v) ui32((v[0]<<16)|(v[1]<<8)|(v[2]))
  /**  */
# define _si24r(v) si32((v[2]<<16)|(v[1]<<8)|(v[0]))
  /**  */
# define _si24(v) si32((v[0]<<16)|(v[1]<<8)|(v[2]))
  /**  */
# define _i24r(v) ui32(_ui24r(v))
  /**  */
# define _i24(v) ui32(_ui24(v))
  /**  */
# define _set24_b(v,b) ( b[0]=(v&0xFF0000)>>16, b[1]=(v & 0xFF00)>>8, b[2]=(v & 0xFF) )
  /**  */
# define _set24_br(v,b) ( b[2]=(v&0xFF0000)>>16, b[1]=(v & 0xFF00)>>8, b[0]=(v & 0xFF) )
  /**  */
# define _set24(v) int((v&0xFF0000)>>16|(v&0xFF00)>>8|(v&0xFF))

/////////////////////////////////////////////////////////////////////////////////////////////////////////

  /**
* 24 bit unsigned (unary) integer
*/
typedef struct t_ui24 {
public:
  union
  {
    ui8 ui24b[3];
    struct
    {
      ui8 lo, mid, hi;
    } ui24_range;
  };
  uint32 set_valuer(uint32 value) { _set24_br(value, ui24b); return get_value(); }
  uint32 set_value(uint32 value) { _set24_b(value, ui24b); return get_value(); }
  uint32 get_value() { return _ui24(ui24b); }
  uint32 get_rvalue() { return _ui24r(ui24b); }
  uint8 operator[](int value)
  {
    if ((value<3) && (value >= 0)) return this->ui24b[value];
    else return false;
  }
  //private:
  //  t_ui24() { }
  //  t_ui24(int ii32) { set_value(ii32); }
} ui24;

/**
* \brief This UI24 class was authored for image formats
* such as used by ADOBE in the encoding of IMAGE DATA.
*/
class UI24 : public ui24
{
public:
  UI24(void) { }
  UI24(int ii32) {
    set_value(ii32);
  }
  int operator =(UI24 value)
  {
    return value.get_value();
  }
  UI24 operator =(int value)
  {
    return UI24(value);
  }
  /*template<class T>
  T operator =(UI24 value)
  {
  if ()
  switch (value)
  {
  default: return (T)dynamic_cast<T>(this->get_value());
  }
  return (T)false;
  }*/
  //UI24 operator =(float value) { UI24 val; val.set_value((ui32)value); return val; }
  uint8 operator[](int value)
  {
    if ((value<3) && (value >= 0)) return this->ui24b[value];
    else return false;
  }
};

typedef UI24 ui_24, UI_24;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// experimental
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef clr_cli_emu
  typedef unsigned long long int UInt64;
  typedef long long int Int64;
  // Emulating System::String would probably take quite a bit of work,
  // as well as needing a bit of help from something along the terms
  // of CultureInfo as defined by the CLR/CLI.
  // I should probably take a good look at .NET specifications
  // in order to see how such things are 'natively' handled.
#endif

typedef unsigned long long UI64, ui64;
typedef signed   long long SI64, si64;
typedef /*unsigned*/ long double F64, f64;

typedef float   FLOAT16, float16, F16, f16;
typedef double  FLOAT32, float32, F32, f32;

// just checking
//typedef UI16  FLOAT; // is in WinDef.h as a float of course

typedef unsigned char UBIT, ubit;
typedef signed char   XBIT, xbit, CHID[4];
typedef unsigned short ushort;
typedef signed short  sshort;

typedef unsigned int
  ulong, // ?why is this unsigned int?
  uint;  // please don't use this

typedef signed int    sint;

// code simplification helper (dont)
#ifndef t_union
# define t_union(n) typedef union n
#endif

  // code simplification helper (dont)
#ifndef t_struct
# define t_struct(n) typedef struct n
#endif

  //  standard &= type check utility macro
#ifndef ckf
# define ckf(v1,v2) ((v1 & v2) == v2)
#endif
//  (I don't think this is used anywhere)
#ifndef _i
# define _i(v) ((uint8)v)
#endif
/*
macro utility to lookup enumeration types defined in their respective helper content
most enums defined in this library have 'int x1[#len]' and 'char *x2[#len]' helpers.
if (v1&=v2!=0) (denoted by 'ckf macro' ... ) this retuns the associated char *string.
*/
# define _xi(v1,v2,x1,x2,n) ckf(v1,v2) ? x1[n] : x2[n]

// 32-Bit little-endian swapper (probably un-referenced and bugged... should probably push back 24bits and not 20)
# define i_4(vvx)( (uint32)(vvx[0]<<20|vvx[1]<<16|vvx[2]<<8|vvx[3]) )

// 16-Bit little-endian swapper (probably un-referenced and bugged)
# define i_2(vv)( uint16(vvx[0]<<8|vvx[1]) )

//  16-Bit (working me-thinks) endian swap
# define endo_ui16(v) (uint16(v[0]<<8|v[1]))

//  32-Bit (working me-thinks) endian swap
# define endo_ui32(v) (uint32(v[0]<<24|v[1]<<16|v[2]<<8|v[3]))

// section:END tfw_defs.h

} // end tfwio namespace

#endif


// you must include <on.tfwio.h> and <on.defs.h>
#ifndef __tfw_ioh__
#define __tfw_ioh__
#pragma once

// we'll probably never use this but here is
#ifdef __dll
//#define decl_impl export(_cdecl)
#define decl_impl export(_stdcall)
#else
#define decl_impl export
#endif

typedef unsigned char    uint8;
typedef signed char      int8;
typedef unsigned __int16 uint16;
typedef signed __int16 int16;
typedef unsigned __int32 uint32;
typedef signed __int32   int32;
typedef unsigned __int64 uint64;
typedef signed __int64   int64;

#ifdef use_console
#  if defined(show_inclusions)
#  pragma message("USING: use_console = 1")
#  endif
#define use_console 1
#else
#  if defined(show_inclusions)
#  pragma message("USING: use_console = 0")
#  endif
#define use_console 0
#endif

#include <string>
#include <tchar.h>

#include <conio.h>
#include <stdio.h>

#include <iostream>
#include <cstdlib>


#include <set>
#include <vector>

//#define use_iplug
//#undef use_iplug
#if defined(use_iplug) && !defined(_IPLUGSTRUCTS_)
#include "IPlugStructs.h"
#endif

#include "on.defs.h"

// #include "snd.h" // selected (WAVEFORMAT) headers were included
#define for_points(points,int_to_iterate,int_start) for (int int_to_iterate = int_start; int_to_iterate < points.size(); i++)


#include "on.std_fs.h"
#endif
//EOF//////////////////////////////////////////////////////////////////////////////////

//
namespace drawing {
  struct intpoint {
    int X, Y;
    void set(int x, int y) { X = x; Y = y; } // same as copy
    void set(intpoint p) { X = p.X; Y = p.Y; } // same as copy
    // obsolete. use set.  copy is for intpoint
    //void from(int x, int y) { X = x; Y = y; } // same as set
    void from(intpoint p) { X = p.X; Y = p.Y; }
    //intpoint copy(int x, int y) { return intpoint(x, y); } // same as set
    //intpoint copy(intpoint p) { return intpoint(p.X, p.Y); }

    void roll_y(int x, int y, IRECT &sizeRef)
    {
      if (y >= sizeRef.B) Y -= sizeRef.H();
      else if (y < sizeRef.T) Y += sizeRef.H();
    }

    intpoint() : intpoint(0,0) { }
    intpoint(int x, int y) { X = x; Y = y; }

    int compare(int input) {
      if (input < 0) return -1;
      if (input == 0) return 0;
      return 1;
    }
    // compare works like you might expect a `sign(float)`,
    // solving for X and Y respectively.
    const intpoint compare() { return intpoint(compare(X), compare(Y)); }

    const intpoint mult(int x, int y) { return intpoint(X * x, Y * y); }
    const intpoint mult(const intpoint& p) { return intpoint(X * p.X, Y * p.Y); }
    const intpoint divi(int x, int y) { return intpoint(X / x, Y / y); }
    const intpoint divi(const intpoint& p) { return intpoint(X / p.X, Y / p.Y); }
    const intpoint  add(int x, int y) { return intpoint(X + x, Y + y); }
    const intpoint  add(const intpoint& p) { return intpoint(X + p.X, Y + p.Y); }
    const intpoint subt(int x, int y) { return intpoint(X - x, Y - y); }
    const intpoint subt(const intpoint& p) { return intpoint(X - p.X, Y - p.Y); }

    const intpoint operator*(const intpoint& p) { return mult(p); }
    const intpoint operator/(const intpoint& p) { return divi(p); }
    const intpoint operator+(const intpoint& p) { return  add(p); }
    const intpoint operator-(const intpoint& p) { return subt(p); }
    const bool operator<(const intpoint& p) { return this->X < p.X; }
    const bool operator>(const intpoint& p) { return this->X > p.X; }
    const bool operator==(const intpoint& p) { return this->X == p.X && this->Y == p.Y; }
  };
  //tern struct IRECT;
  struct floatpoint {

    float X, Y;
    floatpoint() : floatpoint(0.0f, 0.0f) {}
    floatpoint(float x, float y) { X = x; Y = y; }

    const floatpoint mult(const floatpoint& p) { return floatpoint(X * p.X, Y * p.Y); }
    const floatpoint divi(const floatpoint& p) { return floatpoint(X / p.X, Y / p.Y); }
    const floatpoint  add(const floatpoint& p) { return floatpoint(X + p.X, Y + p.Y); }
    const floatpoint subt(const floatpoint& p) { return floatpoint(X - p.X, Y - p.Y); }
    const floatpoint operator*(const floatpoint& p) { return mult(p); }
    const floatpoint operator/(const floatpoint& p) { return divi(p); }
    const floatpoint operator+(const floatpoint& p) { return  add(p); }
    const floatpoint operator-(const floatpoint& p) { return subt(p); }
  };
  struct doublepoint {
    double X, Y;
    doublepoint() : doublepoint(0.0f, 0.0f) {}
    doublepoint(double x, double y) { X = x; Y = y; }
    const doublepoint mult(const doublepoint& p) { return doublepoint(X * p.X, Y * p.Y); }
    const doublepoint divi(const doublepoint& p) { return doublepoint(X / p.X, Y / p.Y); }
    const doublepoint  add(const doublepoint& p) { return doublepoint(X + p.X, Y + p.Y); }
    const doublepoint subt(const doublepoint& p) { return doublepoint(X - p.X, Y - p.Y); }
    const doublepoint operator*(const doublepoint& p) { return mult(p); }
    const doublepoint operator/(const doublepoint& p) { return divi(p); }
    const doublepoint operator+(const doublepoint& p) { return  add(p); }
    const doublepoint operator-(const doublepoint& p) { return subt(p); }
  };
  struct floatrect {
    using FloatPoint = drawing::floatpoint;
    FloatPoint Location;
    FloatPoint Size;

    float L() { return Location.X; }
    float X() { return Location.X; }

    float Y() { return Location.Y; }
    float T() { return Location.Y; }

    float B() { return Location.Y + Size.Y; }
    void SetB(float bottom) { Size.Y = bottom - Y(); }

    float R() { return Location.X + Size.X; }
    void SetR(float r) { Size.X = r - X(); }


    floatrect(FloatPoint location, FloatPoint size) : Location(location), Size(size) { }
    floatrect(float x, float y, float w, float h) : Location(x, y), Size(w, h) { }
  };
  struct doublerect {
    using DoublePoint = drawing::doublepoint;
    DoublePoint Location;
    DoublePoint Size;

    double L() { return Location.X; }
    double X() { return Location.X; }

    double Y() { return Location.Y; }
    double T() { return Location.Y; }

    double B() { return Location.Y + Size.Y; }
    double SetB(double bottom) { Size.Y = bottom - Y(); }

    double R() { return Location.X + Size.X; }
    void SetR(double r) { Size.X = r - X(); }


    doublerect(DoublePoint location, DoublePoint size) : Location(location), Size(size) { }
    doublerect(double x, double y, double w, double h) : Location(x, y), Size(w, h) { }
  };
  struct intrect {
    intpoint Location;
    intpoint Size;

    void Move(intpoint p) { Location = Location + p; }
    void MoveTo(intpoint p) { Location.X = p.X, Location.Y = p.Y; }
    void MoveTo(intrect p) { Size.X = p.X(), Size.Y = p.Y(); }
    void MoveTo(int x, int y) { Size.X = x, Size.Y = y; }
    void MoveX(intpoint p) { Location.X += p.X; }
    void MoveY(intpoint p) { Location.Y += p.Y; }
    int L() { return Location.X; }
    int X() { return Location.X; }
    int R() { return Location.X + Size.X; }
    void  SetR(int r) { Size.X = r - X(); }

    void ScaleTo(intpoint p) { Size.X = p.X, Size.Y = p.Y; }
    void ScaleTo(intrect p) { Size.X = p.X(), Size.Y = p.Y(); }
    void ScaleTo(int w, int h) { Size.X = w, Size.Y = h; }
    void ScaleX(intpoint p) { Size.X += p.X; }
    void ScaleY(intpoint p) { Size.Y += p.Y; }
    int Y() { return Location.Y; }
    int T() { return Location.Y; }
    int B() { return Location.Y + Size.Y; }
    void SetB(int bottom) { Size.Y = bottom - Y(); }
    void Set(int x, int y, int w, int h) { this->MoveTo(x, y); this->ScaleTo(w, h); }

    #ifdef use_iplug
    void Set(IRECT &rect) { intrect::MoveTo(rect.L, rect.T); intrect::ScaleTo(rect.W(), rect.H()); }

    // not within critical loop: IRECT auto-conversion
    operator const IRECT(){ return IRECT(Location.X, Location.Y, T(), R()); }

    // .CTOR from IRECT
    intrect(IRECT &rect) : Location(rect.L, rect.T), Size(rect.W(), rect.H()) { }
    // so for IRECT to be convertable to intpoint without help,
    // the operator would have to be written in IPlugStructs.h
    #endif

    intrect(intpoint location, intpoint size) : Location(location), Size(size) { }
    intrect(int x, int y, int w, int h) : Location(x, y), Size(w, h) { }
  };
};
//
typedef drawing::intpoint IntPoint, *IntPoint_t;
typedef drawing::floatpoint FloatPoint, *FloatPoint_t;
typedef drawing::floatrect FloatRect, *FloatRect_t;
typedef drawing::doublepoint DoublePoint;
typedef drawing::doublerect DoubleRect;
typedef drawing::intrect IntRect, *IntRect_t;

//pedef std::vector<FloatPoint> PointCollection;

class PointCollection : public std::vector<FloatPoint>
{
  #define str_vector_iterator(int_iter,str_vec) for (std::vector<std::string>::const_iterator int_iter = str_vec.begin(); int_iter < str_vec.end(); int_iter++)
  template < class ContainerT >
  //__attribute__((always_inline)) or __forceinline or_no inline
  void tokenize(
    const std::string& str,
    ContainerT& tokens,
    const std::string& delimiters = " ",
    bool trimEmpty = false) {
    std::string::size_type pos, lastPos = 0, length = str.length();

    using value_type = typename ContainerT::value_type;
    using size_type = typename ContainerT::size_type;

    while (lastPos < length + 1)
    {
      pos = str.find_first_of(delimiters, lastPos);
      if (pos == std::string::npos) pos = length;

      if (pos != lastPos || !trimEmpty)
        tokens.push_back(value_type(str.data() + lastPos,
        (size_type)pos - lastPos));

      lastPos = pos + 1;
    }
  }
public:

  void FromString(const std::string & sentence = "0,90 1,120 2,230 3,150")
  {
    std::vector<std::string> data; // attempt tokenization of the sentence.
    tokenize(sentence, data, " ");
    str_vector_iterator(i, data)
    {
      size_t
        b = i->find_first_of(','),
        e = i->size();
      std::string
        x = i->substr(0, b),
        y = i->substr(b + 1, i->size() - 1);
      insert(end(), FloatPoint(strtof(x.c_str(), NULL), strtof(y.c_str(), NULL)));
    }
  }
  void ToString(std::string *str) {
    str->clear();
    int lastpoint = size() - 1;
    for (int i = 0; i < size(); i++)
    {
      str->append(std::to_string(at(i).X));
      str->append(",");
      str->append(std::to_string(at(i).Y));
      if (i < lastpoint) str->append(" ");
    }
  }

  PointCollection(int size) : std::vector<FloatPoint>(size)
  {
  }
  PointCollection() : PointCollection(12) {
  }
};

//
class StrUtil {
public:
  //void charToWchar(char* input, wchar_t *output, size_t length)
  //{
  //  mbstowcs(output, input, 256);
  //}

  /*inline*/ static char *wchar2char(wchar_t *input, int charMax) {
    char *filen_char;
    filen_char = (char*)malloc(256);
    wcstombs(filen_char, input, 256);
    wprintf(L"file: '%s'\n", input);
  }

  // duplicated into PointCollection
  template < class ContainerT >
  static void tokenize(
    const std::string& str,
    ContainerT& tokens,
    const std::string& delimiters = " ",
    bool trimEmpty = false) {
    std::string::size_type pos, lastPos = 0, length = str.length();

    using value_type = typename ContainerT::value_type;
    using size_type = typename ContainerT::size_type;

    while (lastPos < length + 1)
    {
      pos = str.find_first_of(delimiters, lastPos);
      if (pos == std::string::npos) pos = length;

      if (pos != lastPos || !trimEmpty)
        tokens.push_back(value_type(str.data() + lastPos,
        (size_type)pos - lastPos));

      lastPos = pos + 1;
    }
  }
  //
  // String trim.
  // See: http://stackoverflow.com/questions/25829143/c-trim-whitespace-from-a-string
  static std::string trim(std::string & str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return "";

    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
  }

  // while this method splits some points, we could also just
  // (rather) call `StrUtil::SplitCDF(points, inputStr)` to set the data.
  // Since this method is static, I'm thinking it would be wise to be rid of
  // adding the PointCollection to the stack.
  static PointCollection GetPoints(const std::string & sentence = "0,90 1,120 2,230 3,150") {
    PointCollection points;
    StrUtil::SplitCDF(points, sentence);
    // cout << "counted " << points.size() << " points" << endl << endl;
    // for (int i = 0; i < points.size(); i++)
    //   cout << "- X=" << points.at(i).X << ", Y=" << points.at(i).Y << endl;
    // cout << endl; // blank space;
    // cout << endl; // blank space;
    return points;
  }

  static void SplitCDF(PointCollection & pc, const std::string & sentence = "0,90 1,120 2,230 3,150") {

    std::vector<std::string> data;

    // attempt tokenization of the sentence.
    StrUtil::tokenize(sentence, data, " ");

    for (std::vector<std::string>::const_iterator i = data.begin(); i < data.end(); i++)
    {
      size_t b = i->find_first_of(','), e = i->size();
      std::string x = i->substr(0, b), y = i->substr(b + 1, i->size() - 1);
      pc.insert(pc.end(), FloatPoint(strtof(x.c_str(), NULL), strtof(y.c_str(), NULL)));
    }
  }

  static void StrSplitCDF(std::vector<std::string> & data, const std::string & sentence = "0,90 1,120 2,230 3,150") {
    PointCollection c;

    // attempt tokenization of the sentence.
    StrUtil::tokenize(sentence, data, " ");

    for (std::vector<std::string>::const_iterator i = data.begin(); i < data.end(); i++)
    {
      size_t b = i->find_first_of(','), e = i->size();
      std::string x = i->substr(0, b), y = i->substr(b + 1, i->size() - 1);

      std::cout << "{x=" << x << ", " << "y=" << y << "}" << std::endl;

      //cout << "  - " << i->c_str() << endl;
      //cout << "  - b = " << ((b != std::string::npos) ? b : std::string::npos) << endl;
    }
  }
  static void cdf2intpt(std::vector<std::string> & data, const std::string & sentence = "0,90 1,120 2,230 3,150") {

    // attempt tokenization of the sentence.
    StrUtil::tokenize(sentence, data, " ");

    std::vector<std::string>::const_iterator i;

    for (i = data.begin(); i < data.end(); i++)
    {
      size_t b = i->find_first_of(','), e = i->size();
      std::string x = i->substr(0, b), y = i->substr(b + 1, i->size() - 1);

      std::cout << "{x=" << x << ", " << "y=" << y << "}" << std::endl;
      //cout << "  - " << i->c_str() << endl;
      //cout << "  - b = " << ((b != std::string::npos) ? b : std::string::npos) << endl;
    }
  }

  //  2109876543210
  //  3210987654321
  // '654,321.12345'
  //  109876543210
  //  210987654321
  // '654321.12345'
  // char[13]+'\0'

  #define __x_max_str_x__ 32

  //static std::string float2str(double nom)
  //{
  //  char str[__x_max_str_x__];
  //  sprintf_s(str, __x_max_str_x__, "%.5f\0", nom);
  //  std::string sal = str;
  //  long numr = nom;
  //  std::string cs = str;
  //  sal.insert(sal.begin(), 12 - sal.length(), '0');
  //  sal.insert(3, ",");
  //  return sal;
  //}

  static std::string float2str_alt(double nom, char pad = '0', bool comma = true, const int d = 5)
  {
    char str[__x_max_str_x__];
    const char *flo = "%.6f";
    //flo[2] = (char)(30 + d + 1);
    sprintf_s(str, __x_max_str_x__, flo, nom);
    //"9999.999999"
    double intpart;
    modf(nom, &intpart);
    std::string ck = std::to_string(int(intpart));
    const int ncount = d > 0 ? d + 1 : d; // 1 (=the decimal point) + 5 or d (=number of digits past the decimal) - 1 (zero inclusive index but where does -1 occur?)
    str[ck.length() + ncount] = '\0';

    std::string sal = str;
    //long numr = long(nom);
    std::string cs = str;
    sal.insert(sal.begin(), 12 - sal.length(), pad);// 12 chars = '000000.00000'
    if (comma) sal.insert(3, ",");
    return sal;
  }
  // we want to use the above since it formats without rounding
  // using "%.6f\0" compared to "%.5f\0", then it cleans up the value returned.
  // (but not this method)
  static std::string float2str(double nom, char pad='0', bool comma=true, const int d=5)
  {
    char str[__x_max_str_x__];
    char *flo = "%.5f\0";
    flo[2] = (char)(30 + d);
    sprintf_s(str, __x_max_str_x__, flo, nom);
    std::string sal = str;
    //long numr = int(nom);
    std::string cs = str;
    sal.insert(sal.begin(), 12 - sal.length(), pad);
    if (comma) sal.insert(3, ",");
    return sal;
  }
};

//
namespace on {
namespace io {

  /////////////////////////////////////////////////////////////////////////////
  /**
  * @brief template <typename T, typename Data> class typ_TPL
  * Thus far used only in SMF Rich Interchange implementation in lib_snd.
  * It contains a descriptor chunk and a Data chunk both of which should
  * remain private in a inheriting class, but apparently they are not hidden
  * here and perhaps another template is in order requiring concealment
  * of these variables---perhaps not.
  */
  template <typename T, typename Data>
  class typ_TPL {
  public:
    T		ckHead;
    Data	*ckData;
  };

  // is our decimal or flag type; �whatever is being stored within
  // our data3 or `TDecimal***` three dimensional pointer.
  // this is experimental designed to create and manage an array
  // such as short[n][o][p];
  template<typename TDecimal> class TPtr3D {
  public://inline static
    static TDecimal** init_data2(int x, int y) {
      int i, j, k;
      TDecimal **data;
      try {
        data = (TDecimal **)malloc(sizeof(TDecimal*) * x);
        std::cout << "malloc 1 complete" << std::endl;
      }
      catch (char * message) { std::cout << "unknown error (malloc 1)" << std::endl; }
      try {
        for (k = 0; k < x; k++) data[k] = (TDecimal*)malloc(sizeof(TDecimal) * y);
        std::cout << "malloc 2 complete" << std::endl;
      }
      catch (char * message) { std::cout << "unknown error (malloc 2)" << std::endl; }
      try {
        for (i = 0; i < x; i++) {
          for (j = 0; j < y; j++) {
            data[i][j] = (int)((i << 16) | (j << 8));
          }
        }
        std::cout << "iter complete" << std::endl;
      }
      catch (char * message) { std::cout << "unknown error (iteration)" << std::endl; }
      return data;
    }
    /*
    // there really is no jm?
    static void free_data2(TDecimal **data) {
      int i, j;
      //for (k = 0; k < km; k++)
      for (i = 0; i < im; i++) for (j = 0; j < jm; j++) free(data[i][j]);
      for (i = 0; i < im; i++) free(data[i]);
      free(data);
    }
    */
    // malloc() is called on `TDecimal data[x][y][z]` (such as `TDecimal ***data`)
    // at each x, y and z `TDecimal` level.
    static TDecimal*** init_data3(int x, int y, int z) {
      int i, j, k;
      TDecimal ***data = (TDecimal***)malloc(sizeof(TDecimal**) * x);
      try {
        for (i = 0; i < x; i++) {
          data[i] = (TDecimal**)malloc(sizeof(TDecimal*) * y);
          for (j = 0; j < y; j++)
            data[i][j] = (TDecimal*)malloc(sizeof(TDecimal) * z);
          //data[i][j] = (TDecimal*)malloc(sizeof(TDecimal*) * z);
        }
      }
      catch (char * message) { std::cout << "unknown error (malloc 2-3)" << std::endl; }
      return data;
    }
    // Store hexadecimal value in three rows bit shifted 8 bits per int.
    // � x=1, y=2, z=3 would appear in hex: 0x00010203
    // � x=256, y=2, z=128 would appear in hex: 0x00FF7F03 or something
    static void set_data3(TDecimal*** data, int x, int y, int z) {
      int i, j, k;
      try {
        for (i = 0; i < x; i++) for (j = 0; j < y; j++) for (k = 0; k < z; k++)
          data[i][j][k] = ((i << 16) | (j << 8) | k);
      }
      catch (char * message) { std::cout << "unknown error (iteration)" << std::endl; }
    }
    // it appears to be enough to free an int `short[n][o][p]` array
    // starting with second depth `free(data[n][o])` and `free(data)`.
    // otherwise the method generally doesn't act on a single
    // `(short)data[n][o][p]` value.
    // Not exactly the precision I'd been looking for, but dynamic.
    static void free_data3(TDecimal ***data, int im, int jm, int km) {
      int i, j;
      //for (k = 0; k < km; k++)
      for (i = 0; i < im; i++) for (j = 0; j < jm; j++) free(data[i][j]);
      for (i = 0; i < im; i++) free(data[i]);
      free(data);
    }
  };
  // static int *** test2(int x, int y, int z) {
  //   int i, j, k;
  //   int data[3][2][5];
  //   for (i = 0; i < x; i++) for (j = 0; j < y; j++) for (k = 0; k < z; k++) {
  //     data[i][j][k] = (int)((i * 10000) | (j * 100) | k);
  //   }
  //   return (int***)data;
  // }

  //
  typedef enum {
    kt_null = 0, kt_unknown
    // unknown > 0 < kt_bool
    , kt_string , kt_chars
    // > kt_chars < kt_int16
    , kt_bool
    , kt_int16, kt_short = kt_int16
    , kt_uint16, kt_ushort = kt_uint16
    , kt_int32
    , kt_int64, kt_long = kt_int64
    , kt_float/*32*/,  kt_double/*64*/
    , kt_count
  }
  // for parsing string values into respective type depending on the
  // TypeN::ky_float or type requested.
  TypeN;


  //
  // Default = TypeN
  //
  // Enumeration or integer type which can store layers and perhaps
  // using external helper functions decode different flag values
  // from such as directory-flags or file-system-attributes or some-such.
  template <typename TKeyGroup = TypeN>
  // Used as an intermediary string type where we can easily convert
  // between various formats and our string value here.
  // I suppose we're missing parsing features for such things as times
  // and dates.
  //
  // Note: double precision conversions may be effected by
  // compiler language/locality at compile time.
  struct __str_value_t {

    // standard types
    TKeyGroup   Type;

    // Our default value, stored as std::string.
    std::string Value;

    __str_value_t() { }
    //__str_value_t(TKeyGroup t) : Type(NULL) { }
    __str_value_t(char   *value) : Type(TypeN::kt_chars), Value(std::string(value)){}
    __str_value_t(std::string  value) : Value(std::string(value)) {  }
    __str_value_t(double  value) : Type(TypeN::kt_double), Value(std::to_string(value)) {  }
    __str_value_t(float   value) : Type(TypeN::kt_float), Value(std::to_string(value)) {  }
    __str_value_t(int     value) : Type(TypeN::kt_int32), Value(std::to_string(value)) {  }
    __str_value_t(long    value) : Type(TypeN::kt_int64), Value(std::to_string(value)) {  }
    __str_value_t(short   value) : Type(TypeN::kt_int16), Value(std::to_string(value)) {  }

    void set_value(char   *value)      { Type = TypeN::kt_chars; Value = std::string(value); }
    void set_value(std::string  value) { Type = TypeN::kt_string; Value = std::string(value); }
    void set_value(double  value)      { Type = TypeN::kt_double; Value = std::to_string(value); }
    void set_value(float   value)      { Type = TypeN::kt_float; Value = std::to_string(value); }
    void set_value(int     value)      { Type = TypeN::kt_int32; Value = std::to_string(value); }
    void set_value(long    value)      { Type = TypeN::kt_long; Value = std::to_string(value); }
    void set_value(short   value)      { Type = TypeN::kt_int16; Value = std::to_string(value); }

    double   Double(){ return    atof(Value.c_str()); }
    float     Float(){ return  strtof(Value.c_str(), NULL); }
    char      *Char() { return (char *)Value.c_str(); }
    char      *Text(){ return (char *)Value.c_str(); }
    int       Int32(){ return    atoi(Value.c_str()); }
    long long Int64(){ return   atoll(Value.c_str()); }

    operator const      double(){ return Double(); }
    operator const       float(){ return Float(); }
    operator const         int(){ return Int32(); }
    operator const   long long(){ return Int64(); }
    operator const       char*(){ return Text(); }
    operator const std::string(){ return std::string(Value); }
  };
  //
  struct ini_section {
    TypeN type;
    std::string key, val;

    void set_value(double  value, TypeN type=TypeN::kt_double) { this->type = type; val = std::to_string(value); }
    void set_value(float   value, TypeN type=TypeN::kt_float)  { this->type = type; val = std::to_string(value); }
    void set_value(int     value, TypeN type=TypeN::kt_int32)  { this->type = type; val = std::to_string(value); }
    void set_value(long    value, TypeN type=TypeN::kt_long)   { this->type = type; val = std::to_string(value); }
    void set_value(short   value, TypeN type=TypeN::kt_int16)  { this->type = type; val = std::to_string(value); }

    double Double() { return atof(val.c_str()); }
    float  Float()  { return strtof(val.c_str(), NULL); }
    char  *Text()   { return (char *)val.c_str(); }
    int    Int32()  { return atoi(val.c_str()); }

    ini_section(std::string k, TypeN t, std::string v)
      : key(k), type(t), val(v)
    {
    }

  };
  //
}; };
//
typedef on::io::TPtr3D<long>    long3d;
typedef on::io::TPtr3D<short>   short3d;
typedef on::io::TPtr3D<float>   float3d;
typedef on::io::TPtr3D<int>     int3d;
//
typedef on::io::__str_value_t<on::io::TypeN> StrValue;



//
namespace tfwio {
  typedef std::string string;

  // Oh.  I was going to write something that shouldn't be here.
  struct f_range { uint64 start; uint64 length; };
  //
  class fs_base {
  private:
    wchar_t *file_nameW;
    char    *file_nameA;
    bool     is_unicode;
  protected:
    FILE  *file_pointer;

    //static fs_base file_create(const char* fname);
    //static fs_base file_create(const wchar_t* fname);
  public:

    // set_fileA set_fileW

    /** \brief Set (char*) 'fname' to value.
    * If AutoLoad is enabled, then also begins reading the file
    * via a overloaded method for 'open_file_rbA()'.
    * \param fname (char*) file-name string.
    * \param autoload If true, automatically calls "open_file_rbW()".
    */
    virtual void set_fileA(const char *fname, bool autoload = false) {
      file_nameA = (char*)fname;
      if (autoload) open_file_rbA();
    }

    /** \brief Set (wchar_t*) 'fname' to value.
    * If AutoLoad is enabled, then also begins reading the file
    * via a overloaded method for 'open_file_rbA()'.
    * \param fname (wchar_t*) file-name string.
    * \param autoload If true, automatically calls "open_file_rbW()".
    */
    virtual void set_fileW(const wchar_t *fname, bool autoload = false) {
      file_nameW = (wchar_t*)fname;
      if (autoload) open_file_rbW();
    }

    /**
    *  void set_fileW(char *fname);
    *  void open_file(char *fname);
    *  void open_fileb(char *fname);
    *  virtual void lock_file() = 0;
    *  void open_file_r(wchar_t *fname);
    *  int j = fread( m_file.ckHead, isize, 1, fp);
    */

    // lock_file
    /** \brief Not implemented.
    */
    virtual void lock_file() {};
    // close_file
    /** \brief close_file();
    * It should be obvios what this method does.
    */
    void close_file() {
      if (file_pointer != nullptr) {
        fclose(file_pointer);
      }
      file_pointer = 0;
    }
    // open_file_rA open_file_rW open_file_rbA open_file_rbW
    /** \brief Loads a standard ansi file with read privelages */
    void open_file_rA() {
      file_pointer = fopen(file_nameA, "r" /*"a"*/);
      is_unicode = false;
    }

    /** \brief loads wide character file with read privelages */
    void open_file_rW() {
      file_pointer = _wfopen(file_nameW, L"r" /*"a"*/);
      is_unicode = true;
    }

    // just an 'r' would do for getting the binary memory we're interestd in.
    // this may have been working in prior c compiler environments (i've used)

    /** \brief Unknown b flag implemented in file-mode (loads a file with read access). */
    void open_file_rbA() {
      file_pointer = fopen(file_nameA, "rb" /*"a"*/);
      is_unicode = false;
    }

    /** \brief binary b flag implemented in file-mode (loads a wide-char file with read access). */
    long open_file_rbW() {
      file_pointer = _wfopen(file_nameW, L"rb" /*"a"*/);
      is_unicode = true;
      printf("fs_base::open_file_rbW()\n");
      return get_length();
    }

    /** \brief binary b flag implemented in file-mode (loads a wide-char file with read access). */
    long open_file_textW() {
      _wfopen_s(&file_pointer, file_nameW, L"r,ccs=UTF-8" /*"a"*/);
      is_unicode = true;
      return get_length();
    }
    // get_position get_length

    /** \brief Unknown b flag implemented in file-mode (loads a wide-char file with read access). */
    long get_position() {
      if ((is_unicode ? file_nameW == 0 : file_nameA == 0)) return false;
      if (file_pointer == 0) return false;
      fpos_t pp;
      fgetpos(file_pointer, &pp);
      return (long)pp;
    }
    /** \brief Self explanitory method */
    long get_length() {
      long pos = get_position();
      fseek(file_pointer, 0, SEEK_END);
      long len = ftell(file_pointer);
      fseek(file_pointer, pos, 0); // returns the buffer where it started
      return len;
    }

    void seek(long position)
    {
      if (file_pointer == 0) std::wcout << L"Shit! the file wasn't opened." << std::endl;
      fseek(file_pointer, position, SEEK_END);
    }
    // Defintions & Macros for UNICODE flavor
    // Defintions of UNICODE vs non-UNICODE character strings for file name.
    ///////////////////////////////////////////////////////////////////////
    // fairly rediculous...
#if !defined(use_uni)
#  define set_file     set_fileA
#  define open_file_r  open_file_rA
#  define open_file_rb open_file_rbA
#  define get_position get_positionA
#else
#  define set_file     set_fileW
#  define open_file_r  open_file_rW
#  define open_file_rb open_file_rbW
#  define get_position get_position64
    //#  define get_position get_positionW
#endif
//#ifdef _WIN64
//    //  experimental
//    int64 fs_base::get_length64() { uint64 pos = get_position(); _fseeki64(file_pointer, 0, SEEK_END); uint64 len = _ftelli64(file_pointer); _fseeki64(file_pointer, pos, 0); /*returns the buffer where it started*/ return len; }
//    /*ui64 get_positionA() { if (file_nameA==0) return false; if (file_pointer==0) return false; fpos_t pp; fgetpos( file_pointer, &pp ); return (ui64)pp; }*/
//    uint64 fs_base::get_position64() { if (file_nameW == 0) return false; if (file_pointer == 0) return false; fpos_t pp; fgetpos(file_pointer, &pp); return (ui64)pp; }
//#else
//#endif
  };
  //#pragma warning Note that compiler GPP flag - fpermissive needs to be set in order for (T)malloc(ranger->length) to be allowed
  //
  // Not Implemented? Good ;)
  template<typename T>
  // Note that compiler GPP flag -fpermissive needs to be set in order for
  // (T)malloc(ranger->length) to be allowed
  class file_reader : public fs_base {
  public:
    T get_range(long start, long length, bool r2seek = false)
    {
      //#pragma warning "-fpermissive compiler option must be set."
      T      buffer = (T)std::malloc(length);
      size_t fbits = fread(buffer, 1, length, file_pointer);
      return buffer;
    }
    // also not implemented (properly)
    T get_range(f_range *ranger, bool r2seek = false)
    {
      //#pragma warning "-fpermissive compiler option must be set."
      T      cbuffer = (T)malloc(ranger->length);
      size_t fbits = fread(cbuffer, 1, ranger->length, file_pointer);
      return cbuffer;
    }
  };
  //
  // Not Implemented? Good ;)
  class ini_io : public fs_base {
  public:
    ini_io(wchar_t *file_in, bool autoLoad = false) {
      fs_base::set_fileW(file_in, autoLoad);
    }
    // RETURN BUFFER SIZE.
    // DO NOT FORGET TO DE-ALLOCATE THE BUFFER!
    long get_bufferw(wchar_t *buffer, long maxSize) {
      long filesize = get_length();
      buffer = (wchar_t*)malloc(filesize); // the returned data
      fread(buffer, sizeof(wchar_t), filesize < maxSize ? filesize : maxSize, file_pointer);
      return filesize;
    }
    // RETURN BUFFER SIZE.
    // DO NOT FORGET TO DE-ALLOCATE THE BUFFER!
    long get_buffer(char *buffer, long maxSize) {
      long filesize = get_length();
      buffer = (char*)malloc(filesize); // the returned data
      fread(buffer, sizeof(char), filesize < maxSize ? maxSize : filesize, file_pointer);
      return filesize;
    }
  };

  // silly silly.
  template <typename T> class vec {
  public:
    std::vector<T> sec;
    std::vector<T> ite;
    vec() : sec() {
    }
  };
}; // end ns tfwio
//
#ifndef __snd_def__
#define __snd_def__
//
namespace on {
namespace snd {
#include <stdlib.h>
#include <string.h>

  // TODO: data-stream samantics
  // I want to see a data-stream semantic
  // we would like to read and write wave files in the future.
  // waves have zstrings and other interesting tags that we might not really
  // expect all packed into a waveform file format such as r-iff.

  #pragma region endian utility

  #pragma pack(1)

  typedef union {
    uint16	usvalue;
    uint16	ubvalue[2];
    uint16	GetValue() { return endo_ui16(ubvalue); }
  }
  // (byte swap utility)
  // unsigned short and unsigned byte array type union.
  // 16-Bit endian conversion/automation union.
  uushort;

  // 32-Bit automating endian conversion utility
  // note that if you want to return (char*) string[4]
  // it would be helpful to actually get a string[5] so that
  // you can set `string[4] = '\0'`.
  // Otherwise, copying 4 chars from the char pointer
  // to a char string[5] array appears wise.
  typedef union _uuint_t {

    uint32	uivalue;
    uint8		ubvalue[4];

    char	*to_cstr5(int len=5) {
      char *returned = new char[len]; // +'\0'
      strncpy(returned, (char*)ubvalue, 4);
      returned[4] = (char)0; // make sure
      return returned;
    }

    // performs Endian swap. see the #define macro: endo_ui32
    uint32		GetValue() { return endo_ui32(ubvalue); }

    //uint	GetValue(){ return (ubvalue[0]<<24|ubvalue[1]<<16|ubvalue[2]<<8|ubvalue[3]); }
    operator uint32 (){ return GetValue(); }
    operator int (){  return (int)GetValue(); }
    operator char*(){ return (char*)ubvalue; }
  }
  // see uuint and _uuint_t
  // 32-Bit endian conversion/automation union.
  // - with operator overloads convertting to int, uint and char* (char str[4])
  // char pointer returns `char string[4]`.
  // uint is another conversion that yields a native uint result.
  ChunkID,
  // see ChunkID and _uuint_t
  // 32-Bit endian conversion/automation union.
  // - with operator overloads convertting to int, uint and char* (char str[4])
  // char pointer returns `char string[4]`.
  // uint is another conversion that yields a native uint result.
  uuint;

  // this is a re-definition of the above _uuint_t, uuint_t and ChunkID
  typedef union {
    uint8		charID[4];
    uuint	uuiv;

    // returns char-code ID with null terminator ('\0') for a total of 5 chars.
    char	*get_charID() {
      char *returned = new char[5]; // +'\0'
      strncpy(returned, (char*)charID, 4);
      returned[4] = (char)0;
      return returned;
    }
  }
  // another redef of header identifiers ;)
  snd_cktag;

  // packing eight bit bytes?
  #pragma pack(8)

  // FIXME: why is this here?
  typedef struct {
    snd_cktag	ckHeadID;
    uint32 ckSize;
    char *get_strval() { return ckHeadID.get_charID(); }
  }
  // unsigned long integer / 4-char ID
  // for printint CHUNK names to string.
  snd_ckltag;

  #pragma pack()

  #pragma endregion - union typedefs
  };
}; //h_out
//
#endif // SND_DEF
//EOF//////////////////////////////////////////////////////////////////////////////////
#ifndef __iffdefs__
#define __iffdefs__
//
namespace on {
namespace snd {
    // really, not any of this belongs here.
    //
    // the only thing that I can think of that does belong here would be a
    // base class for reading riff headers or the riff_io class (which is
    // defined elsewhere) or writing a waveform file (one might think)
#pragma region
    // data is of a string type with the respective identity...
    // Where we are most likely going to have ascii or ansi char strings.
    enum wnfo {
      snd_w_ArchivalLocation = 0x4941524C, //	"IARL"
      snd_w_Artist = 0x49415254, //	"IART"
      snd_w_Commissioned = 0x49434D53, //	"ICMS"
      snd_w_Comment = 0x49434D54, //	"ICMT"
      snd_w_Copyright = 0x49434F50, //	"ICOP"
      snd_w_DateCreated = 0x49435244, //	"ICRD"
      snd_w_Cropped = 0x49435250, //	"ICRP"
      snd_w_Dimensions = 0x4944494D, //	"IDIM"
      snd_w_DotsPerInch = 0x49445049, //	"IDPI"
      snd_w_Engineer = 0x49454E47, //	"IENG"
      snd_w_Genre = 0x49474E52, //	"IGNR"
      snd_w_Keywords = 0x494B4559, //	"IKEY"
      snd_w_Lightness = 0x494C4754, //	"ILGT"
      snd_w_Medium = 0x494D4544, //	"IMED"
      snd_w_Title = 0x494E414D, //	"INAM"
      snd_w_NumColors = 0x49504C54, //	"IPLT"
      snd_w_Product = 0x49505244, //	"IPRD"
      snd_w_Subject = 0x4953424A, //	"ISBJ"
      snd_w_Software = 0x49534654, //	"ISFT"
      snd_w_Sharpness = 0x49534850, //	"ISHP"
      snd_w_Source = 0x49535243, //	"ISRC"
      snd_w_SourceForm = 0x49535246, //	"ISRF"
      snd_w_Technician = 0x49544348  //	"ITCH"
      // there are 23
    };
    //
    static const std::string itt_hd[23] = {
      "IARL", "IART", "ICMS", "ICMT", "ICOP", "ICRD", "ICRP", "IDIM", "IDPI", "IENG",
      "IGNR", "IKEY", "ILGT", "IMED", "INAM", "IPLT", "IPRD", "ISBJ", "ISFT", "ISHP",
      "ISRC", "ISRF", "ITCH"
    };
    //
#pragma endregion wave info tag name enumeration
  };
};
//
#endif
//EOF//////////////////////////////////////////////////////////////////////////////////


// some more R-IFF sound related definitions.
// read only
#ifndef __sndiff__
#define __sndiff__ snd_iff
// #ifndef SND_DEF
// #  pragma message("sndiff> snd_defs.h included")
// #endif
namespace on {
namespace snd {
  #pragma region
  #pragma pack(1)

  // // Simple Chunk Header Tag
  // typedef struct t_iff_zstring_ansi {
  //   snd_ckltag	header;
  //   // clean up your result with delete returnvalue;
  //   ubit *get_str_type() {
  //     uuint ttype = header.ckHeadID.uuiv;
  //     ubit *mo = new ubit[5];
  //     strncpy((char *)mo, (char *)header.ckHeadID.charID, 4);
  //     mo[4] = (char)0;
  //     return mo;
  //   }
  //   char * get_char_type() {
  //     uuint ttype = header.ckHeadID.uuiv;
  //     char *mo = new char[5];
  //     strncpy((char *)mo, (char *)header.ckHeadID.charID, 4);
  //     mo[4] = (char)0;
  //     return mo;
  //   }
  // } iff_zstring;

  // Simple Chunk Header Tag
  typedef struct t_simple_header {
    uuint	ttype;
    uint32	length;
    uuint	ttag;
    // Simple Chunk Header Tag
  } iff_simple_chunk;

  // iff: zstring type (use zstr)
  // I've not correctly named this yet...
  // Neither has it been implemented.
  // we're not using malloc at all, actually.
  typedef struct _iff_zstr_t {
    uint32 length;
    char *value;

    // input must be terminated with a 'zero\0'
    _iff_zstr_t() : value(NULL), length(0) { }
    _iff_zstr_t(char* input){ set_value(input); }

    // we don't expect this to be over-used; use it once.
    void set_value(const char *input)
    {
      int i = 0;
      while (0 != input[i]) i++;
      length = i;
      value = (char*)malloc(length + 1);
      memcpy(value, input, (int)length + 1);
    }

    ~_iff_zstr_t(){ free(value);  delete value; }

    void write(FILE *pfile) {
      fwrite(&length, 4, 1, pfile);
      fwrite(value, length, 1, pfile);
    }

    // _iff_zstr_t operator =(char* strIn) { _iff_zstr_t bob(strIn); return bob; }
  } zstr;

  typedef struct _iff_wzstr_t {
    uint32 tag;
    zstr value;
    int length() { return value.length; }
    void set_value(uint32 ntag, const char* input) {
      tag = ntag;
      value.set_value(input);
    }

    void write(FILE *pfile) {
      fwrite(&tag, 4, 1, pfile);
      value.write(pfile);
    }

  } wzstr;

  // iff version tag
  typedef struct t_ck_iver {
    uint16		Major;
    uint16		Minor;
  // iff info tag
  } iver;

  typedef struct t_ck_nfo {
    snd_ckltag	infoHead;
    uint32		Length;
    iver		ifil;
    zstr		inam;
    zstr		isng;

    // could we just throw a map in here?
    // what is INFOsub as we used in csharp?
    //
    // std::map<long, INFOsub> nfosub;
    //
    //	public Dictionary<long,INFOsub> nfosub;
    //	public INFO(int ckSize, BinaryReader bir, FileStream fis)
    //	{
    //		ifil = new iver(); inam = new ZSTR(); isng = new ZSTR();
    //
    //		long origin = fis.Seek(8,SeekOrigin.Current);
    //		infoHead = efx.STR.getStr(bir.ReadChars(4));
    //		Length = bir.ReadInt32();
    //		long pos = fis.Position;
    //		nfosub = new Dictionary<long,INFOsub>();
    //		while (fis.Position < pos+ckSize-4)
    //		{
    //			long hand = fis.Position;
    //			INFOsub inx = new INFOsub(bir,fis,this);
    //			fis.Seek(hand+inx.Length+8,SeekOrigin.Begin);
    //		}
    //  }

    // in the origional (csharp) version of this class, we maintained a `Dictionary<long,INFOsub>`.
    // here there is a method which has not yet been constructed.
    // First a dictionary is a class instance.
    // - Dictionary<long,INFOsub> nfosub
    // its method signature: `public INFO(int ckSize, BinaryReader bir, FileStream fis)`
    // - instantiate iver, inam and isng.
    // - origin is fis.seek(8,SeekOrigin.Current)
    // - infoHead = efx.STR.getStr(bir.ReadChars(4))
    // - Length = bir.ReadInt32
    // - long pos = fis.Position
    // - nfosub = new Dictionary<>
    // - while (fis.Position < pos+ckSize-4) {
    //     long hand = fis.Position;
    //     INFOsub inx = new INFOsub(bir,fis,this);
    //     fis.Seek(hand+inx.Length+8,SeekOrigin.Begin);
    //   }
    // }
    // - interestingly, it appears that nothing is added to our local dictionary from
    //   the above code snipping, so I'm not quite sure what is the proper course of action.
    //
  } iff_ck_nfo;

  // iff chunk
  // a default chunk type
  typedef struct t_iff_chunk {

    uint8		ckID[4];	//	usually RIFF unless it's a sub-tag
    uint32	ckLength;	//	the length of the file-eight?
    uint8		ckTag[4];	//	the tagname

    char	*get_tag() {
      char *returned = new char[5];
      strncpy(returned, (char*)ckTag, 4);
      returned[4] = (char)0;
      return returned;
    }
    char	*get_hid() {
      char *returned = new char[5];
      strncpy(returned, (char*)ckID, 4);
      returned[4] = (char)0;
      return returned;
    }
  } CHUNK, *lpCHUNK;
  //
  typedef struct t_fmt_chunk {
    char	chunkID[4];
    uint32	chunkSize;
    uint16	wFormatTag;
    uint16	wChannels;
    //	is this of the right size?
    uint32	dwSamplesPerSec;
    uint32	dwAvgBytesPerSec;
    uint16	wBlockAlign;
    uint16	wBitsPerSample;

    char	*get_ckID() {
      char *returned = new char[5];
      strncpy(returned, chunkID, 4);
      returned[4] = (char)0;
      return returned;
    }
    /*	Note: there may be additional fields here, depending upon wFormatTag. */
  } FormatChunk, *lpFMT;

  //
  typedef struct t_iff_taginfo {
    char	ckID[4];
    uint32	ckSize;
    char	*get_chunkID() {
      char *returned = new char[5];
      strncpy(returned, ckID, 4);
      returned[4] = (char)0;
      return returned;
    }
  } *lpTAGINFO;
  //
  typedef struct t_ck_data {
    //use the freaking automated type, will you?
    char  chunkID[4];
    uint32	chunkSize;
    //should be a union unique to several audio data types & channels
    //but thats for the reader/writer to worry about
    uint8		*waveformData;
    //delete 'return_value'
    char	*get_chunkID() {
      char *returned = new char[5];
      strncpy(returned, chunkID, 4);
      returned[4] = (char)0;
      return returned;
    }
  } DataChunk, *lpDATA;

  // 12 (on line 1030?)
  #define IFF_HEADERSIZE (size_t)0x0C

  // default packing mechanism?
  // prior was pack(1)
  #pragma pack()
  #pragma endregion type defs

  #pragma region

  // R-IFF/Wave Format reader
  class iff_riff_wave {
  protected:
    //
    FILE       *fp;
    //
    lpCHUNK     fmt_chunk;
  public:
    // We have not yet defined a wide-char file path.
    char       *file_path;
    //
    long int    file_length;
    //
    iff_riff_wave() {}
    // TODO: MAKE SURE THIS WORKS!!!
    ~iff_riff_wave() {
      if (file_path) delete[] file_path; // TODO: MAKE SURE THIS WORKS
      if (fp) delete fp; // TODO: MAKE SURE THIS WORKS
    }
    // Get File Size/Length
    // this only works if the file is in an 'open' state
    // with (at least) read access.
    // Returns file size or (bool) false.
    uint32 get_buffer_position() {
      if (file_path == 0) return false;
      fpos_t pp;
      fgetpos(fp, &pp);
      return (uint32)pp;
    }
    //
    uint32 get_file_size() {
      if (file_path == 0) return false;
      uint32 uipos, uiend;
      uipos = get_buffer_position();
      fseek(fp, 0, 2); // 2==SEEK_END
      uiend = get_buffer_position();
      fseek(fp, uipos, 0); // ordinal position==0==SEEK_SET
      return uiend;
    }
    //
    int get_file_sizeu() {
      if (file_path == 0) return false;
      int uipos, uiend;
      uipos = get_buffer_position();
      fseek(fp, 0, 2); // 2==SEEK_END
      uiend = get_buffer_position();
      fseek(fp, uipos, 0); // ordinal position==0==SEEK_SET
      return uiend;
    }
    // Peek Ahead
    // Gets the next tag (IDENTITY) and returns the buffer to
    // it's original position.
    // delete return_value
    char *peek_ahead() {
      int spoint = get_buffer_position();
      char *data = new char[5];
      fread(data, 4, 1, fp);
      data[4] = 0;
      fseek(fp, spoint, 0);
      return data;
    }
    //
    int get_int_position(long position, bool return_bpos = false) {
      int opos;
      if (return_bpos) opos = get_buffer_position();
      fseek(fp, position, 0);
      int data;
      fread(&data, 4, 1, fp);
      if (return_bpos) fseek(fp, opos, 0);
      return data;
    }
    //
    int get_next_chunk() {
      int cpos = get_buffer_position();
      if (cpos >= file_length) return false;
      //char *pchar = peek_ahead();
      int value = get_int_position(cpos + 4);
      fseek(fp, cpos, 0);
      return cpos + 8 + value;
    }
    //
    lpTAGINFO get_next_generic() {
      int cpos = get_buffer_position();
      if (cpos >= file_length) return 0;
      // or do we do size of pointer?
      // I'll go with the size of the structure.
      int gsize = sizeof(t_iff_taginfo);
      //
      lpTAGINFO gCK; // I'm a pointer.
      gCK = (lpTAGINFO) malloc(gsize);
      //
      fread(gCK, gsize, 1, fp);
      //
      if (use_console) printf("'%s'> '%d'\n", gCK->get_chunkID(), gCK->ckSize);
      //
      fseek(fp, cpos, 0);
      if (use_console) printf("==========================\n");
      return gCK;
    }
    // Load Header
    // sets the header data structure
    int load_header() {
      if (file_path == 0) return false;
      fopen_s(&fp, file_path, "rb" /*"a"*/);

      file_length = get_file_size();
      fseek(fp, 0, 0);
      fmt_chunk = (lpCHUNK)malloc(IFF_HEADERSIZE);

      int j = fread(fmt_chunk, IFF_HEADERSIZE, 1, fp);
      fseek(fp, 12, 0);
      if (use_console) printf("==========================\n");
      for (int i = 0; i<file_length; i++) {
        get_next_generic();
        i = get_next_chunk();
        if (i < file_length) fseek(fp, i, 0);
      }
      fclose(fp);
      if (use_console) printf(__T("IFF-header successfully loaded\n\n"));
      return j;
    }
    //
    int set_file(char *new_path) {
      if (new_path == 0) return false;
      file_path = new_path;
      return true;
    }
  };
#pragma endregion " reader class "

}; // end of namespace
}; // end of namespace

#endif // __sndiff__
//EOF//////////////////////////////////////////////////////////////////////////////////

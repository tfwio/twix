#ifndef __IColDetail_h__
#define __IColDetail_h__
// #pragma once

////////////////////////////////////////////////////////////////////////////
// __col_detail_t, ColumnDetail
////////////////////////////////////////////////////////////////////////////

// what is this class used for?

// Used for storing columnar information for graphical representations
// of large numbers, particularly up to 999,999.99999
typedef class __col_detail_t
{
protected:
  double num;
  int X1         // primary mouse-x calculation
    , X2         // a secondary or modified X
    , L          // LEFT most part of the control
    , W          // WIDTH of the control
    , Precision
    , Digits
    ; // what is this?
  bool mIsDown;
  IntPoint md, mddiff, mddir, mm, mm2, mo, mo2, modiff, modir;

  // forgot i put this here.  this is our offset somehow into 
  // where the mouse-x is lined up to what digit.
  // with the value set to 4, the control acts a bit like one of those NI FM plugins or another.
  // This number reflects difference in decimal precision.
  // since we have 5 digits followig the decimal, a number of 4 would shift to the next right digit.
  // See: reindex(int)
  const int dec_ct = 5; 
  double
    A,  // y-dir
    C1, // x-index based on mouse calculation.
    C2, // secondary x-index based on mouse calculation.
    C1O,// flattened x-index // the index (stripped down);
    C2O,// flattened x-index - yes, I wanted two.
    O1, // pow; see the last line of our update() method.
    O2; // pow
  bool has_commas; // by default is set to true

  inline static int ydir(IntPoint a, IntPoint b){ return IntPoint(b - a).compare().Y; }
  inline static int ydir(int a, int b){ return a == b ? 0 : b < a ? 1 : -1; }
  inline static double ydir(double a, double b){ return a == b ? 0 : b < a ? 1 : -1; }

  // translation of x-index from string with commas (eg: '000,247.01234')
  int reindex(int index)
  {
    if (!has_commas) return index;
    int i = index;
    if (index > 2) i--; // so on > 2
    if (has_commas && (index > 6)) i--; // so on > 6
    return i;
  }
  // translation of x-index from string with commas (eg: '000,247.01234')
  static int reindex_s(int index, bool pHasCommas=true)
  {
    if (!pHasCommas) return index;
    int i = index;
    if (index > 2) i--; // so on > 2
    if (pHasCommas && (index > 6)) i--; // so on > 6
    return i;
  }

public:
  inline IntPoint last_mouseMove() { return mm; }

  void PointInfo(IntPoint &last, IntPoint &current, IntPoint &diff)
  {
    last = IntPoint(md);
    current = mm;
    diff = IntPoint(md-mm);
  }

  bool IsDown() { return mIsDown; }

  int getL() { return L; }
  int getW() { return W; }

  void m_over(int x, int y) {
    mm2    . from (mm);     // back up prior MouseMove
    mm     . set  (x, y);   // Set a new MouseMove
    mo2    . from (mo);     // Back Up Prior MouseOver
    mo     . set  (x, y);   // Set New MouseOver
    modiff . from (mo2 - mo);
    modir  . from (modir.compare());
    update(mm, mm2);
  }
  void m_drag(int x, int y, IRECT *roll=NULL) {
    mm2.from(mm);
    mm.set(x, y);
    mddiff.from(mm2 - mm);
    mddir.from(mddir.compare());
    if (roll)
    {
      mm .roll_y(x, y, *roll); // iMouseMove.roll_y
      mm2.roll_y(x, y, *roll);// iMouseMove2.roll_y
    }
    update(mm, mm2);
  }
  void m_up(int x, int y) {
    mIsDown = false;
    mm2.from(mm);
    mm.set(x, y);
    mddiff.from(mm2 - mm);
    update(mm, mm2);
  }
  void m_down(int x, int y) {
    mIsDown = true;
    md.set(x, y);
    mm2.from(mm);
    mm.set(x, y);
    update(mm, mm2);
  }

  // x-index based on mouse calculation.
  // (I THINKS THIS ONE IS FOR MOUSE-UP AND MOUSE-DOWN)
  double index1() { return C1; }

  // the index (stripped down);
  // is limited; see update(…)
  // (I THINKS THIS ONE IS FOR MOUSE-UP AND MOUSE-DOWN)
  double index1d() { return C1O; }

  // x-index based on mouse calculation.
  // (I THINKS THIS ONE IS FOR TRACKING MOUSE-MOVE WHILST MOUSE-OVER)
  double index2() { return C2; }

  // the index (stripped down);
  // is limited; see update(…)
  double index2d() { return C2O; }

  double power() { return O1; }

  int polarity()
  {
    return (int)A;
  }

  // this does not get a value.
  // it gets the POWER, thus our Column-Detail.
  double value(){
    if (A == 0) return 0;
    return (A > 0) ? O1 : O1 * -1;
  }
  // // this does not get a value.
  // // it gets the POWER, thus our Column-Detail.
  // double value_l(double min = 0, double max = 999999.99999){
  //   double v = __l(O1, min, max);
  //   if (A == 0) return 0;
  //   return (A > 0) ? v : v * -1;
  // }


  // ONLY RETURNS ZERO!!!!!!!!
  inline static int count_commas(int len = 12, int dec = 5) // eg: '12,456.89012' len=12, dec=5
  {
    int r = len - dec;   // 12 - 5      = 7 < number of chars including decimal '.' > '12,456.' = 7
    int n =  r - 1;      //  7 - 1      = 6 < number of chars minus the decimal
    int m =  n % 3;      //  6 % 3      = 0 < 
    int f =  3 - m;      //  3 - 0      = 3
    int d = (n - m) / 3; // (6 - 0) / 3 = 2 < this is perhaps our first offset
    return 0;
  }

  // // translation of x-index from string with commas (eg: '123,567.90123')
  // inline static int rpad_digit_offset(int num, int dec, double num)
  // {
  //   std::string nvalue = std::to_string(num);
  //   nvalue.
  // }

  // result is output x-index
  // returns the magnitude based on the direction of mouse-y and the calculated mouse-x-offset
  // relative to the leftmost digit's location and depth and the total number of decimal digits.
  // 
  // i didn't quite explain index. [the resulting value]
  // 
  // PARAM: dec = 5 ; decimal denominator length (number of decimal digits)
  // PARAM: len = 12; '123456.89012' STRING LENGTH IN CHARS THE FLOAT VALUE.
  inline static int str_index(int index, int len = 12, int dec = 5, bool b_commas = true)
  {
    // total number of numerator digits to the left of decimal point + 1
    int length_1up = len - dec;
    
    if (!b_commas) return (index > length_1up) ? int(index - 1) : int(index);
    
    int nextindex = 3-(length_1up%3);
    int m = 3 - (length_1up % 3), i = index;

    // FIND THE FIRST COMMA ON THREES FROM THE RIGHT.
    // i is our left-index // , j = index; while (true) { if (j > length_1up) break; j += 3; } // subtract an index for each found comma
    if (index > 2) i--; // so on > 2
    if (b_commas && (index > length_1up)) i--; // so on > 6
    return i;
  }

  //// this appears to be the primary output
  //inline static int check(
  //  IntPoint &p, int pL, int pW,
  //  int *out_xoff = 0, int *out_off = 0,
  //  int len = 12, int dec = 5)
  //{
  //  int x_offset = (int)(double)(p.X - pL) / (pW + 1.); // input  x-offset
  //  int offset = (int)__l(reindex_s(x_offset), 0, len);   // output x-index
  //  if (out_xoff != NULL) *out_xoff = x_offset;
  //  if (out_off != NULL) *out_off = offset;
  //  return pow(10, dec - offset);
  //}

  // this appears to be the primary output
  inline static double check(
    int &p_x,
    int pL, int pW,
    bool pInvert,
    int *out_xoff = nullptr, // 
    int *out_off = nullptr, // class equiv = C10
    int pLimit = 12,      // class_function->update(param) equiv = limit = default(12)
    int pDecCt = 5        // decimal count ; class equiv = dec_ct
    )
  {
    int x_offset = int((double)(p_x - pL) / (pW + 1.)); // input  x-offset
    int offset = (int)__l(reindex_s(x_offset), 0, pLimit);   // output x-index
    if (out_xoff != nullptr) *out_xoff = x_offset;
    if (out_off != nullptr)  *out_off  = offset;
    if (pInvert) return pow(10, pDecCt - offset);
    else return pow(10, pDecCt - offset) * -1.;
  }

  // we're using min/max 0,11 here.  (should lim be 11 or 10?)
  // this method does not utilize or set A.
  void update(int x1, int x2, int limit = 12)
  {
    // 
    // -------------------------------------------
    X1 = x1;
    C1 = double((int)(double)(X1 - L) / (W + 1.));
    C1O = double((int)__l(reindex(int(C1)), 0, limit));
    O1 = pow(10, dec_ct - C1O);
    // 
    // -------------------------------------------
    X2 = x2;
    C2 = double((int)(double)(X2 - L) / (W + 1.));
    C2O = double((int)__l(reindex(int(C2)), 0, limit));
    O2 = pow(10, dec_ct - C2O);
  }
  
  // // we're using min/max 0,11 here.  (should lim be 11 or 10?)
  // void update(int x1, int x2, IntPoint a, IntPoint b, int limit = 11)
  // {
  //   update(x1, x2, limit);
  //   A = ydir();
  // }
  
  // we're using min/max 0,11 here.  (should lim be 11 or 10?)
  void update(IntPoint a, IntPoint b, int limit = 11)
  {
    update(a.X, b.X, limit);
    A = ydir(a,b);
  }
  void clear()
  {
    C1 = -1;
  }
  //int compare() { return A; }
  
  // /param l is the left position of the control;
  // /param w is the width of the control
  void reset(int l, int w) { L = l, W = w; }
  __col_detail_t(int l, int w)
    : L(l), W(w), has_commas(true), Precision(5), Digits(6)
  {
  }
} ColumnDetail;

#endif
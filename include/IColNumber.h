#ifndef __col_number__
#define __col_number__
// #pragma once

#include "ICControls.h"

// IT APPEARS THIS CLASS IS NOT USED!

////////////////////////////////////////////////////////////////////////////
// __col_detail_t, ColumnDetail
////////////////////////////////////////////////////////////////////////////

// move static functions outside the class?

// index the relationship between float number string and
// human readable decimal value digits with commas.
// TARGET: OUTPUT STRING LIKE '123,567.90123'
//
// the only non-static method here is count_commas
struct col_number
{
  int ct_num, ct_den;
  double mValue;

  // i'm confused.  Is this from the right?
  inline static int first_comma(int len, int d)
  {
    return (len - d - 1) < 4 ? -1 : (len - d - 1) % 4;
  }
  inline static bool has_comma(int len, int d) { return ct_commas(len, d) > 0; }

  inline static int is_comma(int index, int len, int d)
  {
    int first = first_comma(len, d);
    if (index < first) return false;
    if ((index + first) % 4 == 0) return true;
    return false;
  }
  inline static int is_period(int index, int len, int d)
  {
    return index == len - d;
  }

  // o_len=13, dec_ct=5 if string = '123,567.90123'
  inline static int ct_commas(int o_len, int dec_ct){
    // we might throw an exception if (o_len - dec_ct - 1)
    // = 4 which means: ',123.56789' [10-5=5-'.'=4=no]
    return (o_len - dec_ct - 1) / 4;
  }

  int count_commas() { return ct_commas (ct_num, ct_den - 1); }

};

#endif

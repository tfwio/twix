#ifndef __common_m__
#define __common_m__

#include <vector>
#ifndef _INC_MATH
#  include <math.h>
#endif

#ifndef __tfw_ioh__
#include "on.tfwio.h"
#endif

#define f1p(h,input) \
h1.i0 = *in1; \
h1.o1 = b0 * h1.i0 - a1 * h1.o1; \
*out1 = h1.o1

#define direct_form_i_order(h,input,result) \
  b0 * input +            \
  b1 * h.i0 + b2 * h.i1 - \
  a1 * h.o0 - a2 * h.o1;
#define direct_form_i_hist(h,input,result)   \
  h.i1 = h.i0;  h.i0 = input;      \
  h.o2 = h.o1;  h.o1 = h.o0;       \
  h.o0 = result;
#define direct_form_i(h,input,result) \
  result = \
  direct_form_i_order(h,input,result) \
  direct_form_i_hist(h,input,result)

// old as of 20160903_2109_CST
// implies use of i0, i1, o0 and o1
#define bq_butter(h,input,result) direct_form_i(h,input,result)

// USELESS //#define bq_butter_old(h,input,output,result) \
// USELESS //   result = b0 * input + b1 * h.i0 + b2 * h.i1 - a1 * h.o0 - a2 * h.o1; \
// USELESS //   h.i1 = h.i0; \
// USELESS //   h.i0 = input; \
// USELESS //   h.o2 = h.o1; \
// USELESS //   h.o1 = h.o0; \
// USELESS //   h.o0 = result; \
// USELESS //   output = h.o0
// USELESS //// implies use of i0, i1, o0 and o1
// USELESS //#define bq_nother(h,input,output,result) \
// USELESS //result = b0 * input \
// USELESS //- b1 * h.i0 \
// USELESS //+ b2 * h.i1 \
// USELESS //- a1 * h.o0 \
// USELESS //- a2 * h.o1; \
// USELESS //h.i1 = h.i0; \
// USELESS //h.i1 = *in1; \
// USELESS //h.o1 = h.o0; \
// USELESS //h.o0 = result; \
// USELESS //output = h.o0

#define GT0 0.000001
#define LT1 0.999999


#endif
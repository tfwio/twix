#include "bq.h"
#include <math.h>
// a sketch based on notes from RBJ and other notes on bi-quads.
// - no:
//     - optimization (and floats) for the record.
//     - double-buffering to push nyquist
// - not finished
//     - allpass, peaking and others could use some work.

#define bq_sh_hi b0 = A * (ampm * CWMS);         \
                 b1 = -2.0 * A * (ammm * COS);   \
                 b2 = A * (ampm * CWPS);         \
                 a0 = ammp * CWMS;     \
                 a1 = 2.0 * (ampp * COS);        \
                 a2 = ammp * CWMS;               \
                 Normalize();

#define bq_shelf_hi_earlevel \
    if (mAmp >= 0.0) { /* boost */                 \
      norm = 1.0 / (1.0 + sqrt(2.0) * KPKMK);         \
      a0 = (V + sqrt(2.0 * V) * KPKMK) * norm;        \
      a1 =  2.0 * (KTK - V) * norm;                   \
      a2 = (V - sqrt(2.0 * V) * KPKMK) * norm;        \
      b1 =  2.0 * (KTK - 1.0) * norm;                 \
      b2 = (1.0 - sqrt(2.0) * KPKMK) * norm;          \
    } else { /* cut */                               \
      norm = 1 / (V + sqrt(2 * V) * KPKMK);           \
      a0 = (1.0 + sqrt(2.0) * KPKMK) * norm;          \
      a1 =  2.0 * (KTK - 1.0) * norm;                 \
      a2 = (1.0 - sqrt(2.0) * KPKMK) * norm;          \
      b1 =  2.0 * (KTK - V.0) * norm;                 \
      b2 = (V - sqrt(2.0 * V.0) * KPKMK) * norm;      \
    }

#define bq_sh_lo b0 = A *       ((A + 1.0) - (A - 1.0) * cs + 2.0 * sqrt(A) * alpha);     \
                 b1 = 2.0 * A * ((A - 1.0) - (A + 1.0) * cs);                       \
                 b2 = A *       ((A + 1.0) - (A - 1.0) * cs - 2.0 * sqrt(A) * alpha);     \
                 a0 =            (A + 1.0) + (A - 1.0) * cs + 2.0 * sqrt(A) * alpha;           \
                 a1 =    -2.0 * ((A - 1.0) + (A + 1.0) * cs);                          \
                 a2 =            (A + 1.0) + (A - 1.0) * cs - 2.0 * sqrt(A) * alpha;

#define bq_shelf_hi b0 = A * ((A + 1.0) - (A - 1.0) * COS + MR_C); \
                    b1 =  2.0 * A * ((A - 1.0) - (A + 1.0) * COS); \
                    b2 = A * ((A + 1.0) - (A - 1.0) * COS - MR_C); \
                    a0 = ampm * COS + MR_C;       \
                    a1 = -2.0 * (ammm * COS);     \
                    a2 = ampm * COS - MR_C ;      \
                    Normalize();

#define bq_shelf_lo b0 = A * (ammp * COS + MR_C); \
                    b1 = 2.0 * A * (ampp * COS);  \
                    b2 = A * (ammp * COS - MR_C); \
                    a0 = ampm * COS + MR_C;       \
                    a1 = -2.0 * (ammm * COS);     \
                    a2 = ampm * COS - MR_C;       \
                    Normalize();

#define bq_notch_orig b0 = 1.0;                   \
                      b1 = -2.0 * COS;            \
                      b2 = -1.0;                  \
                      a0 = 1.0 + alpha;           \
                      a1 = -2.0 * COS;            \
                      a2 = 1.0 - alpha;           \
                      Normalize();

BiQuad::BiQuad() {}
BiQuad::~BiQuad() {}

void BiQuad::Normalize()
{
  b0 /= a0;
  b1 /= a0;
  b2 /= a0;
  a1 /= a0;
  a2 /= a0;
}

void BiQuad::Process(double **inputs, double **outputs, int nFrames)
{
  double l, temp = 0, demp = 0;

  process_iter_stereo(nFrames)
  {
    direct_form_i(hist1, *in1, l); // left
    temp = LevelPercent(*in1, dry), demp = LevelPercent(l, wet);
    *out1 = MixRaw(temp, demp) * mGain;

    direct_form_i(hist2, *in2, l); // right
    temp = LevelPercent(*in2, dry), demp = LevelPercent(l, wet);
    *out2 = MixRaw(temp, demp) * mGain;
  }
}

void BiQuad::Prepare()
{
  // ;)
  // below should be another separate variable
  double cc, rc, beta;

  beta = sqrt(A + A);
  sn = sin(w0);
  cs = cos(w0);
  omega = 2.0 * M_PI * freq / Fs;

  switch (type)
  {
  case EPassType::pNotch: case EPassType::pApf:
  case EPassType::pLowPass: case EPassType::pHighPass:
    alpha = SIN * sinh(M_LN2 / 2 * Q * w0 / SIN);
    break;
  case EPassType::pHighShelf: case EPassType::pLowShelf:
    A = get_A(40.0); // pow(10, mAmp / 40.0)
    alpha = SIN / 2.0 * sqrt((A + 1.0 / A)*(1.0 / S - 1.0) + 2.0);
    c = sqrt(A);
    break;
  case EPassType::pSHi: case EPassType::pSLo:
    A = get_A(40.0); // pow(10, mAmp / 40.0)
    alpha = SIN * sinh(M_LN2 / 2 * BW * w0 / SIN);
    break;
  case EPassType::pPeakingEQ:
    A = get_A(40.0); // pow(10, mAmp / 40.0)
    c = sqrt(A);
    alpha = SIN / 2.0 * sqrt((A + 1.0 / A)*(1.0 / S - 1.0) + 2.0);
    break;
  default:
    alpha = SIN / (2.0 * Q); break;
  }

  double K = Ktan;
  double norm;


  switch (type)
  {
  case EPassType::pLowPass:   c = 1 / tan(M_PI * f0 / Fs);
    rc = Q * c;
    b0 = 1.0 / (1.0 + RNTZD + CSQ);    b1 = 2.0 * b0;                b2 = b0;                        /* a0 = 0.0; */                 a1 = 2 * (1 - CSQ) * b0;        a2 = (1 - rc + CSQ) * b0; break;//Normalize();
  case EPassType::pHighPass:  c = tan(M_PI * freq / Fs);
    rc = Q * c;
    b0 = 1.0 / (1.0 + rc + NTZS);      b1 = -2.0 * b0;               b2 = b0;                        /* a0 = 0.0; */                 a1 = 2.0 * (NTZS - 1.0) * b0;   a2 = (1.0 - rc + NTZS) * b0; break; //Normalize(); // check prior revision.  this was added.
  case EPassType::pLpf:
    b0 = (1.0 - COS) / 2.0;
    b1 = 1.0 - COS;
    b2 = (1.0 - COS) / 2;
    a0 = 1.0 + alpha;
    a1 = -2.0 * COS;
    a2 = 1.0 - alpha;
    Normalize();
    break;
  case EPassType::pHpf:
    b0 =  (1.0 + COS) / 2.0;
    b1 = -(1.0 + COS);
    b2 =  (1.0 + COS) / 2;
    a0 =   1.0 + alpha;
    a1 =  -2.0 * COS;
    a2 =   1.0 - alpha;
    Normalize();
    break;

  case EPassType::pBpf:       b0 = SIN / 2.0;                    b1 = 0.0;                     b2 = -SIN / 2;                     a0 = 1.0 + alpha;            a1 = -2.0 * COS;                a2 = 1.0 - alpha;            Normalize(); break;
  case EPassType::pBpf2:      b0 = alpha;                        b1 = 0.0;                    b2 = -alpha;                       a0 = 1.0 + alpha;            a1 = -2.0 * COS;                a2 = 1.0 - alpha;            Normalize(); break;
  case EPassType::pApf:       b0 = 1.0 - alpha;                  b1 = -2.0 * COS;              b2 = 1.0 + alpha;                  a0 = 1.0 + alpha;            a1 = -2.0 * COS;                a2 = 1.0 - alpha;            Normalize(); break;
  case EPassType::pPeakingEQ: b0 = 1.0 + alpha * A;              b1 = -2.0 * COS;              b2 = 1.0 - alpha * A;              a0 = 1.0 + alpha / A;        a1 = -2.0 * COS;                a2 = 1.0 - alpha / A;        Normalize(); break;
  case EPassType::pSHi: bq_sh_hi break;

  case EPassType::pSLo:
    //ammp * CWMS;
    //ammp * CWPS;
    break;
    // b0 = A * (ammp * CWMS);
    // b1 = 2.0 * A * (ampp * COS);
    // b2 = A * (ammp * CWPS);
    // a0 = ampm * COS + w0 * SIN;
    // a1 = -2.0 * (ammm * COS);
    // a2 = ampm * CWMS;
    // Normalize();
    // break;
  case EPassType::pHighShelf: bq_shelf_hi break;
  case EPassType::pLowShelf: bq_shelf_lo break;
  case EPassType::pNotch:
    norm = 1.0 / (1.0 + K / Q + KTK);
    b0 = (1.0 + KTK) * norm;
    b1 = 2.0 * (KTK - 1) * norm;
    b2 = a0;
    a1 = a1;
    a2 = (1 - K / Q + KTK) * norm;
    break; //bq_notch
  }
  return;
}

// only call this when you want an actual reset.
// perhaps we should include a snapshot of parameters
// for wet, dry and the freqs
void BiQuad::Reset(double fs = 44100.)
{
  dry = 0.0;
  wet = 100.0;
  Fs = fs; // From VST Plugin (IPlug)
  Prepare();
  hist1 = ChannelHistory();
  hist2 = ChannelHistory();
  return;
}

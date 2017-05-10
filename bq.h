#ifndef __BF_HL__
#define __BF_HL__

#include "bq-common.h"

const static double ResonanceMin = 1.4142135623730950488016887242097;
const static double ResonanceMax = 0.999999;

#define w0 omega
#define Q  reso
#define S  reso
#define f0 freq
#define Fc freq
#define Ktan    tan(M_PI * Fc / Fs)
#define OMEG    M_2PI * f0 / Fs
#define ampm    (A + 1.0) + (A - 1.0)
#define ammp    (A + 1.0) - (A - 1.0)
#define ammm    (A - 1.0) + (A + 1.0)
#define ampp    (A - 1.0) - (A + 1.0)
#define MR_C    2.0 * sqrt(A) * alpha
#define BW reso
#define SIN     sin(M_2PI * f0 / Fs)
#define COS     cos(M_2PI * f0 / Fs)
#define CWPS    cs - w0 * sn
#define CWMS    cs + w0 * sn
#define CSQ     c * c
#define CSR     sqrt(c)
#define CSRA    sqrt(A)
#define CSQA2   sqrt(A + A)
#define QRC     Q * c
#define pi_m_f0_d_Fs M_PI * freq / Fs
#define tan_pff tan(pi_m_f0_d_Fs)
#define RNTZ    Q * tan_pff
#define NTZD    1.0 / tan_pff
#define RNTZD   Q * (1.0 / tan_pff)
#define NTZS    (tan_pff * tan_pff)
#define NTZDS   ((NTZD) * (NTZD))

typedef struct ChannelHistory {
  double i0, i1, o0, o1, o2;
  ChannelHistory() : i0(0.), i1(0.), o0(0.), o1(0.), o2(0.) {}
  void clear() { i0 = 0.0, i1 = 0.0, o0 = 0.0, o1 = 0.0; }
} ButterH;

enum EPassType
{
  pLowPass = 0, pHighPass,
  pLpf, pHpf,
  pBpf, pBpf2, pApf,
  pPeakingEQ,
  pSHi, pSLo,
  pHighShelf, pLowShelf,
  pNotch,
  pFilterCount
};

static const char* cPassType[pFilterCount] = {
  "LP#0", "HP#0",
  "LP#1", "HP#1",
  "BP#0", "BP#1", "AP",
  "P.EQ",
  "SH HI", "SH LO",
  "hs", "lS",
  "NOTCH"
};

class BiQuad
{
public:
  int type;
  double reso, freq, mGain, mAmp;
  #define Q  reso
  #define S  reso
  #define f0 freq
  #define Fc freq


  BiQuad();
  ~BiQuad();

  // pow(10, mAmp * magn)
  inline double get_A(double magn){ return pow(10, mAmp * magn); }
  inline double MixLin(double input1, double input2){ return (input1 * 0.5) + (input2 * .5); }
  inline double MixRaw(double input1, double input2){ return input1 + input2; }
  inline double MixPow(double input1, double input2){ return sqrt(pow(input1, 2) + pow(input2, 2)); }

  // where percent > 0 <= 100
  inline double LevelPercent(double input1, double percent){
    return input1 * (percent * 0.01);
  }

  void Process(double **inputs, double **outputs, int nFrames);

  void Prepare();
  void Reset(double fs);
  void Clear(){ hist1.clear(); hist2.clear(); }
  // Positive value is dry, negative value is wet.
  // both wet and dry are set from this one call.
  void SetMix(double v)
  {
    mix_in = v;
    wet = (v >= 0) ? v : v + 100;
    dry = 100 - wet;
  }
  double GetMix() { return wet; }
private:
  void Normalize();
  // -1 to 0 to 1
  double wet = 1., dry = -1., mix_in;
  double Fs, c, a0, a1, a2, a3, b0, b1, b2, a, cs, sn, rc;
  double A, alpha, beta, omega;
  #define w0 omega
  ButterH hist1, hist2;
  double newOutput[2], newInput[2];
  double inputHistory[2], outputHistory[3];
};

#define KTK    K * K
#define KPKMK  K + K * K
#define V20    pow(10, fabs(mAmp) / 20.0)
#define V40    pow(10, fabs(mAmp) / 40.0)
#define V V20

#define process_iter_stereo(nFrames) \
  double *in1 = inputs[0], *in2 = inputs[1], *out1 = outputs[0], *out2 = outputs[1]; \
  for (int s = 0; s < nFrames; ++s, ++in1, ++in2, ++out1, ++out2)

#endif

//
//  ADRS.h
//
//  Created by Nigel Redmon on 12/18/12.
//  EarLevel Engineering: earlevel.com
//  Copyright 2012 Nigel Redmon
//
//  For a complete explanation of the ADSR envelope generator and code,
//  read the series of articles by the author, starting here:
//  http://www.earlevel.com/main/2013/06/01/envelope-generators/
//
//  License:
//
//  This source code is provided as is, without warranty.
//  You may copy and distribute verbatim copies of this document.
//  You may modify and use this source code to create binary code for your own purposes, free or commercial.
//

#ifndef ADRS_h
#define ADRS_h

#include <stdio.h>
#include <string>

using namespace std;
//#define ASDR_DBL
#ifdef ASDR_DBL
typedef double Number;
#else
typedef float Number;
#endif

enum envState {
  env_idle = 0,
  env_attack,
  env_decay,
  env_sustain,
  env_release
};

class ADSR {
public:
  ADSR(void);
  ~ADSR(void);
  //Number process(void);
  Number process() {
    switch (state) {
    case env_idle:
      break;
    case env_attack:
      output = attackBase + output * attackCoef;
      if (output >= 1.0) {
        output = 1.0;
        state = env_decay;
      }
      break;
    case env_decay:
      output = decayBase + output * decayCoef;
      if (output <= sustainLevel) {
        output = sustainLevel;
        state = env_sustain;
      }
      break;
    case env_sustain:
      break;
    case env_release:
      output = releaseBase + output * releaseCoef;
      if (output <= 0.0) {
        output = 0.0;
        state = env_idle;
      }
      break;
    }
    return output;
  }
  //Number process(int sampleCount);
  Number process(int sampleCount)
  {
    Number retVal = 0;

    if (state != env_idle)
    {
      for (int i = 0; i<sampleCount; i++)
        retVal = process();
    }

    return retVal;
  }
  //Number getOutput(void);
  Number getOutput() { return output; }
  //int getState(void);
  int getState() { return state; }
  // same as inline; hack
  envState getStateT(void) { return envState(getState()); }
  // same as inline; hack
  void gateT(envState pState) { gate(pState); }

  //void gate(int on);
  void gate(int gate) {
    if (gate==1)
      state = env_attack;
    else if (state != env_idle)
      state = env_release;
  }
  void setAttackRate(Number rate);
  void setDecayRate(Number rate);
  void setReleaseRate(Number rate);
  void setSustainLevel(Number level);
  void setTargetRatioA(Number targetRatio);
  void setTargetRatioDR(Number targetRatio);
  //void reset(void);
  void reset() {
    state = env_idle;
    output = 0.0;
  }

protected:
  int state;
  Number output;
  Number attackRate;
  Number decayRate;
  Number releaseRate;
  Number attackCoef;
  Number decayCoef;
  Number releaseCoef;
  Number sustainLevel;
  Number targetRatioA;
  Number targetRatioDR;
  Number attackBase;
  Number decayBase;
  Number releaseBase;
  string name;
  Number calcCoef(Number rate, Number targetRatio);

};



#endif

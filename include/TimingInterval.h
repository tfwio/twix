#ifndef __TIMING_INTERVAL__
#define __TIMING_INTERVAL__

class TimingInterval
{
public:

  enum TIMINGPARAMS {
    _tTempo,
    _tIsRunning,
    _tQuarters,
    _tDivisions
  };

  // void Play(){} void Pause(){} void Stop(){} void Reset(){}

  int GetQuarters() { return int(quar); }
  int GetDivisions() { return int(divs); }
  double GetTempo() { return Tempo; }

  virtual void Set(TIMINGPARAMS paramid, double value)
  {
    switch (paramid)
    {
    case 0: Tempo = value; break;
    case 1: IsRunning = bool(value); break;
    case 2: quar = int8(value); Update(); break;
    case 3: divs = int8(value); Update(); break;
    case 4: segs = int8(value); Update(); break;
    }
  }


private:
  void Update(){
    calcDivs = (double(quar) / double(divs));
    calcSegs = calcDivs * segs;
    Frames = long(floor(calcSegs));
  }

  double Tempo, SampleRate;
  
  bool IsRunning; // Play is pressed.
  byte quar; // number of quarters per segment (numerator)
  byte divs; // divide number of quarters by this
  double calcDivs; // quar/div

  byte segs; // Segment length; total number of `quars/divs` segments per interval
  double calcSegs; // number of frames per segment; (QN / DIV) * Segments

  long  Frames;
};


#endif
#ifndef SYNTH_PITCH_H
#define SYNTH_PITCH_H

/*
  f=\operatorname{floor}\left(488\cdot2^{\frac{\operatorname{mod}\left(x,\ 12\right)}{12}}\right)
*/

class Pitch {
private:
  /*int lut_base_frq[13] = {
      65536, 69432,  73561,  77935,  82570,  87480,
      92681, 98193, 104031, 110217, 116771, 123715,
      131072
  };*/

  int lut_base_frq[13] = {
    488, 517, 547, 580, 614, 651,
    690, 731, 774, 820, 869, 921,
    976
  };

public:
  int base_frequency; // in q16.16! that is 16-bit whole, and 16-bit decimal
  int octave;
  void noteToPitch(int q16_pitch);
};

#endif
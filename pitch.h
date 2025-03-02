#ifndef SYNTH_PITCH_H
#define SYNTH_PITCH_H

class Pitch {
private:
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
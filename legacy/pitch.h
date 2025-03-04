#ifndef SYNTH_PITCH_H
#define SYNTH_PITCH_H



class Pitch {
private:
  uint16_t lut_base_frq[13] = {
    488, 517, 547, 580, 614, 651,
    690, 731, 774, 820, 869, 921,
    976
  };

public:
  uint16_t frequency;
  void noteToPitch(uint16_t semitone); // Q8.8 fixed point!
  void getOctave();
  void getFnumber();
};

#endif
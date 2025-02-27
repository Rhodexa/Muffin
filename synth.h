#ifndef SYNTH_H
#define SYNTH_H

#include "iobus.h"

/* These are my "abstracted" algorithms based on combinations of three different voices in different modes */
enum Algorithms{  
          // 2OP FM | 2OP ADD | 3OP FM | 4OP FM | 1OP ADD |
  ALG_1,  //      3 |         |        |        |         |
  ALG_2,  //      2 |       1 |        |        |         |
  ALG_3,  //      1 |       2 |        |        |         |
  ALG_4,  //        |       3 |        |        |         |
  ALG_5,  //      1 |         |        |      1 |         |
  ALG_6,  //        |       1 |        |      1 |         |
  ALG_7,  //      1 |         |      1 |        |       1 |
  ALG_8,  //        |       1 |      1 |        |       1 |
}

/*  This represents the properties of an OPL3 operator  */
struct Operator {
  uint8_t volume;
  uint8_t attack, decay, sustain, release;
  uint8_t multiplier;
  uint8_t waveform;
  bool has_vibrato;
  bool has_tremolo;
}



class Synth{
public:
  uint8_t algorithm;


}
#endif
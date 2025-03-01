#ifndef SYNTH_VOICE_H
#define SYNTH_VOICE_H
#include "synth_pitch.h"

namespace Synth {
namespace Voice {

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
};

struct Voice {
  int q16_base_frequency;
  int octave; 
  uint8_t algorithm;
  bool is_active;
};

}} // End namespace Synth::Voice

#endif
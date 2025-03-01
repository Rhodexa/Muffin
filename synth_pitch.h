#ifndef SYNTH_PITCH_H
#define SYNTH_PITCH_H

namespace Synth {

struct Pitch {
  int q16_base_frequency;
  int octave;
};

int q16_lut_base_frq6[13] = {
  65536, 69432,  73561,  77935,  82570,  87480,
  92681, 98193, 104031, 110217, 116771, 123715,
  131072
};

// Performs the math to convert indexed notes into base_frequency and octave parameters. 
// Indexed notes basically mean that a value of (Q16) '0' corresponds to A4, 1 is B4, 2 is C4 and so on,
// It also performs interpolation, such that 0.5 is in between A4 and B4, for example.
void pitch_fromLinear(Pitch& pitch, int q16_pitch){
  int int_part = (q16_pitch >> 16);
  int frc_part =  q16_pitch & 0xFFFF;
  int index_a = int_part % 12;
  int index_b = index_a + 1;
  int base_frq_a = q16_lut_base_frq6[index_a];
  int base_frq_b = q16_lut_base_frq6[index_b];

  int interpolated = base_frq_a + ((frc_part * (base_frq_b - base_frq_a)) >> 16);

  pitch.octave = (int_part * 349525) >> 21; // int_part/12 magic
  pitch.q16_base_frequency = interpolated << (pitch.octave - 5);
}

}
#endif
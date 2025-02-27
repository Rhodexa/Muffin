#ifndef PITCH_SHAPER_H
#define PITCH_SHAPER_H

int q16_lut_base_frq6[13] = {
  65536, 69432,  73561,  77935,  82570,  87480,
  92681, 98193, 104031, 110217, 116771, 123715,
  131072
};

/*
  Loads the results into frenquecy and octave variables
*/

struct pitchProperties {
  int q16_base_frequency;
  int octave;
};

int frequency = 0;
int octave = 0;
void computeFrequencyAndOctave(int q16_pitch){
  int int_part = (q16_pitch >> 16);
  int frc_part =  q16_pitch & 0xFFFF;
  int index_a = int_part % 12;
  int index_b = index_a + 1;
  int base_frq_a = q16_lut_base_frq6[index_a];
  int base_frq_b = q16_lut_base_frq6[index_b];

  int interpolated = base_frq_a + ((frc_part * (base_frq_b - base_frq_a)) >> 16);

  octave = (int_part * 349525) >> 21; // int_part/12 magic
  frequency = interpolated << (octave - 5);
}

#endif
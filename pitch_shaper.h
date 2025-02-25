#ifndef PITCH_SHAPER_H
#define PITCH_SHAPER_H

int lut_base_frq_q16[13] = {
  65536, 69432,  73561,  77935,  82570,  87480,
  92681, 98193, 104031, 110217, 116771, 123715,
  131072
};

/*
  Loads the results into frenquecy and octave variables
*/
int frequency = 0;
int octave = 0;
void computeFrequencyAndOctave(int pitch_q16){
  int int_part = (pitch_q16 >> 16);
  int frc_part =  pitch_q16 & 0xFFFF;
  int index_a = int_part % 12;
  int index_b = index_a + 1;
  int base_frq_a = lut_base_frq_q16[index_a];
  int base_frq_b = lut_base_frq_q16[index_b];

  int interpolated = base_frq_a + ((frc_part * (base_frq_b - base_frq_a)) >> 16);

  octave = (int_part * 349525) >> 21; // int_part/12 magic
  frequency = interpolated << (octave - 5);
}

#endif
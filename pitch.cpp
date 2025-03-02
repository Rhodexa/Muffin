#include "pitch.h"

/*
  Creates a Pitch object from a semitone value. Minimum number is 18
*/

void Pitch::noteToPitch(unsigned int semitone){
  if(semitone >= 18) {
    semitone -= 18;
    unsigned int int_part = (semitone >> 16);
    unsigned int frc_part =  q16_pitch & 0xFFFF;
    pitch.octave = (int_part * 349525) >> 21;          // magical 'int_part/12' approximation
    unsigned int index = int_part - pitch.octave * 12; // quick mod(int_part, 12)
    unsigned int base_frq_a = lut_base_frq[index];
    unsigned int base_frq_b = lut_base_frq[index + 1];
    unsigned int interpolated = base_frq_a + ((frc_part * (base_frq_b - base_frq_a)) >> 16);  
    pitch.base_frequency = interpolated;
  }
}
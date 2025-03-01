#include "pitch.h"

/*
  A fast MIDI-note to Pitch object converter.
  Input: A MIDI note index (including fractional part)
  Output: A Pitch Object containing the scaled base_frequency information for OPL3 and an octave exponent.
  The midi note object begins at F#0 (MIDI pitch 18)
  With the current implementation, notes below MIDI-18 will wrap around to a horrible screech :)  
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
  /*
  else {
    placeholder for some kind of interpolation or different LUT for the lower section of the keyboard.
    It is possible to reach this range, but resolution is pretty bad and we need to hardcode octave to '0'
    In any case...getting that low is pretty much unecessary. (This is sub-20Hz territory already. We can't hear that!)

    Potential implementation:
      pitch.octave = 0; // set octave to 0 and compute as usual
      semitone -= 4;    // subtract 18-12 to compensate for the lower octave
      unsigned int int_part = (semitone >> 16);
      unsigned int frc_part =  q16_pitch & 0xFFFF;
      unsigned int index = int_part;
      unsigned int base_frq_a = lut_base_frq[index];
      unsigned int base_frq_b = lut_base_frq[index + 1];
      unsigned int interpolated = base_frq_a + ((frc_part * (base_frq_b - base_frq_a)) >> 16);  
      pitch.base_frequency = interpolated >> 1; // simulate octave-1 by right-shifting the fnumber
  }
  */
}
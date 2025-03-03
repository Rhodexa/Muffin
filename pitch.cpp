#include "pitch.h"

/*
  Creates a Pitch object from a semitone value. Minimum number is 18
  ((uint32_t)int_part * 349525) >> 22; // super accurate 1/12 approximation
*/

void Pitch::noteToPitch(uint16_t semitone){
  if(semitone >= 18) {
    semitone -= 18;
    uint8_t int_part = (semitone >> 8);
    uint8_t frc_part =  semitone & 0xFF;
    uint16_t octave = ((uint32_t)int_part * 1366) >> 14; // magical 'int_part/12' approximation (accurate up to 2050)
    uint8_t index = int_part - octave * 12; // quick mod(int_part, 12)
    uint16_t base_frq_a = lut_base_frq[index];
    uint16_t base_frq_b = lut_base_frq[index + 1];
    frequency = base_frq_a + ((frc_part * (base_frq_b - base_frq_a)) >> 8);   
    frequency |= octave << 8;
  }
}
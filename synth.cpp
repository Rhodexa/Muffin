#include "synth.h"
namespace Synth {

void setAlgorithm(uint8_t voice, uint8_t algorithm){
  if(voice < 4) REG_104 &= ~(1 << voice);
  else REG_104 |= (1 << voice);
  voices[voice].setAlgorithm(algorithm);    
}

uint16_t pitchToFrequency(uint32_t q16_pitch){
  if(q16_pitch >= 18 && q16_pitch < 114) {
    q16_pitch -= 18;
    uint8_t int_part = (q16_pitch >> 8);
    uint8_t frc_part =  q16_pitch & 0xFF;
    uint16_t octave = ((uint32_t)int_part * 1366) >> 14;
    uint8_t index = int_part - octave * 12;
    uint16_t frq_a = lut_base_frq[index];
    uint16_t frq_b = lut_base_frq[index + 1];
    return ( frq_a + ((frc_part * (frq_b - frq_a)) >> 16) ) << octave;
  }
  return 0;
}

uint16_t encodeFrequency(uint32_t frequency){
  uint8_t leading_zeroes = 7 - (__builtin_clz(frequency) - (32 - 17));
  uint8_t exponent = (leading_zeroes > 7) ? 0 : (7 - leading_zeroes);
  uint16_t mantissa = value >> exponent;
  return (exponent << 10) | mantissa;
}

}
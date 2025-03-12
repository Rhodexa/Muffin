#include "synth.h"

namespace Synth {

void begin(){
  oplrw_write(0x104, 0x01);
}

void setAlgorithm(uint8_t voice, uint8_t algorithm){
  if (voice < 4) REG_104 &= ~(1 << voice);
  else REG_104 |= (1 << voice);
  voices[voice].setAlgorithm(algorithm);    
}

uint16_t pitchToFrequency(uint32_t q16_pitch){
  if (q16_pitch >= 18 && q16_pitch < 114) {
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

void handleNoteOn(uint8_t pitch){
  // Find the next available voice
  for (uint8_t i = 0; i < 6; i++){
    m_current_voice++;
    if (m_current_voice >= 6) m_current_voice = 0;
    if (voices[m_current_voice].is_active != true) break;
  }
  held_key[m_current_voice] = pitch;
  voices[m_current_voice].setFrequency(pitchToFrequency((pitch + 18) << 16));
  voices[m_current_voice].setNoteOn(true);
}

void updateOPL(){
  for (uint8_t i = 0; i < 6; i++){
    voices[i].loadToOPL();
  }
}

}
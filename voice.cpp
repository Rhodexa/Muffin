#include "voice.h"

// This function is a joy! It handles the frequency conversion from a Q16.16 frequency value in abstract OPL3 base units and 
// converts it to the 10-bit frequency value that the OPL3 chip expects, including the octave exponent.
// It also handles the frenquency scaling between channels, since in our synth a voice is composed of three 2OP channels played together
// but not necessarily at the same frequency. This allows for some funky phasing tricks.

uint16_t Voice::encodeFrequency(uint32_t frequency){
  uint8_t leading_zeroes = 7 - (__builtin_clz(frequency) - (32 - 17));
  uint8_t exponent = (leading_zeroes > 7) ? 0 : (7 - leading_zeroes);
  uint16_t mantissa = value >> exponent;
  return (exponent << 10) | mantissa;
}

void Voice::setFrequency(uint32_t frequency){
  uint16_t frequencyEncoded = encodeFrequency((frequency * instrument.multiplier[0]) >> 16);
  m_channel_reg_A0[0] = frequencyEncoded & 0xFF;
  m_channel_reg_B0[0] &= 0xE0;
  m_channel_reg_B0[0] |= (frequencyEncoded >> 8) & 0x1F;
  frequencyEncoded = encodeFrequency((frequency * instrument.multiplier[1]) >> 16);
  m_channel_reg_A0[1] = frequencyEncoded & 0xFF;
  m_channel_reg_B0[1] &= 0xE0;
  m_channel_reg_B0[1] |= (frequencyEncoded >> 8) & 0x1F;
  frequencyEncoded = encodeFrequency((frequency * instrument.multiplier[2]) >> 16);
  m_channel_reg_A0[2] = frequencyEncoded & 0xFF;
  m_channel_reg_B0[2] &= 0xE0;
  m_channel_reg_B0[2] |= (frequencyEncoded >> 8) & 0x1F;
}

void Voice::setnoteOn(uint8_t is_on){
  if (is_on){
    m_channel_reg_B0[0] |= 0x20;
    m_channel_reg_B0[1] |= 0x20;
    m_channel_reg_B0[2] |= 0x20;
  } else {
    m_channel_reg_B0[0] &= 0xDF;
    m_channel_reg_B0[1] &= 0xDF;
    m_channel_reg_B0[2] &= 0xDF;
  }
}

void Voice::setInstrument(Instrument& instr){
  instrument = instr;
}
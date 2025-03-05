#include "voice.h"

// This function is a joy! It handles the frequency conversion from a Q16.16 frequency value in abstract OPL3 base units and 
// converts it to the 10-bit frequency value that the OPL3 chip expects, including the octave exponent.
// It also handles the frenquency scaling between channels, since in our synth a voice is composed of three 2OP channels played together
// but not necessarily at the same frequency. This allows for some funky phasing tricks.

Voice::Voice(uint8_t selfIndex, Instrument& instrument) : m_inst(&instrument)
{
  m_selfIndex = selfIndex;
  for (uint8_t i = 0; i < 3; i++){
    m_channel_reg_A0[i] = 0;
    m_channel_reg_B0[i] = 0;
  }
}

void Voice::setInstrument(Instrument& instrument)
{
  m_inst = instrument;
}

uint16_t Voice::encodeFrequency(uint32_t frequency)
{
  uint8_t leading_zeroes = 7 - (__builtin_clz(frequency) - (32 - 17));
  uint8_t exponent = (leading_zeroes > 7) ? 0 : (7 - leading_zeroes);
  uint16_t mantissa = value >> exponent;
  return (exponent << 10) | mantissa;
}

void Voice::loadToOPL()
{
  for (uint8_t c = 0; c < 3; c++){
    for (uint8_t o = 0; o < 2; o++){
      uint16_t offset = operator_offset[m_selfIndex][c][o];
      OPLRW::write(0x20 + offset, m_inst->operator_reg_20[o]);
      OPLRW::write(0x40 + offset, m_inst->operator_reg_40[o]);
      OPLRW::write(0x60 + offset, m_inst->operator_reg_60[o]);
      OPLRW::write(0x80 + offset, m_inst->operator_reg_80[o]);
      OPLRW::write(0xE0 + offset, m_inst->operator_reg_E0[o]);
    }
    uint16_t offset = channel_offset[m_selfIndex][c];
    OPLRW::write(0xC0 + offset, m_inst->channel_reg_C0[c]);

    // Frequency and Note On Registers
    OPLRW::write(0xA0 + channel_offset[m_selfIndex][i], m_inst->m_channel_reg_A0[i]);
    OPLRW::write(0xB0 + channel_offset[m_selfIndex][i], m_inst->m_channel_reg_B0[i]);
  }
  
}

void Voice::setFrequency(uint32_t frequency)
{
  uint16_t
  frequencyEncoded = encodeFrequency((frequency * m_inst->multiplier[0]) >> 16);
  m_inst->m_channel_reg_A0[0] = frequencyEncoded & 0xFF;
  m_inst->m_channel_reg_B0[0] &= 0xE0;
  m_inst->m_channel_reg_B0[0] |= (frequencyEncoded >> 8) & 0x1F;

  frequencyEncoded = encodeFrequency((frequency * m_inst->multiplier[1]) >> 16);
  m_inst->m_channel_reg_A0[1] = frequencyEncoded & 0xFF;
  m_inst->m_channel_reg_B0[1] &= 0xE0;
  m_inst->m_channel_reg_B0[1] |= (frequencyEncoded >> 8) & 0x1F;

  frequencyEncoded = encodeFrequency((frequency * m_inst->multiplier[2]) >> 16);
  m_inst->m_channel_reg_A0[2] = frequencyEncoded & 0xFF;
  m_inst->m_channel_reg_B0[2] &= 0xE0;
  m_inst->m_channel_reg_B0[2] |= (frequencyEncoded >> 8) & 0x1F;
}

void Voice::setNoteOn(bool is_on)
{
  if (is_on) {
    is_active = true;
    m_inst->m_channel_reg_B0[0] |= 0x20;
    m_inst->m_channel_reg_B0[1] |= 0x20;
    m_inst->m_channel_reg_B0[2] |= 0x20;
  }
  else {
    m_inst->m_channel_reg_B0[0] &= 0xDF;
    m_inst->m_channel_reg_B0[1] &= 0xDF;
    m_inst->m_channel_reg_B0[2] &= 0xDF;
  }
}


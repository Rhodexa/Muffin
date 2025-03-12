#include "voice.h"

Voice::s_connection_select = 0;

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
  m_inst = &instrument;
}

uint16_t Voice::encodeFrequency(uint32_t frequency)
{
  uint8_t leading_zeroes = 7 - (__builtin_clz(frequency) - (32 - 17));
  uint8_t exponent = (leading_zeroes > 7) ? 0 : (7 - leading_zeroes);
  uint16_t mantissa = frequency >> exponent;
  return (exponent << 10) | mantissa;
}

void Voice::loadToOPL()
{
  // Iterate through all 3 channels in the voice
  for (uint8_t ch = 0; ch < 3; ch++){
    // Load the channel's operators data
    for (uint8_t op = 0; op < 2; op++){
      uint16_t offset = operator_offset[m_selfIndex][ch][op];
      OPL::write(0x20 + offset, m_inst->operator_reg_20[op]);
      OPL::write(0x40 + offset, m_inst->operator_reg_40[op]);
      OPL::write(0x60 + offset, m_inst->operator_reg_60[op]);
      OPL::write(0x80 + offset, m_inst->operator_reg_80[op]);
      OPL::write(0xE0 + offset, m_inst->operator_reg_E0[op]);
    }
    uint16_t offset = channel_offset[m_selfIndex][ch];
    OPL::write(0xC0 + offset, m_inst->channel_reg_C0[ch]); // Stereo and CNT1

    // build register 0x104 (CONNECTION_SEL). (because its shared! ugh! lol)
    if(m_inst->connection_select) s_connection_select &= ~(1 << m_selfIndex);
    else s_connection_select |= (1 << m_selfIndex);
    OPL::write(0x104, s_connection_select);

    // Frequency and Note On Registers
    OPL::write(0xA0 + channel_offset[m_selfIndex][ch], m_channel_reg_A0[ch]);
    OPL::write(0xB0 + channel_offset[m_selfIndex][ch], m_channel_reg_B0[ch]);
  }  
}

void Voice::setFrequency(uint32_t frequency)
{
  uint16_t
  frequencyEncoded = encodeFrequency((frequency * m_inst->multiplier[0]) >> 16);
  m_channel_reg_A0[0] = frequencyEncoded & 0xFF;
  m_channel_reg_B0[0] &= 0xE0;
  m_channel_reg_B0[0] |= (frequencyEncoded >> 8) & 0x1F;

  frequencyEncoded = encodeFrequency((frequency * m_inst->multiplier[1]) >> 16);
  m_channel_reg_A0[1] = frequencyEncoded & 0xFF;
  m_channel_reg_B0[1] &= 0xE0;
  m_channel_reg_B0[1] |= (frequencyEncoded >> 8) & 0x1F;

  frequencyEncoded = encodeFrequency((frequency * m_inst->multiplier[2]) >> 16);
  m_channel_reg_A0[2] = frequencyEncoded & 0xFF;
  m_channel_reg_B0[2] &= 0xE0;
  m_channel_reg_B0[2] |= (frequencyEncoded >> 8) & 0x1F;
}

void Voice::setNoteOn(bool is_on)
{
  if (is_on) {
    if(!m_is_active){
      m_is_active = true;
      m_channel_reg_B0[0] |= 0x20;
      m_channel_reg_B0[1] |= 0x20;
      m_channel_reg_B0[2] |= 0x20;
    }
  }
  else {
    if(m_is_active){
      m_channel_reg_B0[0] &= ~(0x20);
      m_channel_reg_B0[1] &= ~(0x20);
      m_channel_reg_B0[2] &= ~(0x20);
    }
  }
}


#include "voice.h"
#include <Arduino.h>

uint8_t Voice::s_connection_select = 0;

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
  uint8_t leading_zeroes = __builtin_clz(frequency) - 15;
  uint8_t exponent = (leading_zeroes > 7) ? 0 : (7 - leading_zeroes);
  uint16_t mantissa = frequency >> exponent;
  return (exponent << 10) | mantissa;
}

void Voice::loadToOPL()
{
  //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  // Iterate through all 3 channels in the voice
  for (uint8_t ch = 0; ch < 3; ch++){
    // Load the channel's operators data
    uint16_t offset = operator_offset[m_selfIndex][ch][0];
    OPL::write(0x20 + offset, m_inst->operator_reg_20[ch][0]);
    OPL::write(0x40 + offset, m_inst->operator_reg_40[ch][0]);
    OPL::write(0x60 + offset, m_inst->operator_reg_60[ch][0]);
    OPL::write(0x80 + offset, m_inst->operator_reg_80[ch][0]);
    OPL::write(0xE0 + offset, m_inst->operator_reg_E0[ch][0]);

    offset = operator_offset[m_selfIndex][ch][1];
    OPL::write(0x20 + offset, m_inst->operator_reg_20[ch][1]);
    OPL::write(0x40 + offset, m_inst->operator_reg_40[ch][1]);
    OPL::write(0x60 + offset, m_inst->operator_reg_60[ch][1]);
    OPL::write(0x80 + offset, m_inst->operator_reg_80[ch][1]);
    OPL::write(0xE0 + offset, m_inst->operator_reg_E0[ch][1]);

    // load channel data
    offset = channel_offset[m_selfIndex][ch];
    OPL::write(0xC0 + offset, m_inst->channel_reg_C0[ch]); // Stereo and CNT1
  }

  // build register 0x104 (CONNECTION_SEL). (because its shared! ugh! lol)
  if(m_inst->connection_select) s_connection_select |= (1 << m_selfIndex);
  else s_connection_select &= ~(1 << m_selfIndex);
  OPL::write(0x104, s_connection_select);

  for(uint8_t ch = 0; ch < 3; ch++){
    // Frequency and Note On Registers
    OPL::write(0xA0 + channel_offset[m_selfIndex][ch], m_channel_reg_A0[ch]);
    OPL::write(0xB0 + channel_offset[m_selfIndex][ch], m_channel_reg_B0[ch]);
  }
}

void Voice::setFrequency(uint32_t frequency)
{
  uint16_t
  frequencyEncoded = encodeFrequency((frequency * m_inst->multiplier[0]) >> 12);
  m_channel_reg_A0[0] = frequencyEncoded & 0xFF;
  m_channel_reg_B0[0] &= 0xE0;
  m_channel_reg_B0[0] |= (frequencyEncoded >> 8) & 0x1F;

  frequencyEncoded = encodeFrequency((frequency * m_inst->multiplier[1]) >> 12);
  m_channel_reg_A0[1] = frequencyEncoded & 0xFF;
  m_channel_reg_B0[1] &= 0xE0;
  m_channel_reg_B0[1] |= (frequencyEncoded >> 8) & 0x1F;

  frequencyEncoded = encodeFrequency((frequency * m_inst->multiplier[2]) >> 12);
  m_channel_reg_A0[2] = frequencyEncoded & 0xFF;
  m_channel_reg_B0[2] &= 0xE0;
  m_channel_reg_B0[2] |= (frequencyEncoded >> 8) & 0x1F;
}

void Voice::setNoteOn(bool is_on)
{
  if (is_on) {
    if(!is_active){
      is_active = true;
      m_channel_reg_B0[0] |= 0x20;
      m_channel_reg_B0[1] |= 0x20;
      m_channel_reg_B0[2] |= 0x20;
    }
  }
  else {
    if(is_active){
      is_active = false;
      m_channel_reg_B0[0] &= ~(0x20);
      m_channel_reg_B0[1] &= ~(0x20);
      m_channel_reg_B0[2] &= ~(0x20);
    }
  }
}


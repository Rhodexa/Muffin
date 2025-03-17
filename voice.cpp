#include "voice.h"
#include <Arduino.h>


// Utility methods

void Voice::set(uint8_t flag){ m_flags |= flag;}
void Voice::clear(uint8_t flag){ m_flags &= ~(flag);}
bool Voice::check(uint8_t flag){ return m_flags & flag;}

// static
uint16_t Voice::encodeFrequency(uint32_t frequency)
{
  uint8_t leading_zeroes = __builtin_clz(frequency) - 15;
  uint8_t exponent = (leading_zeroes > 7) ? 0 : (7 - leading_zeroes);
  uint16_t mantissa = frequency >> exponent;
  return (exponent << 10) | mantissa;
}

uint32_t Voice::pitchToFrequency(uint32_t q16_pitch){
  if (q16_pitch >= (18 << 16) && q16_pitch < (114 << 16)) {
    q16_pitch -= (18 << 16);
    uint8_t int_part = (q16_pitch >> 16);
    uint16_t frc_part =  q16_pitch & 0xFFFF;
    uint16_t octave = ((uint32_t)int_part * 1366) >> 14;
    uint8_t index = int_part - octave * 12;
    uint16_t frq_a = lut_base_frq[index];
    uint16_t frq_b = lut_base_frq[index + 1];
    return ( frq_a + ((frc_part * (frq_b - frq_a)) >> 16) ) << (octave+1);
  }
  return 0;
}


// Voice

uint8_t Voice::s_connection_select = 0;

Voice::Voice() : instrument(nullptr) {
  for (uint8_t i = 0; i < 3; i++) {
    m_channel_reg_A0[i] = 0;
    m_channel_reg_B0[i] = 0;
  }
}

void Voice::setInstrument(Instrument& instrument) { this->instrument = &instrument; }

void Voice::loadToOPL()
{
  //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  // Iterate through all 3 channels in the voice
  for (uint8_t ch = 0; ch < 3; ch++){
    // Load the channel's operators data
    uint16_t offset = operator_offset[m_selfIndex][ch][0];
    OPL::write(0x20 + offset, instrument->operator_reg_20[ch][0]);
    OPL::write(0x40 + offset, instrument->operator_reg_40[ch][0]);
    OPL::write(0x60 + offset, instrument->operator_reg_60[ch][0]);
    OPL::write(0x80 + offset, instrument->operator_reg_80[ch][0]);
    OPL::write(0xE0 + offset, instrument->operator_reg_E0[ch][0]);

    offset = operator_offset[m_selfIndex][ch][1];
    OPL::write(0x20 + offset, instrument->operator_reg_20[ch][1]);
    OPL::write(0x40 + offset, instrument->operator_reg_40[ch][1]);
    OPL::write(0x60 + offset, instrument->operator_reg_60[ch][1]);
    OPL::write(0x80 + offset, instrument->operator_reg_80[ch][1]);
    OPL::write(0xE0 + offset, instrument->operator_reg_E0[ch][1]);

    // load channel data
    offset = channel_offset[m_selfIndex][ch];
    OPL::write(0xC0 + offset, instrument->channel_reg_C0[ch]); // Stereo and CNT1
  }

  // build register 0x104 (CONNECTION_SEL). (because its shared! ugh! lol)
  // algorithms 4, 5, 6 and 7 has 4OP mode enabled
  if(instrument->algorithm < 4) s_connection_select &= ~(1 << m_selfIndex);
  else s_connection_select |= (1 << m_selfIndex);
  OPL::write(0x104, s_connection_select);

  OPL::write(0xA0 + channel_offset[m_selfIndex][0], m_channel_reg_A0[0]);
  OPL::write(0xA0 + channel_offset[m_selfIndex][2], m_channel_reg_A0[2]);
  OPL::write(0xA0 + channel_offset[m_selfIndex][1], m_channel_reg_A0[1]);

  if(check(FLAG_RETRIGGER)){ // Send key off for a split second to retrigger them
    clear(FLAG_RETRIGGER);
    OPL::write(0xB0 + channel_offset[m_selfIndex][0], m_channel_reg_B0[0] & 0b11011111);
    OPL::write(0xB0 + channel_offset[m_selfIndex][1], m_channel_reg_B0[1] & 0b11011111);
    OPL::write(0xB0 + channel_offset[m_selfIndex][2], m_channel_reg_B0[2] & 0b11011111);
  }
  OPL::write(0xB0 + channel_offset[m_selfIndex][0], m_channel_reg_B0[0]);
  OPL::write(0xB0 + channel_offset[m_selfIndex][1], m_channel_reg_B0[1]);
  OPL::write(0xB0 + channel_offset[m_selfIndex][2], m_channel_reg_B0[2]);
}

void Voice::setFrequency(uint32_t frequency)
{
  uint16_t
  frequencyEncoded = encodeFrequency((frequency * instrument->multiplier[0]) >> 12);
  m_channel_reg_A0[0] = frequencyEncoded & 0xFF;
  m_channel_reg_B0[0] &= 0xE0;
  m_channel_reg_B0[0] |= (frequencyEncoded >> 8) & 0x1F;

  frequencyEncoded = encodeFrequency((frequency * instrument->multiplier[1]) >> 12);
  m_channel_reg_A0[1] = frequencyEncoded & 0xFF;
  m_channel_reg_B0[1] &= 0xE0;
  m_channel_reg_B0[1] |= (frequencyEncoded >> 8) & 0x1F;

  frequencyEncoded = encodeFrequency((frequency * instrument->multiplier[2]) >> 12);
  m_channel_reg_A0[2] = frequencyEncoded & 0xFF;
  m_channel_reg_B0[2] &= 0xE0;
  m_channel_reg_B0[2] |= (frequencyEncoded >> 8) & 0x1F;
}

void Voice::setNoteOn(bool is_on)
{
  if (is_on) {
    if(!(check(FLAG_IS_ACTIVE))){
      set(FLAG_IS_ACTIVE);
      m_channel_reg_B0[0] |= 0x20;
      m_channel_reg_B0[1] |= 0x20;
      m_channel_reg_B0[2] |= 0x20;
    }
    else{
      set(FLAG_RETRIGGER);
    }
  }
  else {
    if(check(FLAG_IS_ACTIVE)){
      clear(FLAG_IS_ACTIVE);
      clear(FLAG_RETRIGGER);
      m_channel_reg_B0[0] &= ~(0x20);
      m_channel_reg_B0[1] &= ~(0x20);
      m_channel_reg_B0[2] &= ~(0x20);
    }
  }
}

bool Voice::isActive(){return check(FLAG_IS_ACTIVE);}

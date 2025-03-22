#include "voice.h"
#include <Arduino.h>


// Utility methods /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

// Converts MIDI pitch values in Q16.16 format into frequency values
// The output is 10.546ths of a Hertz (OPL3 base). That is, 1 unit translates to ~0.09482Hz or ~1/10.54587...
// A value of 0 << 16 means F#1, likewise 12 << 16 means F#2, and so on.
uint32_t Voice::pitchToFrequency(uint32_t q16_pitch)
{
  if (q16_pitch < (114 << 16)) // Pitch 114
  {
    // Split the Q16.16 pitch number into its Integer and Fractional components
    uint32_t pitch_integer_component = (q16_pitch >> 16);
    uint16_t pitch_fractional_component = q16_pitch & 0xFFFF;

    // Compute octave number using pitch_integer/12 (This bit-shifting magic does just that but avoiding division which can be slow)
    uint16_t octave = (pitch_integer_component * 1366) >> 14;

    // Get the index into the fnumber lookup table
    uint8_t lut_index = pitch_integer_component - (12*octave);

    // Interpolate between fnum_a and fnum_b based on the fractional part of pitch
    // Add in the octave number
    uint16_t fnum_a = lut_base_fnumber[lut_index];
    uint16_t fnum_b = lut_base_fnumber[lut_index + 1];
    return (fnum_a + ((pitch_fractional_component * (fnum_b - fnum_a)) >> 16)) << (octave);
  }
  return 0;
}

// Voice /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t Voice::s_connection_select = 0;

Voice::Voice() : instrument(nullptr) {
  for (uint8_t i = 0; i < 3; i++) {
    m_channel_reg_A0[i] = 0;
    m_channel_reg_B0[i] = 0;
  }
}

void Voice::setInstrument(Instrument& instrument) { this->instrument = &instrument; }

bool Voice::isActive(){return check(FLAG_IS_ACTIVE);}

void Voice::setPitch(uint32_t q16_pitch)
{
  uint32_t frequency = pitchToFrequency(q16_pitch);
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
  set(FLAG_IS_ACTIVE);
  if (is_on)
  {
    m_channel_reg_B0[0] |= 0x20;
    m_channel_reg_B0[1] |= 0x20;
    m_channel_reg_B0[2] |= 0x20;
  }
  else
  {
    m_channel_reg_B0[0] &= ~(0x20);
    m_channel_reg_B0[1] &= ~(0x20);
    m_channel_reg_B0[2] &= ~(0x20);
  }
  OPL::write_address(0xB0 + voice_offset + 0); OPL::write_data(m_channel_reg_B0[0]);
  OPL::write_address(0xB0 + voice_offset + 3); OPL::write_data(m_channel_reg_B0[1]);
  OPL::write_address(0xB0 + voice_offset + 6); OPL::write_data(m_channel_reg_B0[2]);
}

void setVolume(uint8_t volume)
{
  m_global_volume = volume;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * Load the instrument data into the voice.
 */
void Voice::loadToOPL()
{
  // Iterate through all 3 channels in the voice
  uint16_t voice_offset = (m_index < 4) ? m_index : (m_index + 0xFC);
  for (uint8_t ch = 0; ch < 3; ch++)
  {
    uint16_t offset = voice_offset + ch*8;
    for(uint8_t op = 0; op < 2; op++)
    {
      OPL::write_address(0x20 + offset); OPL::write_data(instrument->operator_reg_20[ch][op]); // AM | VIB | EGT | KSR | MULT

      // If this op is last in the chain of the current algorithm, 
      // let's overwrite its Total Level value with the global volume value.
      uint8_t reg_40_reformat = instrument->operator_reg_40[ch][op];
      if (is_last_in_chain[instrument->algorithm] & (1 >> (ch + offset)))
        reg_40_reformat = (reg_40_reformat & 0xC0) | m_volume;

      OPL::write_address(0x40 + offset); OPL::write_data(reg_40_reformat);                     // KSL     | Total Level
      OPL::write_address(0x60 + offset); OPL::write_data(instrument->operator_reg_60[ch][op]); // ATTACK  | DECAY
      OPL::write_address(0x80 + offset); OPL::write_data(instrument->operator_reg_80[ch][op]); // SUSTAIN | RELEASE
      OPL::write_address(0xE0 + offset); OPL::write_data(instrument->operator_reg_E0[ch][op]); // Waveform
      offset += 3;
    }  
  }

  // Stereo and CNT1
  OPL::write_address(0xC0 + voice_offset + 0); OPL::write_data(instrument->channel_reg_C0[0]); 
  OPL::write_address(0xC0 + voice_offset + 3); OPL::write_data(instrument->channel_reg_C0[1]);
  OPL::write_address(0xC0 + voice_offset + 6); OPL::write_data(instrument->channel_reg_C0[2]);

  // build register 0x104 (CONNECTION_SEL). (because its shared! ugh! lol)
  // algorithms 4, 5, 6 and 7 has 4OP mode enabled
  if (instrument->algorithm < 4) s_connection_select &= ~(1 << m_index);
  else                           s_connection_select |=  (1 << m_index);
  OPL::write_address(0x104); OPL::write_data(s_connection_select);
}

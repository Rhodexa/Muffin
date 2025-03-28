#include "voice.h"
#include <Arduino.h>

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
  // Split the Q16.16 pitch number into its Integer and Fractional components
  uint32_t pitch_integer_component = (q16_pitch >> 16);

  if (pitch_integer_component < 114) // Pitch 114
  {
    uint16_t pitch_fractional_component = q16_pitch & 0xFFFF;    
    uint16_t octave = (pitch_integer_component * 1366) >> 14;  // Compute octave number using pitch_integer/12 (This bit-shifting magic does just that but avoiding division which can be slow)
    uint8_t lut_index = pitch_integer_component - (12*octave); // Get the index into the fnumber lookup table

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
    m_chan[i].reg_A0 = 0;
    m_chan[i].reg_B0 = 0;
  }
}

void Voice::setInstrument(Instrument& inst) { this->instrument = &inst; }

void Voice::assingIndex(uint8_t index)
{
  m_self_index = index;
  m_voice_base_address = (m_self_index < 3) ? m_self_index : (m_self_index + 0xFD);
}


void Voice::setVolume(uint8_t volume)
{
  m_volume_scale = (volume >> 2) + 1;
}

// These function do writes on the OPL3! /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Voice::setPitch(uint32_t q16_pitch)
{
  uint32_t frequency = pitchToFrequency(q16_pitch);
  uint16_t
  frequencyEncoded = encodeFrequency((frequency * instrument->chan[0].multiplier) >> 12);
  m_chan[0].reg_A0 = frequencyEncoded & 0xFF;
  m_chan[0].reg_B0 &= 0xE0;
  m_chan[0].reg_B0 |= (frequencyEncoded >> 8) & 0x1F;

  frequencyEncoded = encodeFrequency((frequency * instrument->chan[1].multiplier) >> 12);
  m_chan[1].reg_A0 = frequencyEncoded & 0xFF;
  m_chan[1].reg_B0 &= 0xE0;
  m_chan[1].reg_B0 |= (frequencyEncoded >> 8) & 0x1F;

  frequencyEncoded = encodeFrequency((frequency * instrument->chan[2].multiplier) >> 12);
  m_chan[2].reg_A0 = frequencyEncoded & 0xFF;
  m_chan[2].reg_B0 &= 0xE0;
  m_chan[2].reg_B0 |= (frequencyEncoded >> 8) & 0x1F;

  OPL::write_address(m_voice_base_address + 0 + 0xA0); OPL::write_data(m_chan[0].reg_A0);
  OPL::write_address(m_voice_base_address + 3 + 0xB0); OPL::write_data(m_chan[0].reg_B0);
  OPL::write_address(m_voice_base_address + 6 + 0xA0); OPL::write_data(m_chan[1].reg_A0);
  OPL::write_address(m_voice_base_address + 0 + 0xB0); OPL::write_data(m_chan[1].reg_B0);
  OPL::write_address(m_voice_base_address + 3 + 0xA0); OPL::write_data(m_chan[2].reg_A0);
  OPL::write_address(m_voice_base_address + 6 + 0xB0); OPL::write_data(m_chan[2].reg_B0);
}

void Voice::setNoteOn(bool is_on)
{
  if (is_on)
  {
    m_chan[0].reg_B0 |= 0x20;
    m_chan[1].reg_B0 |= 0x20;
    m_chan[2].reg_B0 |= 0x20;
  }
  else
  {
    m_chan[0].reg_B0 &= ~(0x20);
    m_chan[1].reg_B0 &= ~(0x20);
    m_chan[2].reg_B0 &= ~(0x20);
  }
  OPL::write_address(m_voice_base_address + 0 + 0xB0); OPL::write_data(m_chan[0].reg_B0);
  OPL::write_address(m_voice_base_address + 3 + 0xB0); OPL::write_data(m_chan[1].reg_B0);
  OPL::write_address(m_voice_base_address + 6 + 0xB0); OPL::write_data(m_chan[2].reg_B0);
}

/*
 * Load the instrument data into the voice.
 * Voice_Base + 0 = Channel_0_Base + 0 = OP1_Base
 *                                 + 3 = OP2_Base
 *            + 3 = Channel_1_Base 
 *            + 6 = Channel_2_Base
 */
void Voice::loadToOPL()
{  
  // Iterate through all 3 channels in the voice
  uint16_t op_base = m_voice_base_address;
  for (uint8_t ch = 0; ch < 3; ch++)
  {
    for(uint8_t op = 0; op < 2; op++)
    {

      // If this op is last in the chain of the current algorithm, 
      // let's scale its volume with velocity. (Perhaps we can scale every thing with velocity based on another table, to allow for modulation!)
      // we could allow enabling scaling for each operator
      // or have different scalings for each.
      /*uint8_t reg_40_reformat = instrument->chan[ch].op[op].reg_40;
      uint8_t total_level = reg_40_reformat & 0x3F;
      if (instrument->flags_is_last_in_chain & (0b00100000 >> (ch*2 + op)))
        total_level = ( (total_level << 7) * m_volume_scale ) >> 7;
      reg_40_reformat &= 0xC0;
      reg_40_reformat |= (0x3F - total_level) & 0x3F;*/

      OPL::write_address(op_base + 0x20); OPL::write_data(instrument->chan[ch].op[op].reg_20); // AM | VIB | EGT | KSR | MULT
      OPL::write_address(op_base + 0x40); OPL::write_data(instrument->chan[ch].op[op].reg_40); // KSL     | Total Level
      OPL::write_address(op_base + 0x60); OPL::write_data(instrument->chan[ch].op[op].reg_60); // ATTACK  | DECAY
      OPL::write_address(op_base + 0x80); OPL::write_data(instrument->chan[ch].op[op].reg_80); // SUSTAIN | RELEASE
      OPL::write_address(op_base + 0xE0); OPL::write_data(instrument->chan[ch].op[op].reg_E0); // Waveform
      op_base += 3; // OP2 is always 3 addresses away from OP1
    }
    op_base += 5; // Next channel's OP1
  }

  // Stereo and CNT1
  OPL::write_address(m_voice_base_address + 0 + 0xC0); OPL::write_data(instrument->chan[0].reg_C0); 
  OPL::write_address(m_voice_base_address + 3 + 0xC0); OPL::write_data(instrument->chan[1].reg_C0);
  OPL::write_address(m_voice_base_address + 6 + 0xC0); OPL::write_data(instrument->chan[2].reg_C0);

  // build register 0x104 (CONNECTION_SEL). (because its shared! ugh! lol)
  // algorithms 4, 5, 6 and 7 has 4OP mode enabled
  if (instrument->algorithm < 4) s_connection_select &= ~(1 << m_self_index);
  else                           s_connection_select |=  (1 << m_self_index);
  OPL::write_address(0x104); OPL::write_data(s_connection_select);
}

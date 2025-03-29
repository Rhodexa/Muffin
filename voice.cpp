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
  uint32_t pitch_integer_component = (q16_pitch >> 16) + 6; // +6 corrects for MIDI. This fuction begins at F#, but MIDI starts at C

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

Voice::Voice() : instrument(nullptr)
{
  for (uint8_t i = 0; i < 3; i++)
  {
    m_chan[i].reg_A0 = 0;
    m_chan[i].reg_B0 = 0;
  }
}

void Voice::setInstrument(Instrument& inst)
{
  this->instrument = &inst;
}

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
  for(uint8_t i = 0; i < 3; i++)
  {
    uint16_t frequencyEncoded = encodeFrequency((frequency * instrument->chan[i].multiplier) >> 12);
    m_chan[i].reg_A0 = frequencyEncoded & 0xFF;
    m_chan[i].reg_B0 &= 0xE0;
    m_chan[i].reg_B0 |= (frequencyEncoded >> 8) & 0x1F;
  }

  for(uint8_t i = 0; i < 3; i++)
  {
    OPL::write((m_voice_base_address + i*3 + 0xA0), (instrument->chan[i].reg_A0));
    OPL::write((m_voice_base_address + i*3 + 0xB0), (instrument->chan[i].reg_B0));
  }
}

void Voice::sendNoteOn()
{
  for(uint8_t i = 0; i < 3; i++)
  {
    m_chan[i].reg_B0 |= 0x20;
    OPL::write((m_voice_base_address + i*3 + 0xB0), (instrument->chan[i].reg_B0 & ~(0x20)); // Set note-off first, to re-trigger the note
    OPL::write((m_voice_base_address + i*3 + 0xB0), (instrument->chan[i].reg_B0));      
  }
}

void Voice::sendNoteOff()
{
  for(uint8_t i = 0; i < 3; i++)
  {
    m_chan[i].reg_B0 &= ~(0x20);
    OPL::write((m_voice_base_address + i*3 + 0xB0), (instrument->chan[i].reg_B0));
  }
}

void Voice::loadToOPL()
{
  uint16_t op_base = m_voice_base_address;
  for (uint8_t ch = 0; ch < 3; ch++)
  {
    for(uint8_t op = 0; op < 2; op++)
    {
      /* uint8_t reg_40_reformat = instrument->chan[ch].op[op].reg_40;
      uint8_t total_level = reg_40_reformat & 0x3F;
      if (instrument->flags_is_last_in_chain & (0b00100000 >> (ch*2 + op)))
        total_level = ( (total_level << 7) * m_volume_scale ) >> 7;
      reg_40_reformat &= 0xC0;
      reg_40_reformat |= (0x3F - total_level) & 0x3F; */
      OPL::write((op_base + 0x20), (instrument->chan[ch].op[op].reg_20 | 0x20)); // AM | VIB | EGT(always set) | KSR | MULT
      OPL::write((op_base + 0x40), (instrument->chan[ch].op[op].reg_40));        // KSL     | Total Level
      OPL::write((op_base + 0x60), (instrument->chan[ch].op[op].reg_60));        // ATTACK  | DECAY
      OPL::write((op_base + 0x80), (instrument->chan[ch].op[op].reg_80));        // SUSTAIN | RELEASE
      OPL::write((op_base + 0xE0), (instrument->chan[ch].op[op].reg_E0));        // Waveform      
      OPL::write((m_voice_base_address + ch*3 + 0xC0), (instrument->chan[ch].reg_C0)); // Stereo and CNT1
      op_base += 3; // OP2 is always 3 addresses away from OP1
    }
    op_base += 2; // Next channel's OP1 is 8 addresses away from Prev chan's OP1
  }

  // build register 0x104 (CONNECTION_SEL). (because its shared! ugh! lol)
  // algorithms 4, 5, 6 and 7 has 4OP mode enabled
  if (instrument->algorithm < 4)
    s_connection_select &= ~(1 << m_self_index);
  else
    s_connection_select |=  (1 << m_self_index);
  OPL::write(0x104, s_connection_select);
}

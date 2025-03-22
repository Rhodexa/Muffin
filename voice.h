#ifndef VOICE_H
#define VOICE_H

#include <cstdint>
#include "opl.h"
#include "toolbits.h"
#include "instrument.h"

class Voice
{
private:
  Instrument* instrument;
  uint8_t m_index;

  // This table begins at F#1 and ends at F#2
  static constexpr uint16_t lut_base_fnumber[13] = {
    488, 517, 547, 580, 614, 651,
    690, 731, 774, 820, 869, 921,
    976
  };

  static uint32_t pitchToFrequency(uint32_t pitch);
  static uint16_t encodeFrequency(uint32_t frequency);
  
public:
  // Shared. represents the state of the 0x104 register... annoying, yes!
  static uint8_t s_connection_select;

  // Used for velocity control
  uint8_t m_global_volume;

  // Fnumber, Octave and KeyOn ( These are what I like to call the MIDI Registers! )
  uint8_t m_channel_reg_A0[3]; 
  uint8_t m_channel_reg_B0[3];

  Voice();
  void setInstrument(Instrument& instrument);
  void setPitch(uint32_t q16_pitch);
  void setVolume(uint8_t volume);
  void setNoteOn(bool is_on);
  bool isActive();
  void loadToOPL();
};

#endif
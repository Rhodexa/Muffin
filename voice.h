#ifndef VOICE_H
#define VOICE_H

#include <cstdint>
#include "opl.h"
#include "toolbits.h"
#include "instrument.h"

class Voice {
public:
  Instrument* instrument;
  uint8_t  m_index;

  // This table begins at F#1 and ends at F#2
  static constexpr uint16_t lut_base_fnumber[13] = {
    488, 517, 547, 580, 614, 651,
    690, 731, 774, 820, 869, 921,
    976
  };
  
public:
  // shared. represents the state of the 0x104 register... annoying, yes!
  static uint8_t s_connection_select;

  // Fnumber and Octave
  uint8_t m_channel_reg_A0[3]; 
  uint8_t m_channel_reg_B0[3];
  
  uint8_t m_flags;
  enum Flags {
    FLAG_IS_ACTIVE = 1 << 0,
    FLAG_RETRIGGER = 1 << 1
  };
  bool check(uint8_t flag);
  void set(uint8_t flag);
  void clear(uint8_t flag);

public:
  static uint32_t pitchToFrequency(uint32_t pitch);
  static uint16_t encodeFrequency(uint32_t frequency);

public:
  Voice();
  void setInstrument(Instrument& instrument);
  void setPitch(uint32_t q16_pitch);
  void setNoteOn(bool is_on);
  bool isActive();
  void loadToOPL();
};

#endif
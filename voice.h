#ifndef VOICE_H
#define VOICE_H

#include <cstdint>
#include "opl.h"
#include "toolbits.h"
#include "instrument.h"

class Voice
{
private:
  static uint32_t pitchToFrequency(uint32_t pitch);
  static uint16_t encodeFrequency(uint32_t frequency);

private:
  // This table begins at F#1 and ends at F#2
  static constexpr uint16_t lut_base_fnumber[13] = {
    488, 517, 547, 580, 614, 651,
    690, 731, 774, 820, 869, 921,
    976
  };

  Instrument* instrument;
  uint8_t m_self_index;
  uint16_t m_voice_base_address;

  uint8_t m_flags;
  enum Flags {
    FLAG_IS_ACTIVE = 1 << 0,
    FLAG_RETRIGGER = 1 << 1
  };
  bool check(uint8_t flag);
  void set(uint8_t flag);
  void clear(uint8_t flag);

public:
  static uint8_t s_connection_select; // Shared. represents the state of the 0x104 register... annoying, yes!
  uint8_t m_volume_scale;            // Used for velocity control, Q1.7 format
  uint8_t m_channel_reg_A0[3];        // Fnumber_L
  uint8_t m_channel_reg_B0[3];        // Fnumber_H, Octave and KeyOn

  Voice();
  void assingIndex(uint8_t index);
  void setInstrument(Instrument& instrument);
  void setPitch(uint32_t q16_pitch);
  void setVolume(uint8_t volume);
  void setNoteOn(bool is_on);
  bool isActive();
  void loadToOPL();
};

#endif
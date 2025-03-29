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
  typedef struct Channel {
    uint8_t reg_A0;
    uint8_t reg_B0;
  };

private:
  // This table begins at F#1 and ends at F#2
  static constexpr uint16_t lut_base_fnumber[13] = {
    488, 517, 547, 580, 614, 651,
    690, 731, 774, 820, 869, 921,
    976
  };

  Instrument *instrument;
  uint8_t m_self_index;
  uint16_t m_voice_base_address;

public:
  static uint8_t s_connection_select; // Shared. represents the state of the 0x104 register... annoying, yes!
  uint8_t m_volume_scale;            // Used for velocity control, Q1.7 format

  Channel m_chan[3];

  Voice();
  void assingIndex(uint8_t index);
  void setInstrument(Instrument &inst);
  void setPitch(uint32_t q16_pitch);
  void setVolume(uint8_t volume);
  void sendNoteOn();
  void sendNoteOff();
  bool isActive();
  void loadToOPL();
};

#endif
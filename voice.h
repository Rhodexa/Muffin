#ifndef VOICE_H
#define VOICE_H

#include <cstdint>
#include "opl.h"
#include "toolbits.h"
#include "instrument.h"

class Voice
{
private:
  static constexpr uint16_t lut_base_fnumber[13] =
  { /*
    F#1  G1   G#1  A1   A#1  B1   C2   C#2  D2   D#2  E2   F2   F#2 */ 
    488, 517, 547, 580, 614, 651, 690, 731, 774, 820, 869, 921, 976
  };
  static uint32_t pitchToFrequency(uint32_t pitch);
  static uint16_t encodeFrequency(uint32_t frequency);
  static uint8_t s_connection_select; // Shared. represents the state of the 0x104 register... annoying, yes!

private:
  Instrument *instrument;
  uint8_t m_self_index;
  uint16_t m_voice_base_address;

public:
  uint8_t sequence_1[16]; // Sequences are cool little things, they get updated every frame and can modulate parts of the voice playing.

  uint8_t m_volume_scale; // Used for velocity control, Q1.7 format
  uint8_t m_chan_reg_B0[3];

  Voice();
  void assingIndex(uint8_t index);
  void setInstrument(Instrument &inst);

  // Interface
  void setPitch(uint32_t q16_pitch);
  void setVolume(uint8_t volume);
  void sendNoteOn();
  void sendNoteOff();
  // updates

  void update();
  void loadToOPL();
};

#endif
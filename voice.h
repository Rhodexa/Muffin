#ifndef VOICE_H
#define VOICE_H

#include "toolbits.h"
#include "instrument.h"

class Voice {
public:
  Instrument& instrument;
  uint8_t m_selfIndex;

  // OPL3-specific memory:
  uint8_t m_channel_reg_A0[3];
  uint8_t m_channel_reg_B0[3];

  uint16_t encodeFrequency(uint32_t frequency);
  void setFrequency(uint32_t frequency);
  void setNoteOn();
  void setInstrument(Instrument& instr);
}

#endif
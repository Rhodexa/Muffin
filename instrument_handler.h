#ifndef INSTRUMENT_HANDLER_H
#define INSTRUMENT_HANDLER_H

#include <cstdint>
#include "toolbits.h"
#include "instrument.h"


class InstrumentHandler {
public:
  Instrument* m_inst;
  uint8_t m_operator;
  uint8_t m_channel;

public:
  InstrumentHandler(Instrument& instrument);
  void setActiveOperator(uint8_t op);
  void setActiveChannel (uint8_t ch);
  void setActiveInstrument(Instrument& inst);

public:
  // Operators
  void op_setAmplitude (uint8_t value);
  void op_setAttack    (uint8_t value);
  void op_setDecay     (uint8_t value);
  void op_setSustain   (uint8_t value);
  void op_setRelease   (uint8_t value);
  void op_setMultiplier(uint8_t value);
  void op_setWaveform  (uint8_t value);
  void op_setUseVibratro   (uint8_t value);
  void op_setUseTremolo    (uint8_t value);
  void op_setKeyScaleRate  (uint8_t value);
  void op_setKeyScaleLevel (uint8_t value);

  // Channels
  void ch_setFeedback(uint8_t value);
  void ch_setStereoSwitches(uint8_t value);
  void ch_setMultiplier(uint16_t value);  

  // Instrument Globals
  void setAlgorithm(uint8_t value);
  void setVibratoDepth(uint8_t value);
  void setTremoloDepth(uint8_t value);
};




#endif
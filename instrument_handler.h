#ifndef INSTRUMENT_HANDLER_H
#define INSTRUMENT_HANDLER_H

#include <cstdint>
#include "toolbits.h"
#include "instrument.h"


class InstrumentHandler {
private:
  Instrument* m_inst;
  uint8_t m_operator;
  uint8_t m_channel;

public:
  InstrumentHandler();
  void selectOperator(uint8_t op);
  void selectChannel (uint8_t ch);
  void setInstrument(Instrument& inst);

public:
  // Operators
  void setAmplitude (uint8_t value);
  void setAttack    (uint8_t value);
  void setDecay     (uint8_t value);
  void setSustain   (uint8_t value);
  void setRelease   (uint8_t value);
  void setMultiplier(uint8_t value);
  void setWaveform  (uint8_t value);
  void setUseVibratro   (uint8_t value);
  void setUseTremolo    (uint8_t value);
  void setKeyScaleRate  (uint8_t value);
  void setKeyScaleLevel (uint8_t value);

  // Channels
  void setFeedback(uint8_t value);
  void setConnectionType(uin8_t value);
  void setStereoSwitches(uint8_t value);
  void setMultiplier();  

  // Instrument Globals
  void setAlgorithm();
  void setVibratoDepth();
  void setTremoloDepth();
};




#endif
#ifndef INSTRUMENT_HANDLER_H
#define INSTRUMENT_HANDLER_H

/*
  Call me crazy for structuring this in such an ugly way, but believe me, I have  tried a lot of tricks,
  and this, while not super elegant, makes everything soooo much simpler in my opinion.
  And I'm willing to sacrifice elegance for development time and prototyping...
  we can always refine things later!

  "Feel free to comment how wrong I am down below! It boosts engagement!" — Alec Watson
*/
// Rant aside...

#include "toolbits.h"
#include "instrument.h"

class InstrumentHandler {
private:
  Instrument& instrument;
  uint8_t operator;
  uint8_t channel;
  uint8_t channel_index;

public:
  InstrumentHandler();
  void selectOperator(uint8_t op);
  void selectChannel (uint8_t ch);
  void setInstrumentReference(Instrument& inst);

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
  void setKeyOn(uint8_t value);
  void setFrequency();
  void setMultiplier();  

  // Instrument Globals
  void setAlgorithm();
  void setVibratoDepth();
  void setTremoloDepth();
};




#endif
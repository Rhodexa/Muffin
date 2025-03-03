#ifndef INSTRUMENT_H
#define INSTRUMENT_H
#include "operator.h"
#include "oplhw.h"

/*
  Call me crazy for structuring this in such an ugly way, but believe me, I have  tried a lot of tricks,
  and this, while not super elegant, makes everything soooo much simpler in my opinion.
  And I'm willing to sacrifice elegance for development time and prototyping...
  we can always refine things later!

  "Feel free to comment how wrong I am down below! It boosts engagement!" — Alec Watson
*/
// Rant aside...


/*
  Having an Instrument struct allows for a bunch of neat little things!
  specifically, dynamic loading of new instruments, voice isolation, multiple instruments on the same keyboard
  and even saving and loading presets on external memory.

  OPL3 register values can be constructed dynamically rather than having to keep track of "shadow" registers because the instrument itself is a shadow register.
*/

typedef struct Instrument {
  // OPL3-specific memory:
  uint8_t operator[30]; // operator > register
  uint8_t channel[9];   // channel  > register
  uint8_t connection_select;
  uint8_t note_type_select;
  uint8_t vibrato_tremolo_depth_flags;

  // Other parameters that make up the "instrument":
  uint8_t algorithm;
};

class InstrumentHandler {
private:
  // The getters and setters work like a state machine;
  //   You need to select the operator _first_ and then use the setters and getters
  Instrument& instrument;
  uint8_t m_selectedOperatorOffset = 0;
  uint8_t m_selectedChannelOffset = 0;

public:
  void writeRegField(uint8_t& reg, uint8_t mask, uint8_t data, uint8_t shift){ reg &= (~(mask)) | (data << shift) & mask; }
  uint8_t readRegField(uint8_t& reg, uint8_t mask, uint8_t shift){ return (reg & mask) >> shift }
  void selectOperator(uint8_t op){ m_selectedOperatorOffset = op * 5; }
  void selectChannel (uint8_t ch){ m_selectedChannelOffset  = ch * 3; }
  void setInstrumentReference(Instrument& inst) {instrument = inst;}

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
  void setFrequency(); // This is a tricky one, because it should use the Pitch object and multiplier variable to generate its output
  

  // Instrument Globals
  void setAlgorithm();
  void setVibratoDepth();
  void setTremoloDepth();
};




#endif
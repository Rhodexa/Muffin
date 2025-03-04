#include "instrument_handler.h"

void Instrument::setInstrumentReference(Instrument& inst) {instrument = inst;}
void Instrument::selectOperator(uint8_t op){ operator = op * 5; }
void Instrument::selectChannel(uint8_t ch){ channel = ch * 3; channel_index = ch;}

// Operators
void Instrument::setAmplitude(uint8_t value)      { writeRegField(operator_registers[operator + 1], 0x3F, 0, value); }
void Instrument::setAttack(uint8_t value)         { writeRegField(operator_registers[operator + 2], 0xF0, 4, value); }
void Instrument::setDecay(uint8_t value)          { writeRegField(operator_registers[operator + 2], 0x0F, 0, value); }
void Instrument::setSustain(uint8_t value)        { writeRegField(operator_registers[operator + 3], 0xF0, 4, value); }
void Instrument::setRelease(uint8_t value)        { writeRegField(operator_registers[operator + 3], 0x0F, 0, value); }
void Instrument::setMultiplier(uint8_t value)     { writeRegField(operator_registers[operator + 0], 0x0F, 0, value); }
void Instrument::setWaveform(uint8_t value)       { writeRegField(operator_registers[operator + 4], 0x07, 0, value); }
void Instrument::setUseVibratro(uint8_t value)    { writeRegField(operator_registers[operator + 0], 0x40, 6, value); }
void Instrument::setUseTremolo(uint8_t value)     { writeRegField(operator_registers[operator + 0], 0x80, 7, value); }
void Instrument::setKeyScaleRate(uint8_t value)   { writeRegField(operator_registers[operator + 0], 0x10, 4, value); }
void Instrument::setKeyScaleLevel(uint8_t value)  { writeRegField(operator_registers[operator + 1], 0xC0, 6, value); }

// Channels
void Instrument::setFeedback(uint8_t value)       { writeRegField(channel_registers[channel + 2], 0x0E, 1, value); }
void Instrument::setConnectionType(uin8_t value)  { writeRegField(channel_registers[channel + 2], 0x01, 0, value); }
void Instrument::setStereoSwitches(uint8_t value) { writeRegField(channel_registers[channel + 2], 0xC0, 6, value); }
void Instrument::setMultiplier(uint16_t value)    { multiplier[channel_index] = value; }
void Instrument::setFrequency() {
  /// hmmm... actually, he frequency setting is part of the voicing and controllers, not the instrument
}

// Instrument Globals
void Instrument::setVibratoDepth();
void Instrument::setTremoloDepth();
void Instrument::setAlgorithm();
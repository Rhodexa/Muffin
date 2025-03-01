#ifndef SYNTH_OPERATOR_H
#define SYNTH_OPERATOR_H

namespace Synth {
namespace Operator {

struct Operator {
  uint16_t offset;
  uint8_t REG_20;
  uint8_t REG_40;
  uint8_t REG_60;
  uint8_t REG_80;
  uint8_t REG_E0;
};

void setAddressOffset(Operator& operator, uint16_t offset) {
  operator.offset = offset;
}

/* Getters */
uint8_t getAmplitude(Operator& operator)  { return (operator.REG_40 & 0x3F) >> 0; }
uint8_t getAttack(Operator& operator)     { return (operator.REG_60 & 0xF0) >> 4; }
uint8_t getDecay(Operator& operator)      { return (operator.REG_60 & 0x0F) >> 0; }
uint8_t getSustain(Operator& operator)    { return (operator.REG_80 & 0xF0) >> 4; }
uint8_t getRelease(Operator& operator)    { return (operator.REG_80 & 0x0F) >> 0; }
uint8_t getWaveform(Operator& operator)   { return (operator.REG_E0 & 0x07) >> 0; }
uint8_t getMultiplier(Operator& operator) { return (operator.REG_20 & 0x0F) >> 0; }


/* Setters */
void setAmplitude(Operator& operator, uint8_t value) {
  operator.REG_40 &= 0xC0;
  operator.REG_40 |= value & 0x3F;
}
void setAttack(Operator& operator, uint8_t value){
  operator.REG_60 &= 0x0F;
  operator.REG_60 |= (value << 4) & 0xF0;
}
void setDecay(Operator& operator, uint8_t value){
  operator.REG_60 &= 0xF0;
  operator.REG_60 |= value & 0x0F;
}
void setSustain(Operator& operator, uint8_t value){
  operator.REG_80 &= 0x0F;
  operator.REG_80 |= (value << 4) & 0xF0;
}
void setRelease(Operator& operator, uint8_t value){
  operator.REG_80 &= 0xF0;
  operator.REG_80 |= value & 0x0F;
}
void setWaveform(Operator& operator, uint8_t value){
  operator.REG_E0 = value & 0x07;
}
void setMultiplier(Operator& operator, uint8_t value){
  operator.REG_20 &= 0xF0;
  operator.REG_20 |= value & 0x0F;
}

}} // End namespace Synth::Operator
#endif
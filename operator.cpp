#include "operator.h"

void Operator::setAddressOffset(Operator& operator, uint16_t offset) {
  operator.offset = offset;
}

/* Getters */
uint8_t Operator::getAmplitude(Operator& operator)  { return (operator.REG_40 & 0x3F) >> 0; }
uint8_t Operator::getAttack(Operator& operator)     { return (operator.REG_60 & 0xF0) >> 4; }
uint8_t Operator::getDecay(Operator& operator)      { return (operator.REG_60 & 0x0F) >> 0; }
uint8_t Operator::getSustain(Operator& operator)    { return (operator.REG_80 & 0xF0) >> 4; }
uint8_t Operator::getRelease(Operator& operator)    { return (operator.REG_80 & 0x0F) >> 0; }
uint8_t Operator::getWaveform(Operator& operator)   { return (operator.REG_E0 & 0x07) >> 0; }
uint8_t Operator::getMultiplier(Operator& operator) { return (operator.REG_20 & 0x0F) >> 0; }


/* Setters */
void Operator::setAmplitude(Operator& operator, uint8_t value) {
  operator.REG_40 &= 0xC0;
  operator.REG_40 |= value & 0x3F;
}
void Operator::setAttack(Operator& operator, uint8_t value){
  operator.REG_60 &= 0x0F;
  operator.REG_60 |= (value << 4) & 0xF0;
}
void Operator::setDecay(Operator& operator, uint8_t value){
  operator.REG_60 &= 0xF0;
  operator.REG_60 |= value & 0x0F;
}
void Operator::setSustain(Operator& operator, uint8_t value){
  operator.REG_80 &= 0x0F;
  operator.REG_80 |= (value << 4) & 0xF0;
}
void Operator::setRelease(Operator& operator, uint8_t value){
  operator.REG_80 &= 0xF0;
  operator.REG_80 |= value & 0x0F;
}
void Operator::setWaveform(Operator& operator, uint8_t value){
  operator.REG_E0 = value & 0x07;
}
void Operator::setMultiplier(Operator& operator, uint8_t value){
  operator.REG_20 &= 0xF0;
  operator.REG_20 |= value & 0x0F;
}
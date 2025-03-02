#include "operator.h"

void Operator::setAddressOffset(uint16_t offset) {
  this->offset = offset;
}

/* Getters */
uint8_t Operator::getAmplitude()  { return (REG_40 & 0x3F) >> 0; }
uint8_t Operator::getAttack()     { return (REG_60 & 0xF0) >> 4; }
uint8_t Operator::getDecay()      { return (REG_60 & 0x0F) >> 0; }
uint8_t Operator::getSustain()    { return (REG_80 & 0xF0) >> 4; }
uint8_t Operator::getRelease()    { return (REG_80 & 0x0F) >> 0; }
uint8_t Operator::getWaveform()   { return (REG_E0 & 0x07) >> 0; }
uint8_t Operator::getMultiplier() { return (REG_20 & 0x0F) >> 0; }


/* Setters */
void Operator::setAmplitude(uint8_t value) {
  REG_40 &= 0xC0;
  REG_40 |= value & 0x3F;
}

void Operator::setAttack(uint8_t value){
  REG_60 &= 0x0F;
  REG_60 |= (value << 4) & 0xF0;
}

void Operator::setDecay(uint8_t value){
  REG_60 &= 0xF0;
  REG_60 |= value & 0x0F;
}

void Operator::setSustain(uint8_t value){
  REG_80 &= 0x0F;
  REG_80 |= (value << 4) & 0xF0;
}

void Operator::setRelease(uint8_t value){
  REG_80 &= 0xF0;
  REG_80 |= value & 0x0F;
}

void Operator::setWaveform(uint8_t value){
  REG_E0 = value & 0x07;
}

void Operator::setMultiplier(uint8_t value){
  REG_20 &= 0xF0;
  REG_20 |= value & 0x0F;
}
#ifndef OPERATOR_H
#define OPERATOR_H

#include <cstdint>

class Operator {
public:
  uint16_t offset;
  uint8_t REG_20;
  uint8_t REG_40;
  uint8_t REG_60;
  uint8_t REG_80;
  uint8_t REG_E0;

public:
  void setAddressOffset(Operator& operator, uint16_t offset);

  /* Getters */
  uint8_t getAmplitude(Operator& operator);
  uint8_t getAttack(Operator& operator);
  uint8_t getDecay(Operator& operator);
  uint8_t getSustain(Operator& operator);
  uint8_t getRelease(Operator& operator);
  uint8_t getWaveform(Operator& operator);
  uint8_t getMultiplier(Operator& operator);

  /* Setters */
  void setAmplitude(Operator& operator, uint8_t value);
  void setAttack(Operator& operator, uint8_t value);
  void setDecay(Operator& operator, uint8_t value),
  void setSustain(Operator& operator, uint8_t value);
  void setRelease(Operator& operator, uint8_t value);
  void setWaveform(Operator& operator, uint8_t value);
  void setMultiplier(Operator& operator, uint8_t value);
}

#endif
#ifndef OPERATOR_H
#define OPERATOR_H

#include <cstdint>

class Operator {
public:
  uint8_t REG_20;
  uint8_t REG_40;
  uint8_t REG_60;
  uint8_t REG_80;
  uint8_t REG_E0;

public:
  Operator() : REG_20(0), REG_40(0), REG_60(0), REG_80(0), REG_E0(0) {}

  /* Getters */
  uint8_t getAmplitude();
  uint8_t getAttack();
  uint8_t getDecay();
  uint8_t getSustain();
  uint8_t getRelease();
  uint8_t getWaveform();
  uint8_t getMultiplier();

  /* Setters */
  void setAmplitude(uint8_t value);
  void setAttack(uint8_t value);
  void setDecay(uint8_t value),
  void setSustain(uint8_t value);
  void setRelease(uint8_t value);
  void setWaveform(uint8_t value);
  void setMultiplier(uint8_t value);
};

#endif
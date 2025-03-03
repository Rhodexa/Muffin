#ifdef CHANNEL_H
#define CHANNEL_H

#include "operator.h"

class Channel {
public:
  Operator op[2];
  uint8_t REG_A0;
  uint8_t REG_B0;
  uint8_t REG_C0;

public:
  uint8_t multiplier;

  void setKeyOn(uint8_t );
  void setFrequency(uint16_t fnumber, uint8_t octave);
  void setStereoSwitch(uint8_t left, uint8_t right);
};

#endif
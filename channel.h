#ifdef CHANNEL_H
#define CHANNEL_H

#include "operator.h"

class Channel {
public:
  Operator op[2];
  uint8_t REG_A0;
  uint8_t REG_B0;
  uint8_t REG_C0;

  void setFrequency(uint16_t fnumber, uint8_t octave){
    
  }
};

#endif
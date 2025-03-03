#ifndef VOICE_H
#define VOICE_H

#include "channel.h"

class Voice {
  Channel chan[3];
  void setAlgorithm(uint8_t algorithm);
};

#endif
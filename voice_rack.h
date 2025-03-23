#ifndef VOICERACK_H
#define VOICERACK_H

#include "voice.h"
#include "opl.h"
#include "instrument.h"

namespace VoiceRack
{
  // A weird way to create a global variable
  inline Voice voice[6];
  void init(Instrument& instrument);
}

#endif
#ifndef NOTE_WRANGLER_H
#define NOTE_WRANGLER_H

/*
  Note wrangler. This module takes in MIDI messages and assigns them to the different Voices.
  There's two types of wranglers.

  Normal Wrangler:
    Typical, normal, polyphonic wrangler. Assigns new voices as notes comes in.

  Mono Wrangler:
    Uses only one voice <MIDI channel defines which voice to use, 0 to 5>
    Allows Porta and Legato
*/


#include "voice_rack.h"

class NormalWrangler
{
private:
  uint8_t m_current_voice = 0;

public:
  uint8_t held_key[6] = {0, 0, 0, 0, 0, 0};

public:
  NormalWrangler();
  void handleNoteOn(uint8_t pitch);
  void handleNoteOff(uint8_t pitch);
};

#endif
#ifndef NOTE_WRANGLER_H
#define NOTE_WRANGLER_H

#include "voice.h"

/*
  Note wrangler. This module takes in MIDI messages and assigns them to the different Voices.
  There's two types of wranglers.

  Normal Wrangler:
    Typical, normal, polyphonic wrangler. Assigns new voices as notes comes in.

  Mono Wrangler:
    Uses only one voice <MIDI channel defines which voice to use, 0 to 5>
    Allows Porta and Legato
*/

class NormalWrangler {
  Voice* voices;

public:
  NormalWrangler(Voice* voice_array);

  void handleNoteOn(uint16_t pitch, ){

  }
};

#endif
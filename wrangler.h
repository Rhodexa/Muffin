#ifndef NOTE_WRANGLER_H
#define NOTE_WRANGLER_H

#include "voice_rack.h"

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
private:
  Voice* voices;
  uint8_t m_current_voice = 0;

public:
  uint8_t held_key[6] = {0, 0, 0, 0, 0, 0};

public:
  NormalWrangler(Voice* voice_array);  
  void setVoicesPointer();
  void handleNoteOn(uint8_t pitch);

};

#endif
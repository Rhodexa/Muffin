#include "wrangler.h"

NormalWrangler::NormalWrangler() {}

void NormalWrangler::handleNoteOn(uint8_t pitch)
{
  // Find the next available voice
  for (uint8_t i = 0; i < 5; i++)
  {
    current_voice++;
    if (current_voice >= 6) current_voice = 0;
    if (!voices[current_voice].isActive()) break;
  }
  held_key[current_voice] = pitch;
  uint32_t frq = pitchToFrequency(pitch << 16);
  voices[current_voice].setFrequency(frq);
  voices[current_voice].setNoteOn(true);
  handler.op_setWaveform(current_voice);
}

void NormalWrangler::handleNoteOff(uint8_t pitch){
  for (uint8_t i = 0; i < 6; i++)
  {
    if (held_key[i] == pitch)
    {
      held_key[i] = 0;
      voices[i].setNoteOn(false);
    }
  }
}
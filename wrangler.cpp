#include "wrangler.h"

NormalWrangler::NormalWrangler() {}

void NormalWrangler::handleNoteOn(uint8_t pitch)
{
  // Find the next available voice
  for (uint8_t i = 0; i < 5; i++)
  {
    m_current_voice++;
    if (m_current_voice >= 6) m_current_voice = 0;
    if (!VoiceRack::voice[m_current_voice].isActive()) break;
  }
  held_key[m_current_voice] = pitch;
  VoiceRack::voice[m_current_voice].setPitch(pitch << 16);
  VoiceRack::voice[m_current_voice].setVolume(0x7F);
  VoiceRack::voice[m_current_voice].setNoteOn(true);
  //handler.op_setWaveform(m_current_voice);
}

void NormalWrangler::handleNoteOff(uint8_t pitch){
  for (uint8_t i = 0; i < 6; i++)
  {
    if (held_key[i] == pitch)
    {
      held_key[i] = 0;
      VoiceRack::voice[i].setNoteOn(false);
    }
  }
}
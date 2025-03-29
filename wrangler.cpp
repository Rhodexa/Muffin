#include "wrangler.h"

NormalWrangler::NormalWrangler() {}

// This Wrangler was tested with Necrofantasia (Black MIDI)... and it sort of passed!
void NormalWrangler::handleNoteOn(uint8_t pitch)
{
  // Find the next available voice
  for (uint8_t i = 0; i < 7; i++) // 0 0 0 0 0 0
  {
    if (held_key[m_current_voice] == -1) break;
    m_current_voice++;
    if (m_current_voice >= 6) m_current_voice = 0;
  }
  held_key[m_current_voice] = pitch;
  VoiceRack::voice[m_current_voice].setPitch(pitch << 16);
  VoiceRack::voice[m_current_voice].sendNoteOn();
}

void NormalWrangler::handleNoteOff(uint8_t pitch){
  for (uint8_t i = 0; i < 6; i++)
  {
    if (held_key[i] == pitch)
    {
      held_key[i] = -1;
      VoiceRack::voice[i].sendNoteOff();
    }
  }
}

void NormalWrangler::update(){
}
/*
#include "wrangler.h"

NormalWrangler::NormalWrangler() {}

void NormalWrangler::handleNoteOn(uint8_t pitch)
{
  // Find the next available voice
  m_current_voice = 0;
  held_key[m_current_voice] = pitch;
  pitch_target[m_current_voice] = pitch << 16;
  //VoiceRack::voice[m_current_voice].setPitch(pitch << 16);
  VoiceRack::voice[m_current_voice].sendNoteOn();
}

void NormalWrangler::handleNoteOff(uint8_t pitch){
  for (uint8_t i = 0; i < 6; i++)
  {
    if (held_key[i] == pitch)
    {
      held_key[i] = -1;
      VoiceRack::voice[i].sendNoteOff();
    }
  }
}

void NormalWrangler::update(){
  //if(pitch_current[0] < pitch_target[0]) pitch_current[0] += 1<<7;
  //if(pitch_current[0] > pitch_target[0]) pitch_current[0] -= 1<<7;
  pitch_current[0] += ((pitch_target[0] - pitch_current[0]) * 0.0025f);
  VoiceRack::voice[0].setPitch(pitch_current[0]);
}*/
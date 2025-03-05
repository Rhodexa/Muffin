#ifndef SYNTH_H
#define SYNTH_H

#include "oplrw.h"
#include "voice.h"
#include "instrument.h"

namespace Synth {

constexpr uint16_t lut_base_frq[13] = {
  488, 517, 547, 580, 614, 651,
  690, 731, 774, 820, 869, 921,
  976
};

Voice voices[6];
uint8_t m_current_voice = 0;
uint8_t held_key[6] = {0, 0, 0, 0, 0, 0};

void begin();

uint16_t pitchToFrequency(uint32_t q16_pitch);
void updateOPL(){
  for (uint8_t i = 0; i < 6; i++){
    voices[i].loadToOPL();
  }
}

void handleNoteOn(uint8_t pitch);

}

#endif
#ifndef SYNTH_H
#define SYNTH_H

#include "oplrw.h"
#include "voice.h"
#include "instrument.h"
#include "voice.h"

namespace Synth {

// operator_offset[voice][channel][operator]
constexpr uint16_t operator_offset[6][3][2] = {
  { {0x000, 0x003}, {0x008, 0x00B}, {0x010, 0x013} },
  { {0x001, 0x004}, {0x009, 0x00C}, {0x011, 0x014} },
  { {0x002, 0x005}, {0x00A, 0x00D}, {0x012, 0x015} },
  { {0x100, 0x103}, {0x108, 0x10B}, {0x110, 0x113} },
  { {0x101, 0x104}, {0x109, 0x10C}, {0x111, 0x114} },
  { {0x102, 0x105}, {0x10A, 0x10D}, {0x112, 0x115} }
};

// operator_offset[voice][channel]
constexpr uint16_t channel_offset[6][3] = {
  { 0x000, 0x001, 0x002 },
  { 0x003, 0x004, 0x005 },
  { 0x006, 0x007, 0x008 },
  { 0x100, 0x101, 0x102 },
  { 0x103, 0x104, 0x105 },
  { 0x106, 0x107, 0x108 }
};

constexpr uint16_t lut_base_frq[13] = {
  488, 517, 547, 580, 614, 651,
  690, 731, 774, 820, 869, 921,
  976
};

Voice voices[6];
void begin();
uint16_t pitchToFrequency(uint32_t q16_pitch);
void handleNoteOn(uint32_t frequency);

}
#endif
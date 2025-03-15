#ifndef VOICE_H
#define VOICE_H

#include <cstdint>
#include "opl.h"
#include "toolbits.h"
#include "instrument.h"

class Voice {
public:
  Instrument* instrument;
  uint8_t m_selfIndex;

  // operator_offset[voice][channel][operator]
  static constexpr uint16_t operator_offset[6][3][2] = {
    { {0x000, 0x003}, {0x008, 0x00B}, {0x010, 0x013} },
    { {0x001, 0x004}, {0x009, 0x00C}, {0x011, 0x014} },
    { {0x002, 0x005}, {0x00A, 0x00D}, {0x012, 0x015} },
    { {0x100, 0x103}, {0x108, 0x10B}, {0x110, 0x113} },
    { {0x101, 0x104}, {0x109, 0x10C}, {0x111, 0x114} },
    { {0x102, 0x105}, {0x10A, 0x10D}, {0x112, 0x115} }
  };

  // channel_offset[voice][channel]
  static constexpr uint16_t channel_offset[6][3] = {
    { 0x000, 0x003, 0x006 },
    { 0x001, 0x004, 0x007 },
    { 0x002, 0x005, 0x008 },
    { 0x100, 0x103, 0x106 },
    { 0x101, 0x104, 0x107 },
    { 0x102, 0x105, 0x108 }
  };
  
public:
  // shared. represents the state of the 0x104 register... annoying, yes!
  static uint8_t s_connection_select;

  // Fnumber and Octave
  uint8_t m_channel_reg_A0[3]; 
  uint8_t m_channel_reg_B0[3];
  
  uint8_t m_flags;
  enum Flags {
    FLAG_IS_ACTIVE = 1 << 0,
    FLAG_RETRIGGER = 1 << 1
  };
  bool check(uint8_t flag);
  void set(uint8_t flag);
  void clear(uint8_t flag);

public:
  Voice();
  void setInstrument(Instrument& instrument);
  static uint16_t encodeFrequency(uint32_t frequency);
  void setFrequency(uint32_t frequency);
  void setNoteOn(bool is_on);
  bool isActive();
  void loadToOPL();
};

#endif
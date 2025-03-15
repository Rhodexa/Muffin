#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <cstdint>

typedef struct Instrument {
  /* Global */
  // Reg 0xBD [DAM, DVB, RYT, BD, SD, TOM, TC, HH] we only use the first two bits, though.
  uint8_t vibrato_tremolo_depth_flags; 
  
  // Reg 0x008 [-, NTS, -] We... dont't really know what this does. I belive it scales the ADSR differently.
  uint8_t note_type_select; 

  /* Operator */
  //               [channel][operator]
  uint8_t operator_reg_20[3][2]; // 0x020 base [Tremolo, Vibrato, Sustain, KSR, Frequency Multiplier]
  uint8_t operator_reg_40[3][2]; // 0x040 base [KSL, TL]
  uint8_t operator_reg_60[3][2]; // 0x060 base [AR, DR]
  uint8_t operator_reg_80[3][2]; // 0x080 base [SL, RR]
  uint8_t operator_reg_E0[3][2]; // 0x0E0 base [WS]

  /* Channel */
  // The algorithm is actually a combination of connection_select (Reg 0x104) and the channels' CNT (Reg 0xC0) bits.
  // However, we keep a copy of its number to avoid having to decode it for the UI.
  uint8_t algorithm;         // 0-7
  uint16_t multiplier[3];    // Literal frequency multiplier in Fixed Point 8.8 format
  uint8_t channel_reg_C0[3]; // Reg 0x0C0 [Stereo Switches, Feedback, Connection Type]
};

#endif
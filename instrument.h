#ifndef INSTRUMENT_H
#define INSTRUMENT_H

typedef struct Instrument {
  /* Global */
  uint8_t vibrato_tremolo_depth_flags; // Reg 0xBD [DAM, DVB, RYT, BD, SD, TOM, TC, HH] we only use the first two bits, though.
  uint8_t note_type_select; // Reg 0x008 [-, NTS, -] We... dont't really know what this does. I belive it scales the ADSR differently.

  /* Operator */
  // 0x020 base [Tremolo, Vibrato, Sustain, KSR, Frequency Multiplier]
  // 0x040 base [KSL, TL]
  // 0x060 base [AR, DR]
  // 0x080 base [SL, RR]
  // 0x0E0 base [WS]
  uint8_t operator_registers[30];

  /* Channel */
  // The algorithm is actually a combination of connection_select (Reg 0x104) and the channels' CNT (Reg 0xC0) bits.
  // However, we keep a copy of its number to avoid having to decode it for the UI.
  uint8_t algorithm;         // 0-7
  uint16_t multiplier[3];    // Literal frequency multiplier in Fixed Point 8.8 format
  uint8_t channel_reg_C0[3]; // Reg 0x0C0 [Stereo Switches, Feedback, Connection Type]
  uint8_t connection_select; // Reg 0x104
};

#endif
#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <cstdint>

typedef struct Operator {
  uint8_t reg_20; // 0x020 base [Tremolo, Vibrato, Sustain, KSR, Frequency Multiplier]
  uint8_t reg_40; // 0x040 base [KSL, TL]
  uint8_t reg_60; // 0x060 base [AR, DR]
  uint8_t reg_80; // 0x080 base [SL, RR]
  uint8_t reg_E0; // 0x0E0 base [WS]
} Operator;

typedef struct Channel {
  Operator op[2];
  uint8_t reg_C0;       // Stereo Switches, OP1 Feedback and Connection Type (FM/Additive)
  uint16_t multiplier; // Fractional Frequency Multiplier
} Channel;

typedef struct Instrument {
  Channel chan[3];

  /* Global */
    // The algorithm is actually a combination of connection_select (Reg 0x104) and the channels' CNT (Reg 0xC0) bits.
    // However, we keep a copy of its number to avoid having to decode it for the UI.
    // It is also used by voices to construct register 0x104 on the fly
    uint8_t algorithm; // 0-7

    // Voice volume is controlled by the last operators in the algorithm chain. These flags have information about which operators are last in the chain.
    // each byte in this table is a set of six flags
    // Flags are set up by setAlgorithm()
    uint8_t flags_is_last_in_chain; // [  *   *   CH3OP2 CH2OP2 CH1OP2 CH3OP1 CH2OP1 CH1OP1 ]

    // Reg 0xBD [DAM, DVB, RYT, BD, SD, TOM, TC, HH] we only use the first two bits, though.
    uint8_t vibrato_tremolo_depth_flags;   
  
    // Reg 0x008 [-, NTS, -] I... dont't really know what this does. I belive it scales the ADSR differently.
    uint8_t note_type_select; 
} Instrument;

#endif

#ifndef INSTRUMENT_H
#define INSTRUMENT_H

typedef struct Instrument {
  // OPL3-specific memory:
  uint8_t operator_registers[30]; // operator > register
  uint8_t channel_registers[9];   // channel  > register
  uint8_t connection_select;
  uint8_t note_type_select;
  uint8_t vibrato_tremolo_depth_flags;

  // Other parameters that make up the "instrument":
  uint8_t algorithm;
  uint16_t multiplier[3]; // per-channel pitch multiplier
};

#endif
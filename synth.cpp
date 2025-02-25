#include "synth.h"

/*  This represents the properties of an OPL3 operator  */
struct Operator {
  uint8_t volume;
  uint8_t attack, decay, sustain, release;
  uint8_t multiplier;
  uint8_t waveform;
  bool has_vibrato;
  bool has_tremolo;
}

/*
  To _me_, in the context of this project, a channel is essentially a 2OP OPL3 voice.
  Note that an OPL3 voice is NOT the same as the Synth's voices (more on that below)
*/
struct Channel {
  Operator operator[2];
  uint8_t volume;
  uint8_t feedback;
  int detune;
}

/*
  This represents a Synth voice, which is an abstracted collection of three OPL3 voices which can be combined in 8 different abstracted algorithms (see synth.h)
  Efectively, the Synth only has 6 voices in total, but with 6OP each which should be _plenty_ for a keyboard synth while simplifying the user experience;
  People (and certainly _I_) are not computers! lol
*/
struct Voice
{
  Channel channel[3];
  int frequency;
  int octave;

  // [ * * L2 R2 L1 R1 L0 R0 ]
  uint8_t channel_switches;
  uint8_t algorithm; 
  bool is_playing;
}


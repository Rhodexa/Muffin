#ifndef OPL_H_
#define OPL_H_

/*
  There's 236 register we actually care about.
  In theory, we could essentially update the entirety of the register set
  at about 800Hz...this is good news, because it means at the very least
  we have plenty of time to update the buffers

  The 236 registers are:
    18 channels running on 3 registers each (+54)
    36 operators running on 5 registers each (+180)
    0x104 (Connection Select)
    0x0BD (Tremolo and Vibrato depth) — this also includes rhythms but... yeah, we don't use that  

  // two-digit register names are mirrored in the second half of the chip at A1 = 1
*/

uint8_t REG_0BD = 0; // Vibrato and Tremolo depth
uint8_t REG_104 = 0; // Connection Select

struct Operator {
  uint8_t
    REG_20, // AM | VIB | KSR | MULT
    REG_40, // KSL     | Amplitude
    REG_60, // Attack  | Decay
    REG_80, // Sustain | Release
    REG_E0  // Waveform Select
  ;
  uint8_t volume;
  uint8_t attack, decay, sustain, release;
  uint8_t multiplier;
  uint8_t waveform;
  bool has_vibrato;
  bool has_tremolo;
};

struct Channel {
  uint8_t
    REG_A0,
    REG_B0,
    REG_C0
  ;
  Operator operator[2]; 
  uint8_t volume;
  uint8_t feedback;
  int detune; 
};

struct Voice
{
  Channel channel[3];
  int frequency;
  int octave;
  uint8_t algorithm; 
  bool is_playing;
  uint8_t stereo_switches; // [ * * L2 R2 L1 R1 L0 R0 ]
};

class OPL {
public:
  uint8_t offset;
  void updateOffset(uint8_t voice){
    if (offset >= 3) offset = voice - 3 + 0x100;
    else offset = voice;
  }
  
  // see opl.cpp for algorithms
  void setAlgorithm(uint8_t voice, uint8_t algorithm);
  void updateVoice(uint8_t voice);
  //void updateOperator();
};

#endif
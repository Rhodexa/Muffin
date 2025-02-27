#ifndef OPL_H_
#define OPL_H_

/*
  There's 236 register we actually care about.
  In theory we could essentially update the entirety of the register set
  at about 800Hz...this is good news, because it means at the very least
  we have plenty of time to update the buffers

  The 236 registers are:
    18 channels running on 3 registers each (+54)
    36 operators running on 5 registers each (+180)
    0x104 (Connection Select)
    0x0BD (Tremolo and Vibrato depth) — this also includes rhythms but... yeah, we don't use that    
*/

struct Operator {
  uint8_t
    REG_20,
    REG_40,
    REG_60,
    REG_80,
    REG_E0
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







uint8_t REG_0BD = 0;
uint8_t REG_108 = 0; // Connection Select

class OPL {
  void update_ChannelBase(uint8_t base, uint8_t voice_index){
    addr_t effective_index = get_voiceChannelOffset(voice_index);
    hw_write(base, 0x00100000);
    hw_write(base + 8, 0x00100000);
    hw_write(base + 16, 0x00100000);
  }


  //   | 2OP FM | 2OP ADD | 3OP FM | 4OP FM | 1OP ADD |
  //   |--------|---------|--------|--------|---------|
  // 0 |      3 |         |        |        |         |
  // 1 |      2 |       1 |        |        |         | These use the 2OP mode
  // 2 |      1 |       2 |        |        |         |
  // 3 |        |       3 |        |        |         |
  //   |--------|---------|--------|--------|---------| 
  // 4 |      1 |         |        |      1 |         |
  // 5 |        |       1 |        |      1 |         | These use the 4OP mode
  // 6 |      1 |         |      1 |        |       1 | 
  // 7 |        |       1 |      1 |        |       1 |
  //   |--------|---------|--------|--------|---------|

  void setAlgorithm(uint8_t voice, uint8_t algorithm) {
    switch(algorithm){
      case 0: // 3xFM
        REG_108 &= (0b11111110 << voice);
        channel[voice    ].REG_C0 &= 0b11111110;
        channel[voice + 3].REG_C0 &= 0b11111110;
        channel[voice + 6].REG_C0 &= 0b11111110;
        break;

      case 1: // 2xFM + 1xADD
        REG_108 &= (0b11111110 << voice);
        channel[voice    ].REG_C0 |= 0b00000001;
        channel[voice + 3].REG_C0 &= 0b11111110;
        channel[voice + 6].REG_C0 &= 0b11111110;
        break;

      case 2: // 1xFM + 2xADD
        REG_108 &= (0b11111110 << voice);
        channel[voice    ].REG_C0 |= 0b00000001;
        channel[voice + 3].REG_C0 |= 0b00000001;
        channel[voice + 6].REG_C0 &= 0b11111110;
        break;

      case 3: // 3xADD
        REG_108 &= (0b11111110 << voice);
        channel[voice    ].REG_C0 |= 0b00000001;
        channel[voice + 3].REG_C0 |= 0b00000001;
        channel[voice + 6].REG_C0 |= 0b00000001;
        break;

      case 4: // 1x4OPFM + 1x2OPFM
        REG_108 |= (0b00000001 << voice);
        channel[voice    ].REG_C0 &= 0b11111110;
        channel[voice + 3].REG_C0 &= 0b11111110;
        channel[voice + 6].REG_C0 &= 0b11111110;
        break;
      
      case 5: // 1x4OPFM + 1x2OPADD 
        REG_108 |= (0b00000001 << voice);
        channel[voice    ].REG_C0 &= 0b11111110;
        channel[voice + 3].REG_C0 &= 0b11111110;
        channel[voice + 6].REG_C0 |= 0b00000001;
        break;

      case 6: // 1x3OPFM + 1xOP + 1x2OPFM
        REG_108 |= (0b00000001 << voice);
        channel[voice    ].REG_C0 |= 0b00000001;
        channel[voice + 3].REG_C0 &= 0b11111110;
        channel[voice + 6].REG_C0 &= 0b11111110;
        break;
      
      case 7: // 1x3OPFM + 1xOP + 1x2OPADD
        REG_108 |= (0b00000001 << voice);
        channel[voice    ].REG_C0 |= 0b00000001;
        channel[voice + 3].REG_C0 &= 0b11111110;
        channel[voice + 6].REG_C0 |= 0b00000001;
        break;
    }

    uint16_t offset = voice;
    if (offset >= 3) offset = offset - 3 + 0x100;
    opl_hw_setRegister(0xC0 + offset, channel[voice].REG_C0);
    opl_hw_setRegister(0xC3 + offset, channel[voice].REG_C0);
    opl_hw_setRegister(0xC6 + offset, channel[voice].REG_C0);
    opl_hw_setRegister(0x104, REG_108);
  }
};

#endif
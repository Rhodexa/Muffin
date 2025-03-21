#include "instrument_handler.h"

InstrumentHandler::InstrumentHandler(Instrument& inst) : instrument(&inst) {
  m_operator = 0;
  m_channel = 0;
  buildDefaultInstrument();
}

void InstrumentHandler::setActiveInstrument(Instrument& instrument) {this->instrument = &instrument;}
void InstrumentHandler::setActiveOperator(uint8_t op){ m_operator = op; }
void InstrumentHandler::setActiveChannel(uint8_t ch){ m_channel = ch;}

// Operators
void InstrumentHandler::op_setUseTremolo(uint8_t value)     { Toolbits::writeField(instrument->operator_reg_20[m_channel][m_operator], 0x80, 7, value); }
void InstrumentHandler::op_setUseVibratro(uint8_t value)    { Toolbits::writeField(instrument->operator_reg_20[m_channel][m_operator], 0x40, 6, value); }
void InstrumentHandler::op_setKeyScaleRate(uint8_t value)   { Toolbits::writeField(instrument->operator_reg_20[m_channel][m_operator], 0x10, 4, value); }
void InstrumentHandler::op_setMultiplier(uint8_t value)     { Toolbits::writeField(instrument->operator_reg_20[m_channel][m_operator], 0x0F, 0, value); }
void InstrumentHandler::op_setKeyScaleLevel(uint8_t value)  { Toolbits::writeField(instrument->operator_reg_40[m_channel][m_operator], 0xC0, 6, value); }
void InstrumentHandler::op_setAmplitude(uint8_t value)      { Toolbits::writeField(instrument->operator_reg_40[m_channel][m_operator], 0x3F, 0, value); }
void InstrumentHandler::op_setAttack(uint8_t value)         { Toolbits::writeField(instrument->operator_reg_60[m_channel][m_operator], 0xF0, 4, value); }
void InstrumentHandler::op_setDecay(uint8_t value)          { Toolbits::writeField(instrument->operator_reg_60[m_channel][m_operator], 0x0F, 0, value); }
void InstrumentHandler::op_setSustain(uint8_t value)        { Toolbits::writeField(instrument->operator_reg_80[m_channel][m_operator], 0xF0, 4, value); }
void InstrumentHandler::op_setRelease(uint8_t value)        { Toolbits::writeField(instrument->operator_reg_80[m_channel][m_operator], 0x0F, 0, value); }
void InstrumentHandler::op_setWaveform(uint8_t value)       { Toolbits::writeField(instrument->operator_reg_E0[m_channel][m_operator], 0x07, 0, value); }

// Channels
void InstrumentHandler::ch_setFeedback(uint8_t value)       { Toolbits::writeField(instrument->channel_reg_C0[m_channel], 0x0E, 1, value); }
void InstrumentHandler::ch_setStereoSwitches(uint8_t value) { Toolbits::writeField(instrument->channel_reg_C0[m_channel], 0x30, 4, value); }
void InstrumentHandler::ch_setMultiplier(uint16_t value)    { instrument->multiplier[m_channel] = value; }

// Globals
void InstrumentHandler::global_setVibratoDepth(uint8_t value){
  instrument->vibrato_tremolo_depth_flags &= 0b10111111;
  instrument->vibrato_tremolo_depth_flags |= ((value & 1) << 6);
}

void InstrumentHandler::global_setTremoloDepth(uint8_t value){
  instrument->vibrato_tremolo_depth_flags &= 0b01111111;
  instrument->vibrato_tremolo_depth_flags |= ((value & 1) << 7);
}

// This table represents the combination of modes each algorithm can use. The table is indexed by the algorithm number.
/////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////
void InstrumentHandler::voice_setAlgorithm(uint8_t value) {
  instrument->algorithm = value;

  // Because the channels also have their own CNT bit to set the rest on the configuratio we do that here
  switch (value){
    /* Modes using 2OP only */
    case 0: // 3xFM
      instrument->channel_reg_C0[0] &= 0b11111110;
      instrument->channel_reg_C0[1] &= 0b11111110;
      instrument->channel_reg_C0[2] &= 0b11111110;
      instrument->flags_is_last_in_chain = 0b00111000;
      break;
    case 1: // 2xFM + 1xADD
      instrument->channel_reg_C0[0] |= 0b00000001;
      instrument->channel_reg_C0[1] &= 0b11111110;
      instrument->channel_reg_C0[2] &= 0b11111110;
      instrument->flags_is_last_in_chain = 0b00111001;
      break;
    case 2: // 1xFM + 2xADD
      instrument->channel_reg_C0[0] |= 0b00000001;
      instrument->channel_reg_C0[1] |= 0b00000001;
      instrument->channel_reg_C0[2] &= 0b11111110;
      instrument->flags_is_last_in_chain = 0b00111011;
      break;
    case 3: // 3xADD
      instrument->channel_reg_C0[0] |= 0b00000001;
      instrument->channel_reg_C0[1] |= 0b00000001;
      instrument->channel_reg_C0[2] |= 0b00000001;
      instrument->flags_is_last_in_chain = 0b00111111;
      break;

    /* Modes using 4OP */
    case 4: // 1x4OPFM + 1x2OPFM
      instrument->channel_reg_C0[0] &= 0b11111110;
      instrument->channel_reg_C0[1] &= 0b11111110;
      instrument->channel_reg_C0[2] &= 0b11111110;
      instrument->flags_is_last_in_chain = 0b00110000;
      break;
    case 5: // 1x4OPFM + 1x2OPADD 
      instrument->channel_reg_C0[0] &= 0b11111110;
      instrument->channel_reg_C0[1] &= 0b11111110;
      instrument->channel_reg_C0[2] |= 0b00000001;
      instrument->flags_is_last_in_chain = 0b00110100;
      break;
    case 6: // 1x3OPFM + 1xOP + 1x2OPFM
      instrument->channel_reg_C0[0] |= 0b00000001;
      instrument->channel_reg_C0[1] &= 0b11111110;
      instrument->channel_reg_C0[2] &= 0b11111110;
      instrument->flags_is_last_in_chain = 0b00110001;
      break;
    case 7: // 1x3OPFM + 1xOP + 1x2OPADD
      instrument->channel_reg_C0[0] |= 0b00000001;
      instrument->channel_reg_C0[1] &= 0b11111110;
      instrument->channel_reg_C0[2] |= 0b00000001;
      instrument->flags_is_last_in_chain = 0b00110101;
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default instrument is a simple Sine Wave
void InstrumentHandler::buildDefaultInstrument() {
  // Use 4OPFM mode with channels 0 and 1, channel 2 is in 2OPFM mode
  // This would allow anyone to quickly play around with a 4OPFM chain
  voice_setAlgorithm(4); 

  // First, we need to choose which of all 3 channels on this voice to edit.
  // We wanna hear a simple sine by default, this means editing OP4, that is Chan 1 Op 1
  setActiveChannel(1);  // OP3 and OP4
    // Now let's edit this voice's operators
    setActiveOperator(1); // By default, channels of FM mode, so to hear something we need to edit its carrier (Operator 2)
      op_setAmplitude(0);      // Max. amplitude
      op_setAttack(15);        // -
      op_setDecay(6);          // - Set instantaneous reaction (Max attack and release)
      op_setSustain(8);        // -
      op_setRelease(15);       // -
      op_setWaveform(0);       // Sinewave (default anyway)
      // that's it. CHAN1 -> OP1 (OP4) is set for sine wave.
    
  // Now onto the channel-specific configurations
  ch_setStereoSwitches(0b11); // Enable sound ouput on Left and Right
  ch_setMultiplier(1 << 12);  // No frequency scaling
  // Done. That's all we need for a simple sine wave! (I hope)
}

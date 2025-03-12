#include "instrument_handler.h"

InstrumentHandler::InstrumentHandler(Instrument& instrument) : m_inst(&instrument) {
  m_operator = 0;
  m_channel = 0;
  buildDefaultInstrument();
}

void InstrumentHandler::setActiveInstrument(Instrument& instrument) {m_inst = &instrument;}
void InstrumentHandler::setActiveOperator(uint8_t op){ m_operator = op; }
void InstrumentHandler::setActiveChannel(uint8_t ch){ m_channel = ch;}

// Operators
void InstrumentHandler::op_setUseTremolo(uint8_t value)     { Toolbits::writeField(m_inst->operator_reg_20[m_channel][m_operator], 0x80, 7, value); }
void InstrumentHandler::op_setUseVibratro(uint8_t value)    { Toolbits::writeField(m_inst->operator_reg_20[m_channel][m_operator], 0x40, 6, value); }
void InstrumentHandler::op_setKeyScaleRate(uint8_t value)   { Toolbits::writeField(m_inst->operator_reg_20[m_channel][m_operator], 0x10, 4, value); }
void InstrumentHandler::op_setMultiplier(uint8_t value)     { Toolbits::writeField(m_inst->operator_reg_20[m_channel][m_operator], 0x0F, 0, value); }
void InstrumentHandler::op_setKeyScaleLevel(uint8_t value)  { Toolbits::writeField(m_inst->operator_reg_40[m_channel][m_operator], 0xC0, 6, value); }
void InstrumentHandler::op_setAmplitude(uint8_t value)      { Toolbits::writeField(m_inst->operator_reg_40[m_channel][m_operator], 0x3F, 0, value); }
void InstrumentHandler::op_setAttack(uint8_t value)         { Toolbits::writeField(m_inst->operator_reg_60[m_channel][m_operator], 0xF0, 4, value); }
void InstrumentHandler::op_setDecay(uint8_t value)          { Toolbits::writeField(m_inst->operator_reg_60[m_channel][m_operator], 0x0F, 0, value); }
void InstrumentHandler::op_setSustain(uint8_t value)        { Toolbits::writeField(m_inst->operator_reg_80[m_channel][m_operator], 0xF0, 4, value); }
void InstrumentHandler::op_setRelease(uint8_t value)        { Toolbits::writeField(m_inst->operator_reg_80[m_channel][m_operator], 0x0F, 0, value); }
void InstrumentHandler::op_setWaveform(uint8_t value)       { Toolbits::writeField(m_inst->operator_reg_E0[m_channel][m_operator], 0x07, 0, value); }

// Channels
void InstrumentHandler::ch_setFeedback(uint8_t value)       { Toolbits::writeField(m_inst->channel_reg_C0[m_channel], 0x0E, 1, value); }
void InstrumentHandler::ch_setStereoSwitches(uint8_t value) { Toolbits::writeField(m_inst->channel_reg_C0[m_channel], 0x30, 4, value); }
void InstrumentHandler::ch_setMultiplier(uint16_t value)    { m_inst->multiplier[m_channel] = value; }

// Globals
void InstrumentHandler::setVibratoDepth(uint8_t value){
  m_inst->vibrato_tremolo_depth_flags &= 0b10111111;
  m_inst->vibrato_tremolo_depth_flags |= ((value & 1) << 6);
}

void InstrumentHandler::setTremoloDepth(uint8_t value){
  m_inst->vibrato_tremolo_depth_flags &= 0b01111111;
  m_inst->vibrato_tremolo_depth_flags |= ((value & 1) << 7);
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
void InstrumentHandler::setAlgorithm(uint8_t value) {
  m_inst->algorithm = value;
  
  // Set whether the channel uses OP2 or OP4 mode. Only bit 0 is used. Voice class handles the logic of building the actual state of 0x104 register before loading.
  m_inst->connection_select = value < 4 ? 0 : 1;

  // Because the channels also have their own CNT bit to set the rest on the configuratio we do that here
  switch (value){
    /* Modes using 2OP only */
    case 0: // 3xFM
      m_inst->channel_reg_C0[0] &= 0b11111110;
      m_inst->channel_reg_C0[1] &= 0b11111110;
      m_inst->channel_reg_C0[2] &= 0b11111110;
      break;
    case 1: // 2xFM + 1xADD
      m_inst->channel_reg_C0[0] |= 0b00000001;
      m_inst->channel_reg_C0[1] &= 0b11111110;
      m_inst->channel_reg_C0[2] &= 0b11111110;
      break;
    case 2: // 1xFM + 2xADD
      m_inst->channel_reg_C0[0] |= 0b00000001;
      m_inst->channel_reg_C0[1] |= 0b00000001;
      m_inst->channel_reg_C0[2] &= 0b11111110;
      break;
    case 3: // 3xADD
      m_inst->channel_reg_C0[0] |= 0b00000001;
      m_inst->channel_reg_C0[1] |= 0b00000001;
      m_inst->channel_reg_C0[2] |= 0b00000001;
      break;

    /* Modes using 4OP */
    case 4: // 1x4OPFM + 1x2OPFM
      m_inst->channel_reg_C0[0] &= 0b11111110;
      m_inst->channel_reg_C0[1] &= 0b11111110;
      m_inst->channel_reg_C0[2] &= 0b11111110;
      break;
    case 5: // 1x4OPFM + 1x2OPADD 
      m_inst->channel_reg_C0[0] &= 0b11111110;
      m_inst->channel_reg_C0[1] &= 0b11111110;
      m_inst->channel_reg_C0[2] |= 0b00000001;
      break;
    case 6: // 1x3OPFM + 1xOP + 1x2OPFM
      m_inst->channel_reg_C0[0] |= 0b00000001;
      m_inst->channel_reg_C0[1] &= 0b11111110;
      m_inst->channel_reg_C0[2] &= 0b11111110;
      break;
    case 7: // 1x3OPFM + 1xOP + 1x2OPADD
      m_inst->channel_reg_C0[0] |= 0b00000001;
      m_inst->channel_reg_C0[1] &= 0b11111110;
      m_inst->channel_reg_C0[2] |= 0b00000001;
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default instrument is a simple Sine Wave
void InstrumentHandler::buildDefaultInstrument() {
  // First, we need to choose which of all 3 channels on this voice to edit.
  setActiveChannel(0);  // On channel 0 (No 4OP available)

    // Now let's edit this voice's operators
    setActiveOperator(1); // By default, channels of FM mode, so to hear something we need to edit its carrier (Operator 2)
      op_setAmplitude(0);      // Max. amplitude
      op_setAttack(15);        // -
      op_setDecay(6);          // - Set instantaneous reaction (Max attack and release)
      op_setSustain(8);       // -
      op_setRelease(15);       // -
      op_setWaveform(6);
      // that's it. CHAN0 -> OP1 is set for sine wave.
    
  // Now onto the channel-specific configurations
  ch_setStereoSwitches(0b11); // Enable sound ouput on Left and Right
  ch_setMultiplier(1 << 12);  // No frequency scaling
  // Done. That's all we need for a simple sine wave! (I hope)
}
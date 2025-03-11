#include "instrument_handler.h"

InstrumentHandler::InstrumentHandler(Instrument& instrument) : m_inst(&instrument) {
  m_operator = 0;
  m_channel = 0;
}

void InstrumentHandler::setInstrument(Instrument* instrument) {if(!instrument) return; m_inst = instrument;}
void InstrumentHandler::selectOperator(uint8_t op){ m_operator = op; }
void InstrumentHandler::selectChannel(uint8_t ch){ m_channel = ch;}

// Operators
void InstrumentHandler::setUseTremolo(uint8_t value)     { writeRegField(m_inst->operator_reg_20[m_operator + 0], 0x80, 7, value); }
void InstrumentHandler::setUseVibratro(uint8_t value)    { writeRegField(m_inst->operator_reg_20[m_operator + 0], 0x40, 6, value); }
void InstrumentHandler::setKeyScaleRate(uint8_t value)   { writeRegField(m_inst->operator_reg_20[m_operator + 0], 0x10, 4, value); }
void InstrumentHandler::setMultiplier(uint8_t value)     { writeRegField(m_inst->operator_reg_20[m_operator + 0], 0x0F, 0, value); }
void InstrumentHandler::setKeyScaleLevel(uint8_t value)  { writeRegField(m_inst->operator_reg_40[m_operator + 1], 0xC0, 6, value); }
void InstrumentHandler::setAmplitude(uint8_t value)      { writeRegField(m_inst->operator_reg_40[m_operator + 1], 0x3F, 0, value); }
void InstrumentHandler::setAttack(uint8_t value)         { writeRegField(m_inst->operator_reg_60[m_operator + 2], 0xF0, 4, value); }
void InstrumentHandler::setDecay(uint8_t value)          { writeRegField(m_inst->operator_reg_60[m_operator + 2], 0x0F, 0, value); }
void InstrumentHandler::setSustain(uint8_t value)        { writeRegField(m_inst->operator_reg_80[m_operator + 3], 0xF0, 4, value); }
void InstrumentHandler::setRelease(uint8_t value)        { writeRegField(m_inst->operator_reg_80[m_operator + 3], 0x0F, 0, value); }
void InstrumentHandler::setWaveform(uint8_t value)       { writeRegField(m_inst->operator_reg_E0[m_operator + 4], 0x07, 0, value); }

// Channels
void InstrumentHandler::setFeedback(uint8_t value)       { writeRegField(m_inst->m_inst->channel_reg_C0[m_channel], 0x0E, 1, value); }
void InstrumentHandler::setConnectionType(uin8_t value)  { writeRegField(m_inst->m_inst->channel_reg_C0[m_channel], 0x01, 0, value); }
void InstrumentHandler::setStereoSwitches(uint8_t value) { writeRegField(m_inst->m_inst->channel_reg_C0[m_channel], 0xC0, 6, value); }
void InstrumentHandler::setMultiplier(uint16_t value)    { multiplier[m_channel_index] = value; }

// Globals
void InstrumentHandler::setVibratoDepth(){
  m_inst->vibrato_tremolo_depth_flags &= 0b10111111;
  m_inst->vibrato_tremolo_depth_flags |= (value << 6);
}

void InstrumentHandler::setTremoloDepth(){
  m_inst->vibrato_tremolo_depth_flags &= 0b01111111;
  m_inst->vibrato_tremolo_depth_flags |= (value << 7);
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
  
  // Set whether the channel uses OP2 or OP4 mode. There's six bits, one for each channel. 1 means 4OP mode.
  if (m_channel < 4) m_inst->connection_select &= ~(1 << channel);
  else m_inst->connection_select |= (1 << channel);

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
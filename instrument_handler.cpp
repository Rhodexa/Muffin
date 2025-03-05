#include "m_inst_handler.h"

m_instHandler::m_instHandler(m_inst* m_instrument){
  m_inst = m_instrument;
  m_operator = 0;
  m_channel = 0;
}

void m_inst::setm_instReference(m_inst* m_instrument) {if(!m_instrument) return; m_inst = m_instrument;}
void m_inst::selectOperator(uint8_t op){ m_operator = op * 5; } // calculate the offset of the operator in the operator_registers array beforehand for efficiency
void m_inst::selectChannel(uint8_t ch){ m_channel = ch;}

// Operators
void m_inst::setUseTremolo(uint8_t value)     { writeRegField(m_inst->operator_registers[m_operator + 0], 0x80, 7, value); }
void m_inst::setUseVibratro(uint8_t value)    { writeRegField(m_inst->operator_registers[m_operator + 0], 0x40, 6, value); }
void m_inst::setKeyScaleRate(uint8_t value)   { writeRegField(m_inst->operator_registers[m_operator + 0], 0x10, 4, value); }
void m_inst::setMultiplier(uint8_t value)     { writeRegField(m_inst->operator_registers[m_operator + 0], 0x0F, 0, value); }
void m_inst::setKeyScaleLevel(uint8_t value)  { writeRegField(m_inst->operator_registers[m_operator + 1], 0xC0, 6, value); }
void m_inst::setAmplitude(uint8_t value)      { writeRegField(m_inst->operator_registers[m_operator + 1], 0x3F, 0, value); }
void m_inst::setAttack(uint8_t value)         { writeRegField(m_inst->operator_registers[m_operator + 2], 0xF0, 4, value); }
void m_inst::setDecay(uint8_t value)          { writeRegField(m_inst->operator_registers[m_operator + 2], 0x0F, 0, value); }
void m_inst::setSustain(uint8_t value)        { writeRegField(m_inst->operator_registers[m_operator + 3], 0xF0, 4, value); }
void m_inst::setRelease(uint8_t value)        { writeRegField(m_inst->operator_registers[m_operator + 3], 0x0F, 0, value); }
void m_inst::setWaveform(uint8_t value)       { writeRegField(m_inst->operator_registers[m_operator + 4], 0x07, 0, value); }

// Channels
void m_inst::setFeedback(uint8_t value)       { writeRegField(m_inst->m_inst->channel_reg_C0[m_channel], 0x0E, 1, value); }
void m_inst::setConnectionType(uin8_t value)  { writeRegField(m_inst->m_inst->channel_reg_C0[m_channel], 0x01, 0, value); }
void m_inst::setStereoSwitches(uint8_t value) { writeRegField(m_inst->m_inst->channel_reg_C0[m_channel], 0xC0, 6, value); }
void m_inst::setMultiplier(uint16_t value)    { multiplier[m_channel_index] = value; }

// m_inst Globals
void m_inst::setVibratoDepth(){
  m_inst->vibrato_tremolo_depth_flags &= 0b10111111;
  m_inst->vibrato_tremolo_depth_flags |= (value << 6);
}

void m_inst::setTremoloDepth(){
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
void m_inst::setAlgorithm(uint8_t value) {
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
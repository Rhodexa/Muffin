#include "instrument_handler.h"

InstrumentHandler::InstrumentHandler(Instrument &inst) : instrument(&inst) {
  m_operator = 0;
  m_channel = 0;
  buildDefaultInstrument();
}

void InstrumentHandler::setActiveInstrument(Instrument& inst) {this->instrument = &inst;}
void InstrumentHandler::setActiveOperator(uint8_t op){ m_operator = op; }
void InstrumentHandler::setActiveChannel(uint8_t ch){ m_channel = ch;}

// Operators
void InstrumentHandler::op_setUseTremolo(uint8_t value)     { writeField(instrument->chan[m_channel].op[m_operator].reg_20, 0b10000000, 7, value); }
void InstrumentHandler::op_setUseVibratro(uint8_t value)    { writeField(instrument->chan[m_channel].op[m_operator].reg_20, 0b01000000, 6, value); }
void InstrumentHandler::op_setKeyScaleRate(uint8_t value)   { writeField(instrument->chan[m_channel].op[m_operator].reg_20, 0b00010000, 4, value); }
void InstrumentHandler::op_setMultiplier(uint8_t value)     { writeField(instrument->chan[m_channel].op[m_operator].reg_20, 0b00001111, 0, value); }
void InstrumentHandler::op_setKeyScaleLevel(uint8_t value)  { writeField(instrument->chan[m_channel].op[m_operator].reg_40, 0b11000000, 6, value); }
void InstrumentHandler::op_setAmplitude(uint8_t value)      { writeField(instrument->chan[m_channel].op[m_operator].reg_40, 0b00111111, 0, value); }
void InstrumentHandler::op_setAttack(uint8_t value)         { writeField(instrument->chan[m_channel].op[m_operator].reg_60, 0b11110000, 4, value); }
void InstrumentHandler::op_setDecay(uint8_t value)          { writeField(instrument->chan[m_channel].op[m_operator].reg_60, 0b00001111, 0, value); }
void InstrumentHandler::op_setSustain(uint8_t value)        { writeField(instrument->chan[m_channel].op[m_operator].reg_80, 0b11110000, 4, value); }
void InstrumentHandler::op_setRelease(uint8_t value)        { writeField(instrument->chan[m_channel].op[m_operator].reg_80, 0b00001111, 0, value); }
void InstrumentHandler::op_setWaveform(uint8_t value)       { writeField(instrument->chan[m_channel].op[m_operator].reg_E0, 0b00000111, 0, value); }

// Channels
void InstrumentHandler::ch_setFeedback(uint8_t value)       { writeField(instrument->chan[m_channel].reg_C0, 0b00001110, 1, value); }
void InstrumentHandler::ch_setStereoSwitches(uint8_t value) { writeField(instrument->chan[m_channel].reg_C0, 0b00110000, 4, value); }
void InstrumentHandler::ch_setDetune(uint16_t value)    { instrument->chan[m_channel].multiplier = value; }

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
  // Because the channels also have their own CNT bit to set the rest on the configuration we do that here
  switch (value){
    /* Modes using 2OP only */
    case 0: // 3xFM
      instrument->chan[0].reg_C0 &= 0b11111110;
      instrument->chan[1].reg_C0 &= 0b11111110;
      instrument->chan[2].reg_C0 &= 0b11111110;
      instrument->flags_is_last_in_chain = 0b00101010;
      break;
    case 1: // 2xFM + 1xADD
      instrument->chan[0].reg_C0 |= 0b00000001;
      instrument->chan[1].reg_C0 &= 0b11111110;
      instrument->chan[2].reg_C0 &= 0b11111110;
      instrument->flags_is_last_in_chain = 0b00101011;
      break;
    case 2: // 1xFM + 2xADD
      instrument->chan[0].reg_C0 |= 0b00000001;
      instrument->chan[1].reg_C0 |= 0b00000001;
      instrument->chan[2].reg_C0 &= 0b11111110;
      instrument->flags_is_last_in_chain = 0b00101111;
      break;
    case 3: // 3xADD
      instrument->chan[0].reg_C0 |= 0b00000001;
      instrument->chan[1].reg_C0 |= 0b00000001;
      instrument->chan[2].reg_C0 |= 0b00000001;
      instrument->flags_is_last_in_chain = 0b00111111;
      break;

    /* Modes using 4OP */
    case 4: // 1x4OPFM + 1x2OPFM
      instrument->chan[0].reg_C0 &= 0b11111110;
      instrument->chan[1].reg_C0 &= 0b11111110;
      instrument->chan[2].reg_C0 &= 0b11111110;
      instrument->flags_is_last_in_chain = 0b00101000;
      break;
    case 5: // 1x4OPFM + 1x2OPADD 
      instrument->chan[0].reg_C0 &= 0b11111110;
      instrument->chan[1].reg_C0 &= 0b11111110;
      instrument->chan[2].reg_C0 |= 0b00000001;
      instrument->flags_is_last_in_chain = 0b00111000;
      break;
    case 6: // 1x3OPFM + 1xOP + 1x2OPFM
      instrument->chan[0].reg_C0 |= 0b00000001;
      instrument->chan[1].reg_C0 &= 0b11111110;
      instrument->chan[2].reg_C0 &= 0b11111110;
      instrument->flags_is_last_in_chain = 0b00101001;
      break;
    case 7: // 1x3OPFM + 1xOP + 1x2OPADD
      instrument->chan[0].reg_C0 |= 0b00000001;
      instrument->chan[1].reg_C0 &= 0b11111110;
      instrument->chan[2].reg_C0 |= 0b00000001;
      instrument->flags_is_last_in_chain = 0b00111001;
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Load an instrument array (such as from a file on a flash or eeprom) into an Instrument object
void InstrumentHandler::load(uint8_t* file)
{
  uint16_t cursor = 0;
  // load channels and operators
  for(int ch = 0; ch < 3; ch++)
  {
    for(int op = 0; op < 2; op++)
    {
      instrument->chan[ch].op[op].reg_20 = file[cursor++];
    }
  }
  instrument->algorithm = file[cursor++];
  instrument->vibrato_tremolo_depth_flags = file[cursor++];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InstrumentHandler::buildDefaultInstrument() {
  voice_setAlgorithm(0);
  setActiveChannel(0);
    ch_setStereoSwitches(0b11);
    ch_setDetune(1 << 12);
      setActiveOperator(0);
        op_setAmplitude(0);
        op_setAttack(15);
        op_setDecay(0);
        op_setSustain(0);
        op_setRelease(15);
        op_setWaveform(0);
}

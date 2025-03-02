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
void OPL::setAlgorithm(uint8_t voice, uint8_t algorithm) {
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


  updateOffset();
  opl_hw_setRegister(0xC0 + offset, channel[voice].REG_C0);
  opl_hw_setRegister(0xC3 + offset, channel[voice].REG_C0);
  opl_hw_setRegister(0xC6 + offset, channel[voice].REG_C0);
  opl_hw_setRegister(0x104, REG_108);
}
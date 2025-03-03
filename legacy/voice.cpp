#include "voice.h"

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
void Voice::setAlgorithm(uint8_t algorithm) {
  switch(algorithm){
    case 0: // 3xFM
      chan[0].REG_C0 &= 0b11111110;
      chan[1].REG_C0 &= 0b11111110;
      chan[2].REG_C0 &= 0b11111110;
      break;

    case 1: // 2xFM + 1xADD
      chan[0].REG_C0 |= 0b00000001;
      chan[1].REG_C0 &= 0b11111110;
      chan[2].REG_C0 &= 0b11111110;
      break;

    case 2: // 1xFM + 2xADD
      chan[0].REG_C0 |= 0b00000001;
      chan[1].REG_C0 |= 0b00000001;
      chan[2].REG_C0 &= 0b11111110;
      break;

    case 3: // 3xADD
      chan[0].REG_C0 |= 0b00000001;
      chan[1].REG_C0 |= 0b00000001;
      chan[2].REG_C0 |= 0b00000001;
      break;

    case 4: // 1x4OPFM + 1x2OPFM
      chan[0].REG_C0 &= 0b11111110;
      chan[1].REG_C0 &= 0b11111110;
      chan[2].REG_C0 &= 0b11111110;
      break;
    
    case 5: // 1x4OPFM + 1x2OPADD 
      chan[0].REG_C0 &= 0b11111110;
      chan[1].REG_C0 &= 0b11111110;
      chan[2].REG_C0 |= 0b00000001;
      break;

    case 6: // 1x3OPFM + 1xOP + 1x2OPFM
      chan[0].REG_C0 |= 0b00000001;
      chan[1].REG_C0 &= 0b11111110;
      chan[2].REG_C0 &= 0b11111110;
      break;
    
    case 7: // 1x3OPFM + 1xOP + 1x2OPADD
      chan[0].REG_C0 |= 0b00000001;
      chan[1].REG_C0 &= 0b11111110;
      chan[2].REG_C0 |= 0b00000001;
      break;
  }
}
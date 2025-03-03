#include "channel.cpp"

void Channel::setFrequency(uint16_t fnumber, uint8_t octave){
  REG_A0 = fnumber & 0xFF;
  REG_B0 &= 0xE3;
  REG_B0 |= (octave << 2) & 0x1C;
}

void Channel::setStereoSwitch(uint8_t left, uint8_t right){
  REG_C0 &= 3F;
  REG_C0 |= ((left & 1)  << 7);
  REG_C0 |= ((right & 1) << 6);
}
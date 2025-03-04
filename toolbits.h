#ifndef TOOLBITS_H
#define TOOLBITS_H

namespace Toolbits(){
  uint8_t readField(uint8_t& reg, uint8_t mask, uint8_t shift)
  {
    return (reg & mask) >> shift;
  }

  void writeField(uint8_t& reg, uint8_t mask, uint8_t shift, uint8_t data)
  {
    reg &= (~(mask)) | (data << shift) & mask;
  }
}

#endif;
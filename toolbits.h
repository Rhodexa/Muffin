#ifndef TOOLBITS_H
#define TOOLBITS_H

#include <cstdint>

namespace Toolbits {
  inline uint8_t readField(uint8_t& reg, uint8_t mask, uint8_t shift)
  {
    return (reg & mask) >> shift;
  }

  inline void writeField(uint8_t& reg, uint8_t mask, uint8_t shift, uint8_t data)
  {
    reg &= (~(mask)) | (data << shift) & mask;
  }
}

#endif;
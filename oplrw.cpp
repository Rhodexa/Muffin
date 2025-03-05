#ifndef OPL_HW_H
#define OPL_HW_H

/*
  All there's here is the interface code to blast data to the YMF262.
*/
#include <cstdint>
#include "IObus.h"
namespace OPLRW {
  void write(uint16_t address, uint8_t data){    
    IO::setChipSelect(OPL);
        
    if ( address > 0xFF ) IO::setAddress(0b10);
    else IO::setAddress(0b00);
    IO::setData(address & 0xFF);
    IO::strobeWrite();
    delayMicroseconds(3);

    IO::setAddress(0b01);
    IO::setData(data);
    IO::strobeWrite();
    //delayMicroseconds(32);
  }
}

#endif
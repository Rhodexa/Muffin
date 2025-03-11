#ifndef OPL_HW_H
#define OPL_HW_H

/*
  All there's here is the interface code to blast data to the YMF262.
  This is not a driver, we only handle write timings here. Actual sound driving is in another castle!
*/

#include <cstdint>
#include "iobus.h"

namespace OPL {
  void write(uint16_t address, uint8_t data){
    IO::setChipSelect(IO::Chips::OPL);

    if ( address > 0xFF ) IO::setAddress(0b10);
    else IO::setAddress(0b00);
    IO::setData(address);
    IO::setMode(1);
    delayMicroseconds(1);
    IO::strobeWrite();

    delayMicroseconds(3);

    IO::setAddress(0b01);
    IO::setData(data);
    IO::setMode(1);
    delayMicroseconds(1);
    IO::strobeWrite();

    IO::setChipSelect(3);
  }

  uint8_t read(){
    IO::setMode(0);
    IO::setChipSelect(IO::Chips::OPL);
    IO::setAddress(0);
    IO::beginRead();
    uint8_t data = IO::getData();
    IO::endRead();
    IO::setChipSelect(3);
    return data;
  }
}


#endif
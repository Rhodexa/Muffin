#ifndef CODEC_H
#define CODEC_H

// CS4231A CODEC by Crystal

/*
  All there's here is the interface code to blast data to the CS4231A.
  This is not a driver, we only handle write timings here. Actual sound driving is in another castle!
*/

#include <cstdint>
#include "iobus.h"

namespace Codec {
  void write(uint8_t address, uint8_t data){    
    IO::setChipSelect(IO::Chips::CODEC);

    IO::setAddress(0);
    IO::setData(address);
    IO::setMode(1);
    IO::strobeWrite();

    delayMicroseconds(3);

    IO::setAddress(1);
    IO::setData(data);
    IO::setMode(1);
    IO::strobeWrite();

    IO::setChipSelect(3);
  }

  uint8_t read(uint8_t address){
    IO::setChipSelect(IO::Chips::CODEC);

    IO::setAddress(0);
    IO::setData(address);
    IO::setMode(1);
    IO::strobeWrite();

    delayMicroseconds(3);
    IO::setMode(0);

    IO::setAddress(1);
    IO::beginRead();
    uint8_t data = IO::getData();
    IO::endRead();

    IO::setChipSelect(3);
    return data;
  }
}

#endif
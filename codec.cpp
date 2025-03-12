#include "codec.h"

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

    IO::setChipSelect(IO::Chips::NONE);
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

    IO::setChipSelect(IO::Chips::NONE);
    return data;
  }
}
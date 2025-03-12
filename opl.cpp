#include "opl.h"

namespace OPL {
  void write(uint16_t address, uint8_t data){
    IO::setChipSelect(IO::Chips::OPL);

    if ( address > 0xFF ) IO::setAddress(0b10);
    else IO::setAddress(0b00);
    IO::setData(address);
    IO::setMode(1);
    IO::strobeWrite();

    delayMicroseconds(3);

    IO::setAddress(0b01);
    IO::setData(data);
    IO::setMode(1);
    IO::strobeWrite();

    IO::setChipSelect(IO::Chips::NONE);
    delayMicroseconds(1);
  }

  uint8_t read(){
    IO::setMode(0);
    IO::setChipSelect(IO::Chips::OPL);
    IO::setAddress(0);
    IO::beginRead();
    uint8_t data = IO::getData();
    IO::endRead();
    IO::setChipSelect(IO::Chips::NONE);
    return data;
  }
}
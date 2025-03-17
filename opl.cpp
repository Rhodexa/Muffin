#include "opl.h"

namespace OPL {

  // Because the OPL3 needs just a teensy bit of time to process writes, we split the address/data writes in two segments so we can use that time
  // to process the next write. Probably, only the loadToOPL() function in Voice would make use of this.
  void write_address(uint16_t address){
    IO::setChipSelect(IO::Chips::OPL);
    if ( address > 0xFF ) IO::setAddress(0b10);
    else IO::setAddress(0b00);
    IO::setData(address);
    IO::setMode(1);
    IO::strobeWrite();
    delayMicroseconds(1);
    IO::setChipSelect(IO::Chips::NONE);
  }

  void write_data(uint16_t address, uint8_t data){
    IO::setChipSelect(IO::Chips::OPL);
    IO::setAddress(0b01);
    IO::setData(data);
    IO::setMode(1);
    IO::strobeWrite();
    delayMicroseconds(1);
    IO::setChipSelect(IO::Chips::NONE);
  }

  // Write Address and Data in one go. (Blocks code for ~4us)
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
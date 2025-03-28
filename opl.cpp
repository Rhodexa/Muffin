#include "opl.h"

namespace OPL {
  // Because the OPL3 needs just a teensy bit of time to process writes, we split the address/data writes in two segments so we can use that time
  // to process the next write. Probably, only the loadToOPL() function in Voice would make use of this.

  // OPL3 has two "banks" which can be selected using bus bit A1. To select bank 1 we add $100 to the address
  // This function will then decode that as A1 = 1 and Data = $1xx & $FF
  /*void write_address(uint16_t address){
    IO::setChipSelect(IO::Chips::OPL);
    if ( address > 0xFF ) IO::setAddress(0b10);
    else IO::setAddress(0b00);
    IO::setData(address & 0xFF);
    IO::setModeOutput();
    IO::strobeWrite();
    delayMicroseconds(3); 
    IO::setChipSelect(IO::Chips::NONE);
    delayMicroseconds(3);
  }

  void write_data(uint8_t data){
    IO::setChipSelect(IO::Chips::OPL);
    IO::setAddress(0b01);
    IO::setData(data);
    IO::setModeOutput();
    IO::strobeWrite();
    delayMicroseconds(3);
    IO::setChipSelect(IO::Chips::NONE);
    delayMicroseconds(3);
  }*/


  uint16_t tempaddr = 0;
  void write_address(uint16_t address){
    tempaddr = address;
  }

  void write_data(uint8_t data){
    write(tempaddr, data);
  }

  // Write Address and Data in one go. (Blocks code for ~4us)
  void write(uint16_t address, uint8_t data){
    IO::setChipSelect(IO::Chips::OPL);

    if ( address > 0xFF ) IO::setAddress(0b10);
    else IO::setAddress(0b00);
    IO::setData(address);
    IO::setModeOutput();
    IO::strobeWrite();

    delayMicroseconds(3);

    IO::setAddress(0b01);
    IO::setData(data);
    IO::strobeWrite();

    IO::setChipSelect(IO::Chips::NONE);
    delayMicroseconds(3);
  }

  uint8_t read(){
    IO::setModeInput();
    IO::setChipSelect(IO::Chips::OPL);
    IO::setAddress(0);
    IO::beginRead();
    uint8_t data = IO::getData();
    IO::endRead();
    IO::setChipSelect(IO::Chips::NONE);
    return data;
  }
}
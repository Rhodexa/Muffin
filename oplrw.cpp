#ifndef OPL_HW_H
#define OPL_HW_H

#include <cstdint>
#include "iobus.h"

void oplrw_write(uint16_t address, uint8_t data){
  using namespace IOBUS io;
  
  io::setChipSelect(OPL);
  
  if ( address > 0xFF ) io::setAddress(0b10);
  else io::setAddress(0b00);
  io::setData(address & 0xFF);
  io::strobeWrite();
  delayMicroseconds(32);
  
  io::setAddress(0b01);
  io::setData(data);
  io::strobeWrite();
  //delayMicroseconds(32);
}

#endif
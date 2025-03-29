#include "wiring_time.h"
#include "opl.h"
#include <Arduino.h>

namespace OPL {
  // In theory you can trust this command sequence to always work. Useful for debugging hardware and software
  // They will make the OPL3 produce a single sine wave tone at F#4
  // It also ensures the Status register is 0xE0
  void test()
  {
    write(0x105, 1);
    write(0x08, 0x40);
    write(0xC0, 0xF1);
    write(0x23, 0x21);
    write(0x43, 0x00);
    write(0x63, 0xF0);
    write(0x83, 0x70);
    write(0xE3, 0x00);  
    write(0xA0, 0xE8);
    write(0xB0, 0x35);
    write(0x04, 0x02);
    delay(2000);
    //Serial.println(read()); // should return 0xE0
  }

  // Write Address and Data in one go. (Blocks code for ~4.6us)
  void write(uint16_t address, uint8_t data)
  {
    IO::setChipSelect(IO::Chips::OPL);
    if ( address & 0x100 ) IO::setAddress(0b10);
    else IO::setAddress(0b00);
    IO::setData(address);
    IO::setModeOutput();
    IO::strobeWrite();
    delayMicroseconds(1);
    IO::setAddress(0b01);
    IO::setData(data);
    IO::strobeWrite();
    IO::setChipSelect(IO::Chips::NONE);
    delayMicroseconds(1);
  }

  uint8_t read()
  {
    IO::setModeInput();
    IO::setAddress(0);
    IO::setChipSelect(IO::Chips::OPL);
    IO::beginRead();
    uint8_t data = IO::getData();
    //IO::endRead();
    IO::setChipSelect(IO::Chips::NONE);
    return data;
  }
}
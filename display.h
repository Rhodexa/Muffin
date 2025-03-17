#include "wiring_time.h"
#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>
#include <cstdint>
#include <SPI.h>
#include "iobus.h"

#include "bitmaps.h"
// I wanted to use a different display, but i couldn't hack it... so i picked up another one that's sadly smaller...
// but at least I managed to figure out it uses an ST7565-style driver.

namespace Display{

  uint16_t spi_speed = 1125000;

  /*void write(uint8_t data, bool is_command = true){
    IO::setChipSelect(IO::Chips::GLCD);
    delayMicroseconds(1);
    IO::setAddress(is_command ? 0 : 1);
    SPI.transfer(data);
    delayMicroseconds(1);
    IO::setChipSelect(IO::Chips::NONE);
    delayMicroseconds(180);
  }*/

  void write(uint8_t data, bool is_command = true){
    IO::setChipSelect(IO::Chips::GLCD);
    delayMicroseconds(1);
    if(is_command) {
      SPI.transfer16(0x2000 + data);
    }
    else {
      SPI.transfer16(0x6000 + data);
    }
    delayMicroseconds(1);
    IO::setChipSelect(IO::Chips::NONE);
    delayMicroseconds(180);
  }

  void init()
  {
    SPI.beginTransaction(SPISettings(spi_speed, MSBFIRST, SPI_MODE0));
    write(0xA2); // Bias
    write(0xA1); // Bias
    write(0xC0); // Out Mode
    write(0x25); // Voltage Reg
    write(0x81); // Contrast
    write(0x17); // Contrast
    write(0x2F); // Set everything on
    write(0xAF); // Display On
    write(0xA4); // Normal
    write(0x40); // Start Line
    write(0xB0); // Start Page
    write(0x10); // Column Address
    write(0x00); // Columns Address

    for (uint8_t x = 0; x < 8; x++){
      write(0xB0 | x);
      write(0x10);
      write(0x00);
      for(uint8_t y = 0; y < 128; y++){
        write(bitmaps_logo[y + (x * 128)], false);
      }
    }
    SPI.endTransaction();
  }


}

#endif


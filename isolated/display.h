#include "wiring_time.h"
#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>
#include <cstdint>
#include <SPI.h>
#include "iobus.h"


namespace Display{

  uint16_t speed = 1125000;
  uint16_t GLCD_Init[] = {
    0x0900, 0x0900,
    0x0000, 0x0000,
    0xF000, 0xF000, 0xF30A,
    0xA000,
    0x2002, 0x60C4,
    0x2003, 0x600B,
    0x2006, 0x6023,
    0x200C, 0x6098,
    0x2000, 0x6007,
  };

  uint16_t write(uint16_t data){
    IO::setChipSelect(IO::Chips::GLCD);
    delayMicroseconds(1);
    uint16_t received = SPI.transfer16(data);
    delayMicroseconds(1);
    IO::setChipSelect(IO::Chips::NONE);
    delayMicroseconds(180);
    return received;
  }

  
  void init()
  {
    SPI.beginTransaction(SPISettings(speed, MSBFIRST, SPI_MODE0));
    write(0xF000); delayMicroseconds(150);
    write(0xF000); delayMicroseconds(26);
    write(0xF30A); delayMicroseconds(6500);
    write(0xA000); delayMicroseconds(10000);
  
    uint8_t begin = 0x00;
    uint8_t end = 0xFF;
    uint16_t t = begin;
    while(true){ 
      write(0x2000 + t);

      t++;
      if( t > end ) {
        t = begin;
        digitalWrite(PC13, !digitalRead(PC13));
      }
    }

    SPI.endTransaction();    
    delay(1000);
  }

  // 10 Toggle if 100
  void setLEDs(uint8_t leds)
  {
    SPI.beginTransaction(SPISettings(speed, MSBFIRST, SPI_MODE0));
    write(0x9000 | leds);
    SPI.endTransaction();
  }

  void sendRows(uint8_t row_data[], uint8_t count)
  {
    SPI.beginTransaction(SPISettings(speed, MSBFIRST, SPI_MODE0));

    /* Fixed sequence */
    write(0x2001);
    write(0x6003);
    write(0x2004);
    write(0x6000 | (0));
    write(0x60C0 | (0));
    write(0x2007);

    while (count)
    {
      write(0x6000 | *row_data);
      row_data++;
      count--;
    }

    SPI.endTransaction();
  }
}

#endif


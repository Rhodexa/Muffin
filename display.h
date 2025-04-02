#ifndef DISPLAY_H
#define DISPLAY_H

#include "wiring_time.h"
#include <Arduino.h>
#include <cstdint>
#include <SPI.h>
#include "iobus.h"
#include "ui_characters.h"
#include "bitmaps.h"

// I wanted to use a different display, but i couldn't hack it... so i picked up another one that's sadly smaller...
// but at least I managed to figure out it uses an ST7565-style driver.
// Thanks again, El Coco! (cocus.rf.gd)

// Display pinout:
/*
  1 /CS
  2 /RST -> A4
  3 A0   -> A0
  4 CLK  -> PA5
  5 MOSI -> PA7
  6 3v3  
  7 GND  

  */

namespace Display{
  void writeData(uint8_t data);
  void writeCmd(uint8_t data);
  void init();
  void print(String text, uint8_t x, uint8_t y, int s);
  void clearLine(uint8_t y);
  void clearDisplay();
  void stampBitmap(uint8_t *bitmap, uint8_t width, uint8_t height, uint8_t x_pos, uint8_t y_pos);

  inline void cursorX(uint8_t x)
  {
    writeCmd(0x10 | (x >> 4));
    writeCmd(0x00 | (x & 0x0F));
  }

  inline void cursorY(uint8_t y)
  {
    writeCmd(0xB0 | y);
  }
}

#endif


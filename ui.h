#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "stm32f1xx.h"
#include <Arduino.h>
#include <cstdint>
#include "display.h"
#include "ui_characters.h"


namespace UI
{
  void init();

  void print(String text);
  namespace BigDigit
  {
    inline uint8_t x_pos = 0;
    inline uint8_t y_pos = 0;

    void pipe(uint8_t pattern, uint8_t length);
    inline void setPosition(uint8_t x, uint8_t y) { y_pos = y; x_pos = x; }
    void draw(uint8_t digit, uint8_t pos);
  }
}

#endif
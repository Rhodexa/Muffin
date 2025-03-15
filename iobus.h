//#include "wiring_time.h"
#ifndef IOBUS_32_H
#define IOBUS_32_H

#include <cstdint>
#include "stm32f1xx.h"
#include <Arduino.h>

/*
  IOBUS STM32F1 version
  PB[15···8] (Use 3.3V anyway! It's safer. The STM32F103 is 5V-tolerant ONLY when FULLY POWERED)
*/

namespace IO {
  enum Chips {
    CODEC,
    OPL,
    GLCD,
    NONE
  };

  constexpr unsigned int m_MASK = 0x0000FF00;
  #define IO_PIN_RD PA11
  #define IO_PIN_WR PA12

  // yeaah...
  void shortDelay(void);

  void beginRead();
  void endRead();
  void setMode(bool mode);

  void init();

  void setData(uint8_t data);
  uint8_t getData();
  // Other stuff:
  void setAddress(uint8_t addr);
  void setChipSelect(uint8_t chip);
  void strobeWrite();
}

#endif
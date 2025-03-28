//#include "wiring_time.h"
#ifndef IOBUS_32_H
#define IOBUS_32_H

#include <cstdint>
#include "stm32f1xx.h"
#include <Arduino.h>

/*
  IOBUS STM32F1 version
  Reset Pulse:  PA4 (for now)
  Data bus:     PB[15···8] (Use 3.3V anyway! It's safer. The STM32F103 is 5V-tolerant ONLY when FULLY POWERED)
  Address bus:  PB[ 1...0] A1, A0
  Read Strobe:  PA11
  Write Strobe: PA12
  Chip Select:  PB[ 7...6] CS0, CS1 (To 74HC139 or 74HC138)
*/

namespace IO {
  enum Chips {
    CODEC,
    OPL,
    GLCD,
    NONE
  };

  constexpr unsigned int m_MASK = 0x0000FF00;
  constexpr uint8_t IO_PIN_RD = PA11;
  constexpr uint8_t IO_PIN_WR = PA12;
  constexpr uint8_t IO_PIN_RESET = PA4;
  // yeaah...
  void shortDelay(void);

  void beginRead();
  void endRead();
  void setModeOutput();
  void setModeInput();

  void init();

  void setData(uint8_t data);
  uint8_t getData();
  // Other stuff:
  void setAddress(uint8_t addr);
  void setChipSelect(uint8_t chip);
  void strobeWrite();
}

#endif
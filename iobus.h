#ifndef IOBUS_32_H
#define IOBUS_32_H
#include <cstdint>
/*
  IOBUS STM32F1 version
  PB[15···8] (Use 3.3V anyway! It's safer. The STM32F103 is 5V-tolerant ONLY when FULLY POWERED)
*/

namespace iobus {
  const unsigned int m_INPUT = 0x44444444;
  const unsigned int m_OUTPUT = 0x11111111;
  const unsigned int m_MASK = 0x0000FF00;

  void init() {
    GPIOB_CRH = m_INPUT;
  }

  void setMode(bool mode) {
    GPIOB_CRH = mode ? m_OUTPUT : m_INPUT;
  }

  void setData(uint8_t data) {
    GPIOB_ODR &= ~(m_MASK);
    GPIOB_ODR |= (data << 8) & m_MASK;
  }

  uint8_t getData() {
    return (GPIOB_IDR >> 8) & m_MASK;
  }
;

#endif
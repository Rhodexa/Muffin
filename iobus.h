#include "wiring_time.h"
#ifndef IOBUS_32_H
#define IOBUS_32_H

#include <cstdint>
#include "stm32f1xx.h"

/*
  IOBUS STM32F1 version
  PB[15···8] (Use 3.3V anyway! It's safer. The STM32F103 is 5V-tolerant ONLY when FULLY POWERED)
*/

namespace IO {
  enum Chips {
    CODEC,
    OPL,
    UNK,
    NONE
  };

  constexpr unsigned int m_MASK = 0x0000FF00;
  #define IO_PIN_RD PA11
  #define IO_PIN_WR PA12

  void beginRead(){
    digitalWrite(IO_PIN_RD, 1);
    delayMicroseconds(1);
  }

  void endRead(){
    digitalWrite(IO_PIN_RD, 0);
  }

  void setMode(bool mode) {
    if(mode){ // Out
      endRead(); // Make sure no one is talking
      GPIOB->CRH = 0x11111111;
    }
    else{ // In
      GPIOB->CRH = 0x44444444;
    }
  }

  void init() {
    // enable Port B's clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // Init the 8-bit Data bus
    setMode(0);

    // init Address ports [A0, A1] -> PB0, PB1
    // and the chip select control too [CS0, CS1] -> PB6, PB7
    uint32_t crl = GPIOB->CRL & 0x00FFFF00;
    crl |= 0x11000011;
    GPIOB->CRL = crl;

    // Init RD and WR strobes
    digitalWrite(IO_PIN_RD, HIGH);
    digitalWrite(IO_PIN_WR, HIGH);
    pinMode(IO_PIN_RD, OUTPUT);
    pinMode(IO_PIN_WR, OUTPUT);
  }  

  void setData(uint8_t data) {
    uint16_t odr = GPIOB->ODR & ~(m_MASK);
    odr |= (data << 8) & m_MASK;
    GPIOB->ODR = odr;
  }

  uint8_t getData() {
    return (GPIOB->IDR & m_MASK) >> 8;
  }

  // Other stuff:
  void setAddress(uint8_t addr){
    uint16_t odr = GPIOB->ODR & ~(0x03);
    odr |= addr & 0x03;
    GPIOB->ODR = odr;
  }

  void setChipSelect(uint8_t chip){
    endRead(); // Make sure no one is talking
    uint16_t odr = GPIOB->ODR & ~(0xC0);
    odr |= (chip << 6) & 0xC0;
    GPIOB->ODR = odr;
  }

  void strobeWrite(){
    digitalWrite(IO_PIN_WR, 1);
    delayMicroseconds(1);
    digitalWrite(IO_PIN_WR, 0);
  }
}

#endif
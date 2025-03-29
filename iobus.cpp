#include "iobus.h"

namespace IO {
  void shortDelay(void)
  {
    __asm("NOP"); __asm("NOP"); __asm("NOP"); __asm("NOP");
    __asm("NOP"); __asm("NOP"); __asm("NOP"); __asm("NOP");
    __asm("NOP"); __asm("NOP"); __asm("NOP"); __asm("NOP");
    __asm("NOP"); __asm("NOP"); __asm("NOP"); __asm("NOP");
  }

  void shorterDelay(void)
  {
    __asm("NOP"); __asm("NOP"); __asm("NOP"); __asm("NOP");
    __asm("NOP"); __asm("NOP");
  }

  void init()
  {
    pinMode(IO_PIN_RESET, OUTPUT);
    pinMode(IO_PIN_RD, OUTPUT);
    pinMode(IO_PIN_WR, OUTPUT);
    digitalWrite(IO_PIN_RD, 0);
    digitalWrite(IO_PIN_WR, 0);
    digitalWrite(IO_PIN_RESET, 1);

    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // enable Port B's clock
    setModeInput(); // Init the 8-bit Data bus

    // init Address ports [A0, A1] -> PB0, PB1
    // and the chip select control too [CS0, CS1] -> PB6, PB7
    uint32_t crl = GPIOB->CRL & 0x00FFFF00;
    crl |= 0x11000011;
    GPIOB->CRL = crl;

    // Enable devices
    delay(200);
    digitalWrite(IO_PIN_RESET, 0);
  } 

  void beginRead()
  {
    digitalWrite(IO_PIN_RD, 1);
    shorterDelay();
  }

  void endRead()
  {
    digitalWrite(IO_PIN_RD, 0);
  }

  void setModeOutput() {
    endRead(); // Make sure no one is talking
    GPIOB->CRH = 0x11111111;
    shorterDelay();
  }

  void setModeInput()
  {
    GPIOB->CRH = 0x44444444;
    shorterDelay();
  }  

  uint8_t getData()
  {
    return (GPIOB->IDR & m_MASK) >> 8;
  }

  void setData(uint8_t data)
  {
    uint16_t odr = GPIOB->ODR & ~(m_MASK);
    odr |= (data << 8) & m_MASK;
    GPIOB->ODR = odr;
  }

  // Other stuff:
  void setAddress(uint8_t addr)
  {
    uint16_t odr = GPIOB->ODR & ~(0x0003);
    odr |= addr & 0x0003;
    GPIOB->ODR = odr;
  }

  void setChipSelect(uint8_t chip)
  {
    endRead(); // Make sure no one is talking
    uint16_t odr = GPIOB->ODR & ~(0x00C0);
    odr |= (chip << 6) & 0x00C0;
    GPIOB->ODR = odr;
    shorterDelay();
  }

  void strobeWrite()
  {
    digitalWrite(IO_PIN_WR, 1);
    shortDelay();
    digitalWrite(IO_PIN_WR, 0);
  }
}
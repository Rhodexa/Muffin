#include "iobus.h"

namespace IO {
  void shortDelay(void) {
    __asm("NOP"); __asm("NOP"); __asm("NOP"); __asm("NOP");
    __asm("NOP"); __asm("NOP"); __asm("NOP"); __asm("NOP");
    __asm("NOP"); __asm("NOP"); __asm("NOP"); __asm("NOP");
    __asm("NOP"); __asm("NOP"); __asm("NOP"); __asm("NOP");
    __asm("NOP"); __asm("NOP"); __asm("NOP"); __asm("NOP");
    __asm("NOP"); __asm("NOP"); __asm("NOP"); __asm("NOP");
    __asm("NOP"); __asm("NOP"); __asm("NOP"); __asm("NOP");
    __asm("NOP"); __asm("NOP"); __asm("NOP"); __asm("NOP");
  }

  void beginRead(){
    digitalWrite(IO_PIN_RD, 1);
    shortDelay();
  }

  void endRead(){
    digitalWrite(IO_PIN_RD, 0);
  }

  void setModeOutput() {
    endRead(); // Make sure no one is talking
    GPIOB->CRH = 0x11111111;
    shortDelay();
  }

  void setModeInput(){
    GPIOB->CRH = 0x44444444;
    shortDelay();
  }

  void init() {
    pinMode(IO_PIN_RESET, OUTPUT);
    digitalWrite(IO_PIN_RESET, LOW); // Redundant... but just in case!

    // enable Port B's clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    
    // Init the 8-bit Data bus
    setModeInput();

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

    // Enable devices
    delay(100);
    digitalWrite(IO_PIN_RESET, HIGH);
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
    uint16_t odr = GPIOB->ODR & ~(0b00000011);
    odr |= addr & 0b00000011;
    GPIOB->ODR = odr;
  }

  void setChipSelect(uint8_t chip){
    endRead(); // Make sure no one is talking
    uint16_t odr = GPIOB->ODR & ~(0b11000000);
    odr |= (chip << 6) & 0x11000000;
    GPIOB->ODR = odr;
  }

  void strobeWrite(){
    digitalWrite(IO_PIN_WR, 1);
    shortDelay();
    digitalWrite(IO_PIN_WR, 0);
  }
}
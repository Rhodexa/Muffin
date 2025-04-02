#include "ui.h"

namespace UI
{
  /*
  void initQuadratureEncoder(){
    __HAL_RCC_TIM2_CLK_ENABLE();
    TIM2->CR1 = 0;
    TIM2->SMCR  = TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;
    TIM2->CCMR1 = TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;
    TIM2->CCER  = TIM_CCER_CC1E | TIM_CCER_CC2E;
    TIM2->ARR = 0xFFFF;
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
  }*/

  void init()
  {
    Display::init();
  }

  void print(String text) {
    for (int i = 0; i < text.length(); i++) {
      BigDigit::draw(text[i], i);
    }
  }

  namespace BigDigit
  {
    void draw(uint8_t character, uint8_t pos)
    {
      for(uint8_t l = 0; l < 5; l++)
      {
        uint8_t line = Fonts::font[(character)*6 + l + 1];
        for(uint8_t y = 0; y < 8; y++)
        {
          Display::cursorX(l*10 + pos*10*6);
          Display::cursorY(y);
          if (line & (1 << y))
            for(uint8_t x = 0; x < 10; x++) { Display::writeData(0xFF); } 
          else
            for(uint8_t x = 0; x < 10; x++) { Display::writeData(0x00); }
        }
      }
    }
  }
}
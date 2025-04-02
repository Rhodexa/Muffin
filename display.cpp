#include "display.h"

void Display::writeData(uint8_t data){
  IO::setChipSelect(IO::Chips::GLCD);
  IO::setAddress(1);
  SPI.transfer(data);
  IO::setChipSelect(IO::Chips::NONE);
}

void Display::writeCmd(uint8_t data){
  IO::setChipSelect(IO::Chips::GLCD);
  IO::setAddress(0);
  SPI.transfer(data);
  IO::setChipSelect(IO::Chips::NONE);
}

void Display::init()
{
  SPI.beginTransaction(SPISettings(9000000, MSBFIRST, SPI_MODE0));
  writeCmd(0xE2); // Reset
  writeCmd(0xA2); // Bias
  writeCmd(0xA1); // Bias
  writeCmd(0xC0); // Out Mode
  writeCmd(0x25); // Voltage Reg
  writeCmd(0x81); // Contrast
  writeCmd(0x17); // Contrast
  writeCmd(0x2F); // Set everything on
  writeCmd(0xAF); // Display On
  writeCmd(0xA4); // Normal
  writeCmd(0x40); // Start Line
  writeCmd(0xB0); // Start Page
  writeCmd(0x10); // Column Address
  writeCmd(0x00); // Column Address

  for (uint8_t y = 0; y < 8; y++){
    writeCmd(0xB0 | y);
    writeCmd(0x10);
    writeCmd(0x00);
    for(uint8_t x = 0; x < 128; x++){
      writeData(bitmaps_logo[x + (y * 128)]);
    }
  }
  SPI.endTransaction();
}

void Display::print(String text, uint8_t x, uint8_t y, int s) {
  if (x > 127) return;
  if (y > 7) return;
  writeCmd(0xB0 | y);
  writeCmd(0x10 | (x >> 4));
  writeCmd(0x00 | (x & 0x0F));
  for (int i = 0; i < text.length(); i++) {
    for(uint8_t line = 0; line < 6; line++)
    {
      if(s > 0) writeData(Fonts::font[text[i]*6 + line] >> s);
      else writeData(Fonts::font[text[i]*6 + line] << (s*-1));
    }
  }
}

void Display::clearLine(uint8_t y){
  if (y > 7) return;
  writeCmd(0xB0 | y);
  writeCmd(0x10);
  writeCmd(0x00);
  for (int i = 0; i < 128; i++) {
    writeData(0);
  }
}

void Display::clearDisplay(){
  for (uint8_t y = 0; y < 8; y++){
    writeCmd(0xB0 | y);
    writeCmd(0x10);
    writeCmd(0x00);
    for(uint8_t x = 0; x < 128; x++){
      writeData(0);
    }
  }
}

void Display::stampBitmap(uint8_t *bitmap, uint8_t width, uint8_t height, uint8_t x_pos, uint8_t y_pos){
  if (x_pos > 127) return;
  if (y_pos > 7) return;
  for (uint8_t y = 0; y < height; y++){
    writeCmd(0xB0 | ((y_pos + height) & 0x0F));
    writeCmd(0x10 | (x_pos >> 4));
    writeCmd(0x00 | (x_pos & 0x0F));
    for(uint8_t x = 0; x < width; x++){
      writeData(bitmap[y*width + x]);
    }
  }
}
#include <Arduino.h>
//#include "synth.h"

#include "iobus.h"
#include "opl.h"
#include "codec.h"


// use PC13 to show activity
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  IO::init();
  IO::setMode(false);

  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
}

uint8_t count = 0; 
void loop() {
  //IO::setChipselect(count++);
  //IO::strobeRead();
  OPL::write(0x105, 0x01);

  OPL::write(0x20, 0x20);
  OPL::write(0x40, 0x01);
  OPL::write(0x60, 0xF0);
  OPL::write(0x80, 0x70);
  OPL::write(0xE0, 0x05);

  OPL::write(0x23, 0x20);
  OPL::write(0x43, 0x01);
  OPL::write(0x63, 0xF0);
  OPL::write(0x83, 0x70);
  OPL::write(0xE3, 0x05);
  
  OPL::write(0xA0, 0x00);
  OPL::write(0xB0, 0x2F);
  OPL::write(0xC0, 0x30);

  Codec::write(2, 0);
  Codec::write(3, 0);

  OPL::write(0xA0, count << 4);
  OPL::write(0xE0, count);

  count++;

  uint8_t data = OPL::read();
  Serial.println(data, BIN);
  delay(500);
  //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

#include <Arduino.h>
#include <MIDI.h>
#include <cstdint>
#include "iobus.h"
#include "opl.h"
#include "codec.h"
#include "instrument.h"
#include "instrument_handler.h"
#include "voice.h"

////

struct CustomBaudRateSettings : public MIDI_NAMESPACE::DefaultSerialSettings {
  static const long BaudRate = 9600;
};

MIDI_NAMESPACE::SerialMIDI<HardwareSerial, CustomBaudRateSettings> serialMIDI(Serial);
MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::SerialMIDI<HardwareSerial, CustomBaudRateSettings>> MIDI((MIDI_NAMESPACE::SerialMIDI<HardwareSerial, CustomBaudRateSettings>&)serialMIDI);

////

Instrument default_instrument;
InstrumentHandler handler(default_instrument);

// uglyyyyy
Voice voices[6] = {
  Voice(0, default_instrument), Voice(1, default_instrument), Voice(2, default_instrument),
  Voice(3, default_instrument), Voice(4, default_instrument), Voice(5, default_instrument)
};
uint8_t current_voice = 0;
uint8_t held_key[6] = {0, 0, 0, 0, 0, 0};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr uint16_t lut_base_frq[13] = {
  488, 517, 547, 580, 614, 651,
  690, 731, 774, 820, 869, 921,
  976
};

uint32_t pitchToFrequency(uint32_t q16_pitch){
  if (q16_pitch >= (18 << 16) && q16_pitch < (114 << 16)) {
    q16_pitch -= (18 << 16);
    uint8_t int_part = (q16_pitch >> 16);
    uint16_t frc_part =  q16_pitch & 0xFFFF;
    uint16_t octave = ((uint32_t)int_part * 1366) >> 14;
    uint8_t index = int_part - octave * 12;
    uint16_t frq_a = lut_base_frq[index];
    uint16_t frq_b = lut_base_frq[index + 1];
    return ( frq_a + ((frc_part * (frq_b - frq_a)) >> 16) ) << (octave+1);
  }
  return 0;
}

void handleNoteOn(uint8_t pitch){
  // Find the next available voice
  for (uint8_t i = 0; i < 5; i++){
    current_voice++;
    if (current_voice >= 6) current_voice = 0;
    if (voices[current_voice].is_active != true) break;
  }
  held_key[current_voice] = pitch;
  uint32_t frq = pitchToFrequency(pitch << 16);
  voices[current_voice].setFrequency(frq);
  voices[current_voice].setNoteOn(0);
  voices[current_voice].loadToOPL();
  voices[current_voice].setNoteOn(true);
}

void updateOPL(){
  for (uint8_t i = 0; i < 6; i++){
    voices[i].loadToOPL();
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize IO
  IO::init();
  IO::setMode(0);

  // Prepare OPL3
  OPL::write(0x105, 1); // Enable OPL3 Features

  // Prepare CODEC
  Codec::write(Codec::Registers::OPL_LEFT,  0); // Enable left  sound output
  Codec::write(Codec::Registers::OPL_RIGHT, 0); // Enable right sound output

  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(midiHandleNoteOn);

  delay(1000); digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  MIDI.read();
  updateOPL();
  delay(5);
}

void midiHandleNoteOn(byte channel, byte note, byte velocity){
  handleNoteOn(note);
}


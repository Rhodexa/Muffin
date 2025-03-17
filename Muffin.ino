#include <Arduino.h>
#include <MIDI.h>
#include <cstdint>
#include "iobus.h"
#include "opl.h"
#include "codec.h"
#include "instrument.h"
#include "instrument_handler.h"
#include "voice.h"
#include "display.h"

// Settings

constexpr SERIAL_BAUD_RATE = 57600;


//// MIDI
struct CustomBaudRateSettings : public MIDI_NAMESPACE::DefaultSerialSettings {
  static const long BaudRate = SERIAL_BAUD_RATE;
};

MIDI_NAMESPACE::SerialMIDI<HardwareSerial, CustomBaudRateSettings> serialMIDI(Serial);
MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::SerialMIDI<HardwareSerial, CustomBaudRateSettings>> MIDI((MIDI_NAMESPACE::SerialMIDI<HardwareSerial, CustomBaudRateSettings>&)serialMIDI);
//// End MIDI

Instrument default_instrument;
InstrumentHandler handler(default_instrument);

Voice voices[6];
uint8_t current_voice = 0;
uint8_t held_key[6] = {0, 0, 0, 0, 0, 0};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void handleNoteOn(uint8_t pitch){
  // Find the next available voice
  for (uint8_t i = 0; i < 5; i++){
    current_voice++;
    if (current_voice >= 6) current_voice = 0;
    if (!voices[current_voice].isActive()) break;
  }
  held_key[current_voice] = pitch;
  uint32_t frq = pitchToFrequency(pitch << 16);
  voices[current_voice].setFrequency(frq);
  voices[current_voice].setNoteOn(true);
  handler.op_setWaveform(current_voice);
}

void updateOPL(){
  for (uint8_t i = 0; i < 6; i++){
    voices[i].loadToOPL();
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Initialize IO
  Serial.begin(SERIAL_BAUD_RATE);
  IO::init();
  IO::setMode(0);
  SPI.begin();

  // Initializing IO Bus will cause a reset pulse on the Reset line. Let's wait here for a bit 'till everything starts up 
  delay(500);

  // Init display, will show the logo
  // Allows us to already start printing information
  Display::init();

  // Prepare OPL3
  OPL::write(0x105, 1); // Enable OPL3 Features

  // Prepare CODEC
  Codec::write(Codec::Registers::OPL_LEFT,  0); // Enable left  sound output
  Codec::write(Codec::Registers::OPL_RIGHT, 0); // Enable right sound output

  // Initialize MIDI
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(midiHandleNoteOn);

  // Make sure voices have a proper pointer to an instrument
  for(int i = 0; i < 6; i++){
    voices[i].setInstrument(default_instrument);
    voices[i].m_selfIndex = i;
  }

  delay(500); digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  MIDI.read();
  updateOPL();
  delay(5); // quick and dirty way to get ~100hz
}

void midiHandleNoteOn(byte channel, byte note, byte velocity){
  handleNoteOn(note);
}


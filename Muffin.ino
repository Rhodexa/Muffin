#include <Arduino.h>
#include <cstdint>
#include <MIDI.h>
#include "iobus.h"
#include "codec.h"
#include "instrument.h"
#include "instrument_handler.h"
#include "voice_rack.h"
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

NormalWrangler wrangler();



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void midiHandleNoteOn(byte channel, byte note, byte velocity){
  wrangler.handleNoteOn(note);
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

  // Prepare CODEC
  Codec::write(Codec::Registers::OPL_LEFT,  0); // Enable left  sound output
  Codec::write(Codec::Registers::OPL_RIGHT, 0); // Enable right sound output

  // Make sure voices have a proper pointer to an instrument
  // This also initializes the OPL3 chip.
  VoiceRack::init(default_instrument);

  // Initialize MIDI
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(midiHandleNoteOn);


  delay(500); digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  MIDI.read();
  for (uint8_t i = 0; i < 6; i++){
    voices[i].loadToOPL();
  }
  delay(5); // quick and dirty way to get ~100hz
}


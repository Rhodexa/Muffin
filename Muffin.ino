#include <Arduino.h>
#include <cstdint>
#include "iobus.h"
#include "ui.h"
#include "codec.h"
#include <MIDI.h>
#include "wrangler.h"
#include "instrument.h"
#include "instrument_handler.h"
#include "voice_rack.h"

// Settings
constexpr int SERIAL_BAUD_RATE = 57600;

//// MIDI
struct CustomBaudRateSettings : public MIDI_NAMESPACE::DefaultSerialSettings {
  static const long BaudRate = 57600;
};

MIDI_NAMESPACE::SerialMIDI<HardwareSerial, CustomBaudRateSettings> serialMIDI(Serial);
MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::SerialMIDI<HardwareSerial, CustomBaudRateSettings>> MIDI((MIDI_NAMESPACE::SerialMIDI<HardwareSerial, CustomBaudRateSettings>&)serialMIDI);
//// End MIDI

Instrument default_instrument;
InstrumentHandler handler(default_instrument);
NormalWrangler wrangler;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void midiHandleNoteOn(byte channel, byte note, byte velocity){
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  wrangler.handleNoteOn(note);
}

void midiHandleNoteOff(byte channel, byte note, byte velocity){
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  wrangler.handleNoteOff(note);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Initialize IO
  Serial.begin(SERIAL_BAUD_RATE);

  
  IO::init();
  SPI.begin();

  // Initializing IO Bus will cause a reset pulse on the Reset line. Let's wait here for a bit 'till everything starts up 
  // This should probably just take less than 100ms... but hey... a bit extra doesn't hurt!
  delay(1000);

  // Init display, will show the logo
  // Allows us to already start printing information
  UI::init();

  // Prepare CODEC  
  Codec::write(Codec::Registers::OPL_LEFT,  3); // Enable left  sound output
  Codec::write(Codec::Registers::OPL_RIGHT, 3); // Enable right sound output

  // Make sure voices have a proper pointer to an instrument
  // This also initializes the OPL3 chip.
  // OPL::test();
  VoiceRack::init(default_instrument);

  // Initialize MIDI
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(midiHandleNoteOn);
  MIDI.setHandleNoteOff(midiHandleNoteOff);

  delay(500); digitalWrite(LED_BUILTIN, HIGH);

  for(int i = 0; i < 6; i++)
  {
    VoiceRack::voice[i].loadToOPL();
  }

  //Display::clearDisplay();
  delay(1000);
}


auto current_millis = millis();
auto last_frame = current_millis;
void loop()
{
  MIDI.read();
  wrangler.update();
  current_millis = millis();
  if(current_millis - last_frame >= 100)
  {
    last_frame = current_millis;
    for(int i = 0; i < 6; i++){
      if(wrangler.held_key[i] != -1){
        Display::print(String(wrangler.held_key[i]), i*21, 6, (wrangler.held_key[i] >> 4)-8);
        Display::print(String(wrangler.held_key[i]), i*21, 7, (wrangler.held_key[i] >> 4));
      }
      else {
        Display::print("    ", i*21, 6, 0);
        Display::print("__  ", i*21, 7, 0);
      }
    }
  }
}

#include <Arduino.h>
#include <cstdint>
#include <MIDI.h>
#include "iobus.h"
#include "codec.h"
#include "display.h"
#include "instrument.h"
#include "instrument_handler.h"
#include "voice_rack.h"
#include "wrangler.h"

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
  for(int i = 0; i < 6; i++)
  {
    VoiceRack::voice[i].loadToOPL();
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Initialize IO
  Serial.begin(SERIAL_BAUD_RATE);

  delay(1000);
  
  IO::init();
  IO::setModeInput();
  //SPI.begin();

  // Initializing IO Bus will cause a reset pulse on the Reset line. Let's wait here for a bit 'till everything starts up 
  // This should probably just take less than 100ms... but hey... a bit extra doesn't hurt!
  delay(500);

  // Init display, will show the logo
  // Allows us to already start printing information
  //Display::init();   

  // Prepare CODEC
  Codec::write(Codec::Registers::OPL_LEFT,  0); // Enable left  sound output
  Codec::write(Codec::Registers::OPL_RIGHT, 0); // Enable right sound output

  // Make sure voices have a proper pointer to an instrument
  // This also initializes the OPL3 chip.
  //VoiceRack::init(default_instrument);
  OPL::write(0x20, 0xF0);
  OPL::write(0x40, 0xF0);
  OPL::write(0x60, 0xF0);
  OPL::write(0x80, 0x70);
  OPL::write(0xA0, 0b11101000);
  OPL::write(0xB0, 0x35);
  OPL::write(0xC0, 0xFF);

  // Initialize MIDI
  //MIDI.begin(MIDI_CHANNEL_OMNI);
  //MIDI.setHandleNoteOn(midiHandleNoteOn);

  delay(500); digitalWrite(LED_BUILTIN, HIGH);

  
}


auto current_millis = millis();
auto last_frame = current_millis;
void loop()
{
  /*current_millis = millis();
  if(last_frame - current_millis >= 5)
  {
    last_frame = current_millis;*/
    //MIDI.read();
  //}
}


/*
  So we have a concept of how Muffin pipeline is supposed to work.
  We have an input system that we simply call MIDI
  MIDI > SHAPER > WRANGLER > VOICES

  Shapers are MIDI modules, they turn MIDI signals into MIDI signals. Yes!
  Shapers can take a chord and arpeggio-ize it
  Or take a note and harmonize it
  Or take a note and make sure it fits in a scale
  etc

  Wranglers are the ones to assign voices to each note. 
  Wranglers decide whether the synth works in Poly/Mono
  Wranglers are also the ones to generate slides, and portamento in mono mode.

  TODO:
    Shapers are nor something we are gonna worry about just yet. Shapers are pretty simple but
    also kind of a niche feature, not a necessity.
    We need to begin development om the Wranglers. Preferably the simplest kind:
    The on/off polyphonic wrangler. 


  Things we need:
  MIDI IN module:
    MIDI RX LED
    MIDI Wrapper. (For now we just call the Wrangler's noteOn() call back. 

  Wrangler Module:
    Wranglers are a bit of a global thing, so they don't need to be sotred in the Instrument object
    This means they support having hardware based controls, rather than just proxies.
    6-voice activity LED feedback
    Mono/Poly switch
    Slide time (This could be a potentiometer really!)
    Porta Switch
    We need to implement some sort of map from Velocity to the proper operators that output signals on that algorithm.
    Different algorithms require different operators to set the volumes.
    Perhaps this could be processed on the voice side, using pointers?
    Could have a global Total Level variable that operators that are the last in the chain 
    can reference rather than their own Total Level.
    Or, a simple for loop between all the operators that require TL. 

  Voices:
    Algorithm
    Channel:
      Stereo Switches (toggles)
      

    Operator:
      Total Level (Almost Forgot!)
      Waveform
      Multiplier
      Attack
      Decay
      Sustain
      Release
      
    

*/

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


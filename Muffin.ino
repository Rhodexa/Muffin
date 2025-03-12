#include <Arduino.h>
//#include "synth.h"

#include "iobus.h"
#include "opl.h"
#include "codec.h"
#include "instrument.h"
#include "instrument_handler.h"
#include "voice.h"

Instrument default_instrument;
InstrumentHandler handler(default_instrument);
// Let's make one voice for now... just for testing. This is gonna be allocated on Voice slot 0 and its gonna use the deafult instrument, of course
Voice voice(0, default_instrument);

// Default instrument is a simple Sine Wave
void makeDefaultInstrument() {
  // First, we need to choose which of all 3 channels on this voice to edit.
  handler.setActiveChannel(0);  // On channel 0 (No 4OP available)

    // Now let's edit this voice's operators
    handler.setActiveOperator(1); // By default, channels of FM mode, so to hear something we need to edit its carrier (Operator 2)
      handler.op_setAmplitude(0);      // Max. amplitude
      handler.op_setAttack(16);        // -
      handler.op_setDecay(0);          // - Set instantaneous reaction (Max attack and release)
      handler.op_setSustain(16);       // -
      handler.op_setRelease(16);       // -
      // that's it. CHAN0 -> OP1 is set for sine wave.
    
  // Now onto the channel-specific configurations
  handler.ch_setStereoSwitches(0b11); // Enable sound ouput on Left and Right
  handler.ch_setMultiplier(1 << 16);  // No frequency scaling

  // Done. That's all we need for a simple sine wave! (I hope)
}

// use PC13 to show activity
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  IO::init();
  IO::setMode(0);
  Codec::write(Codec::Registers::OPL_LEFT,  0); // Enable left  sound output
  Codec::write(Codec::Registers::OPL_RIGHT, 0); // Enable right sound output

  // Let's make sure the default instrument is properly set.
  makeDefaultInstrument();

  // Now onto the voice!
  voice.setFrequency(1952); // Not in hertz! This frequency is in OPL3 base... yeah! Whacky, I know! I wrote it!
  voice.setNoteOn(true);    // Tell the voice it can sound!
  voice.loadToOPL();        // Load the voice's contents into the OPL3 chip. (By now it should be playing a sine wave already)

  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
}

uint8_t count = 0; 
void loop() {
  //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  count++;
}

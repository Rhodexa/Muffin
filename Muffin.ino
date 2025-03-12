#include "synth.h"

#include <Arduino.h>
#include <cstdint>
#include "iobus.h"
#include "opl.h"
#include "codec.h"
#include "instrument.h"
#include "instrument_handler.h"
#include "voice.h"
/*
// Default instrument is a simple Sine Wave
void makeDefaultInstrument() {
  // First, we need to choose which of all 3 channels on this voice to edit.
  handler.setActiveChannel(0);  // On channel 0 (No 4OP available)

    // Now let's edit this voice's operators
    handler.setActiveOperator(1); // By default, channels of FM mode, so to hear something we need to edit its carrier (Operator 2)
      handler.op_setAmplitude(0);      // Max. amplitude
      handler.op_setAttack(15);        // -
      handler.op_setDecay(0);          // - Set instantaneous reaction (Max attack and release)
      handler.op_setSustain(15);       // -
      handler.op_setRelease(15);       // -
      // that's it. CHAN0 -> OP1 is set for sine wave.
    
  // Now onto the channel-specific configurations
  handler.ch_setStereoSwitches(0b11); // Enable sound ouput on Left and Right
  handler.ch_setMultiplier(1 << 13);  // No frequency scaling
  // Done. That's all we need for a simple sine wave! (I hope)
}*/

constexpr uint16_t lut_base_frq[13] = {
  488, 517, 547, 580, 614, 651,
  690, 731, 774, 820, 869, 921,
  976
};

Voice voices[6];
uint8_t m_current_voice = 0;
uint8_t held_key[6] = {0, 0, 0, 0, 0, 0};

uint16_t pitchToFrequency(uint32_t q16_pitch){
  if (q16_pitch >= 18 && q16_pitch < 114) {
    q16_pitch -= 18;
    uint8_t int_part = (q16_pitch >> 8);
    uint8_t frc_part =  q16_pitch & 0xFF;
    uint16_t octave = ((uint32_t)int_part * 1366) >> 14;
    uint8_t index = int_part - octave * 12;
    uint16_t frq_a = lut_base_frq[index];
    uint16_t frq_b = lut_base_frq[index + 1];
    return ( frq_a + ((frc_part * (frq_b - frq_a)) >> 16) ) << octave;
  }
  return 0;
}

void handleNoteOn(uint8_t pitch){
  // Find the next available voice
  for (uint8_t i = 0; i < 6; i++){
    m_current_voice++;
    if (m_current_voice >= 6) m_current_voice = 0;
    if (voices[m_current_voice].is_active != true) break;
  }
  held_key[m_current_voice] = pitch;
  voices[m_current_voice].setFrequency(pitchToFrequency((pitch + 18) << 16));
  voices[m_current_voice].setNoteOn(true);
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

  IO::init();
  IO::setMode(0);
  Codec::write(Codec::Registers::OPL_LEFT,  0); // Enable left  sound output
  Codec::write(Codec::Registers::OPL_RIGHT, 0); // Enable right sound output
  OPL::write(0x105, 1); // Enable OPL3 Features

  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
}

uint16_t count = 0; 
void loop() {
}
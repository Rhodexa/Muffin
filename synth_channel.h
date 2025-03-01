#ifndef SYNTH_CHANNEL_H
#define SYNTH_CHANNEL_H

namespace Synth {
namespace Channel {

struct Channel {
  uint16_t offset;
  uint8_t REG_A0;
  uint8_t REG_B0;
  uint8_t REG_C0;

  int multiplier;
  uint8_t stereo_switches;
  bool is_active;
};

/* Getters */

/* Setters */
void setNoteOn(Channel& channel, bool value){
  channel.is_active = value;
}


}} // End namespace Synth::Channel
#endif

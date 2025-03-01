#ifndef CHANNEL_H
#define CHANNEL_H

class {
public:
  uint16_t offset;
  uint8_t REG_A0;
  uint8_t REG_B0;
  uint8_t REG_C0;

  int multiplier;
  uint8_t stereo_switches;
  bool is_active;

public:
  void setNoteOn(Channel& channel, bool value);
}

#endif

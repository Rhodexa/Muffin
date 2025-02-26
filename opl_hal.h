#include "types.h"

uint16 map_to_real_addr[] = {
  0x08,
  0x20, 0x21, 0x22, 0x28, 0x29, 0x2A, 0x30, 0x31, 0x32,
};

addr_t inline get_voiceChannelOffset(uint8_t channel){
  if (channel < 3) return channel;
  return channel + 0x100;
}

class OPL {
  void update_ChannelBase(uint8_t base, uint8_t voice_index){
    addr_t effective_index = get_voiceChannelOffset(voice_index);
    hw_write(base, 0x00100000);
    hw_write(base + 8, 0x00100000);
    hw_write(base + 16, 0x00100000);
  }

  void setAlgorithm(uint8_t algorithm) {
    switch(algorithm){
      case 0:
        hw_write()
    }
  }
}

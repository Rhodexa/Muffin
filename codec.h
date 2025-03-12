#ifndef CODEC_H
#define CODEC_H

// CS4231A CODEC by Crystal

/*
  All there's here is the interface code to blast data to the CS4231A.
  This is not a driver, we only handle write timings here. Actual sound driving is in another castle!
*/

#include <cstdint>
#include "iobus.h"

namespace Codec {
  enum Registers {
    ADC_LEFT, ADC_RIGHT,
    OPL_LEFT, OPL_RIGHT,
    AUX2_LEFT, AUX2_RIGHT,
    DAC_LEFT, DAC_RIGHT,
    // ... other registers we don't need
    ERROR_AND_INITIALIZATION = 11,
    MODE_AND_ID,
    LOOPBACK,
    // ... other registers we don't need
    ALT_FEATURES1 = 16,
    ALT_FEATURES2,
    LINE_LEFT, LINE_RIGHT,
    // ... other registers we don't need
    ALT_FEATURES3 = 23,
    ALT_FEATURES_STATUS
  };

  void write(uint8_t address, uint8_t data);
  uint8_t read(uint8_t address);  
}

#endif
#ifndef OPL_HW_H
#define OPL_HW_H

/*
  All there's here is the interface code to blast data to the YMF262.
  This is not a driver, we only handle write timings here. Actual sound driving is in another castle!
*/

#include <cstdint>
#include "iobus.h"

namespace OPL {
  void write(uint16_t address, uint8_t data);
  uint8_t read();
}


#endif
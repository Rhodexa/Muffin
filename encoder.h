#include <cstdint>

namespace Encoder {
  volatile uint8_t angle = 0;

  inline void init()
  {
    attachInterrupt(digitalPinToInterrupt(PA0), []() {
      if (digitalRead(PA0) == digitalRead(PA1)) angle--;
      else angle++;
    }, CHANGE);

    attachInterrupt(digitalPinToInterrupt(PA1), []() {
      if (digitalRead(PA0) == digitalRead(PA1)) angle++;
      else angle--;
    }, CHANGE);
  }
}

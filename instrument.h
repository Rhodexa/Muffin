#ifndef INSTRUMENT_H
#define INSTRUMENT_H
#include "operator.h"
#include "oplhw.h"





class Synth {
  Voice voice[6];
  Synth(){
    voice[0].setOffset(0x000);
    voice[1].setOffset(0x001);
    voice[2].setOffset(0x002);
    voice[3].setOffset(0x100);
    voice[4].setOffset(0x101);
    voice[5].setOffset(0x102);
  }
}
#endif
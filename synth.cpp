#include "synth.h"

void Synth::setAlgorithm(uint8_t voice, uint8_t algorithm){
  if(voice < 4) REG_104 &= ~(1 << voice);
  else REG_104 |= (1 << voice);
  voices[voice].setAlgorithm(algorithm);    
}
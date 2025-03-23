#include "voice_rack.h"

namespace VoiceRack
{
  void init(Instrument& instrument)
  {
    // Enable OPL3 Features
    OPL::write(0x105, 1); 

    // Init voices with a default instrument. We don't want nullptrs hangin' 'round
    for(int i = 0; i < 6; i++)
    {
      voice[i].setInstrument(instrument);
      voice[i].assingIndex(i);
    }
  }
}
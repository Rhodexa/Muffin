# Muffin
6-voice MIDI-compatible synth module using OPL3

---

| MIDI | Shaper | Wrangler | Voice | Channel | Operator |
| ---- | ------ | -------- | ----- | ------- | -------- |
| RX LED | >>> | 0 0 0 0 0 0 <br> Poly/Mono <br> Porta | Algorithm <br> Macros | < stereo > <br> Multiplier | A - 0 0 0 0 <br> D - 0 0 0 0 <br> S - 0 0 0 0 <br> R - 0 0 0 0 <br> Total Level <br> Harmonic <br> Waveform |

  So we have a concept of how Muffin pipeline is supposed to work.
  We have an input system that we simply call MIDI
  MIDI > SHAPER > WRANGLER > VOICES

  Shapers are MIDI modules, they turn MIDI signals into MIDI signals. Yes!
  Shapers can take a chord and arpeggio-ize it
  Or take a note and harmonize it
  Or take a note and make sure it fits in a scale
  etc

  Wranglers are the ones to assign voices to each note. 
  Wranglers decide whether the synth works in Poly/Mono
  Wranglers are also the ones to generate slides, and portamento in mono mode.

  TODO:
    Shapers are nor something we are gonna worry about just yet. Shapers are pretty simple but
    also kind of a niche feature, not a necessity.
    We need to begin development om the Wranglers. Preferably the simplest kind:
    The on/off polyphonic wrangler. 


  Things we need:
  MIDI IN module:
    MIDI RX LED
    MIDI Wrapper. (For now we just call the Wrangler's noteOn() call back. 

  Wrangler Module:
    Wranglers are a bit of a global thing, so they don't need to be sotred in the Instrument object
    This means they support having hardware based controls, rather than just proxies.
    6-voice activity LED feedback
    Mono/Poly switch
    Slide time (This could be a potentiometer really!)
    Porta Switch
    We need to implement some sort of map from Velocity to the proper operators that output signals on that algorithm.
    Different algorithms require different operators to set the volumes.
    Perhaps this could be processed on the voice side, using pointers?
    Could have a global Total Level variable that operators that are the last in the chain 
    can reference rather than their own Total Level.
    Or, a simple for loop between all the operators that require TL. 

  Voices:
    Algorithm
    Channel:
      Stereo Switches (toggles)
      

    Operator:
      Total Level (Almost Forgot!)
      Waveform
      Multiplier
      Attack
      Decay
      Sustain
      Release
      
    
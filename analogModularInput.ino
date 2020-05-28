#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  MIDI.begin(4);
}

void loop() {
  int val = analogRead(A0);
  if (val == 0) {
    val = 63;
  } else {
    val = map(val, 0, 1023, 1, 127);
  }
  
  
  MIDI.sendControlChange(50, val, 10);
  MIDI.sendControlChange(51, val, 10);
  MIDI.sendControlChange(52, val, 10);
  MIDI.sendControlChange(53, val, 10);
}

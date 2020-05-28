#define CV_KICK 3
#define CV_SNARE 4
#define CV_LOTOM 5
#define CV_HITOM 6
#define CV_CLHAT 7
#define CV_OPHAT 8
#define CV_CLAP 9
#define CV_CLAVES 10
#define CV_AGOGO 11
#define CV_CRASH 12

const int notes[10] = {
  36, // Kick
  38, // Snare
  43, // Lo Tom
  50, // Hi Tom
  42, // Closed Hat
  46, // Open Hat
  39, // Clap
  75, // Claves
  67, // Agogo
  49, // Crash
};

void setup() {
   Serial.begin(31250);
}

void loop() {
 for (byte i = 3; i <= 12; i++) {
    if (digitalRead(i) == HIGH) {
      playNote(9, notes[i - 3]);
    }
  }
} 

void playNote(byte channel, byte note) {
  Serial.write(0x90 + channel);
  Serial.write(note);
  Serial.write(127);
}
